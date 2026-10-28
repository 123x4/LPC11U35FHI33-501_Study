"""Build a call graph of the linked image and report which functions are reachable.

Inputs: List/Sample.map (symbol table) and output/Sample.axf (code).
Output: JSON with every Thumb function: name, address, size, object, callers, reachable.
"""
import json, re, subprocess, sys, struct
from collections import defaultdict, deque

repo, out = sys.argv[1], sys.argv[2]
mp = open(f'{repo}/List/Sample.map', encoding='latin-1').read().split('\n')

funcs = []  # (name, addr(even), size, obj, local)
local = True
for l in mp:
    if 'Global Symbols' in l: local = False
    m = re.match(r'\s+(\S+)\s+0x([0-9a-f]{8})\s+Thumb Code\s+(\d+)\s+(\S+)', l)
    if m:
        funcs.append(dict(name=m[1], addr=int(m[2], 16) & ~1, size=int(m[3]), obj=m[4], local=local))
funcs.sort(key=lambda f: f['addr'])
byaddr = {f['addr']: i for i, f in enumerate(funcs)}

def owner(a):
    lo, hi = 0, len(funcs) - 1
    best = None
    while lo <= hi:
        mid = (lo + hi) // 2
        if funcs[mid]['addr'] <= a:
            best = mid; lo = mid + 1
        else:
            hi = mid - 1
    if best is not None and a < funcs[best]['addr'] + max(funcs[best]['size'], 2):
        return best
    return None

# raw image bytes for literal / table scanning
subprocess.run(['llvm-objcopy', '-O', 'binary', '--only-section=ER_IROM1', f'{repo}/output/Sample.axf', f'{out}/irom.bin'], check=True)
img = open(f'{out}/irom.bin', 'rb').read()
BASE = 0x4000

edges = defaultdict(set)   # caller idx -> callee idx
roots = set()
root_why = defaultdict(set)

# 1) direct branches from disassembly
dis = subprocess.run(['llvm-objdump', '-d', '--triple=thumbv6m', '--no-show-raw-insn', f'{repo}/output/Sample.axf'],
                     capture_output=True, text=True).stdout
for l in dis.split('\n'):
    m = re.match(r'\s*([0-9a-f]+):\s+(bl|b|beq|bne|bcs|bcc|bmi|bpl|bvs|bvc|bhi|bls|bge|blt|bgt|ble|bhs|blo)\s+0x([0-9a-f]+)', l)
    if not m: continue
    src, dst = int(m[1], 16), int(m[3], 16)
    a, b = owner(src), byaddr.get(dst)
    if a is None or b is None or a == b: continue
    edges[a].add(b)

# 2) any word in the image equal to a function address|1 = function pointer
#    (vector table, literal pools, const tables such as USB_P_EP)
for off in range(0, len(img) - 3, 4):
    v = struct.unpack_from('<I', img, off)[0]
    if v & 1 and (v & ~1) in byaddr:
        callee = byaddr[v & ~1]
        a = owner(BASE + off)
        if a is not None and a != callee:
            edges[a].add(callee)
        elif BASE + off < 0x40C0:
            roots.add(callee); root_why[callee].add('中断向量表')
        else:
            roots.add(callee); root_why[callee].add('常量表里的函数指针')

# 3) Reset entry
for i, f in enumerate(funcs):
    if f['name'] in ('Reset_Handler', '__main'):
        roots.add(i); root_why[i].add('复位入口')

reach = set()
q = deque(roots)
while q:
    i = q.popleft()
    if i in reach: continue
    reach.add(i)
    q.extend(edges[i] - reach)

callers = defaultdict(set)
for a, bs in edges.items():
    for b in bs: callers[b].add(a)

res = []
for i, f in enumerate(funcs):
    res.append(dict(f, reachable=i in reach, root=sorted(root_why[i]),
                    callers=sorted({funcs[c]['name'] for c in callers[i]})[:12]))
json.dump(res, open(f'{out}/funcs.json', 'w'), ensure_ascii=False, indent=0)
user = [r for r in res if not re.search(r'\.l\(|^__|^_', r['obj'] + ' ' + r['name']) ]
print(len(res), 'functions;', sum(r['reachable'] for r in res), 'reachable;',
      sum(1 for r in user if not r['reachable']), 'unreachable user funcs')
