"""Per-function inventory: where it is, who wrote it, is it compiled, is it used."""
import json, os, re, sys
repo, prov = sys.argv[1], sys.argv[2]
OLD = f'{repo}/M300S/MG380U_V1.0'
files = [l.strip() for l in open(f'{prov}/../proj_files.txt') if l.strip().endswith('.c')]
axf = json.load(open(f'{prov}/funcs.json'))

KW = {'if', 'for', 'while', 'switch', 'return', 'sizeof', 'else', 'do', 'case'}

def strip(text):
    """Remove comments and string/char literals, keep newlines."""
    out, i, n = [], 0, len(text)
    while i < n:
        c = text[i]
        if text.startswith('//', i):
            j = text.find('\n', i); j = n if j < 0 else j; i = j
        elif text.startswith('/*', i):
            j = text.find('*/', i + 2); j = n if j < 0 else j + 2
            out.append('\n' * text.count('\n', i, j)); i = j
        elif c in '"\'':
            j = i + 1
            while j < n and text[j] != c:
                j += 2 if text[j] == '\\' else 1
            out.append(c + c); i = j + 1
        else:
            out.append(c); i += 1
    return ''.join(out)

DEF = re.compile(r'([A-Za-z_]\w*)\s*\(((?:[^();{}]|\([^();{}]*\))*)\)\s*$', re.S)

def defs(text, keep_line=None):
    """Find top-level function definitions. keep_line(lineno)->line or None to skip."""
    t = strip(text); res = []; depth = 0; last = 0
    for i, c in enumerate(t):
        if c == '{':
            if depth == 0:
                head = t[last:i]
                hl = head.rsplit(';', 1)[-1].rsplit('}', 1)[-1]
                hl = re.sub(r'^\s*#.*$', '', hl, flags=re.M)
                m = DEF.search(hl)
                if m and m.group(1) not in KW and '=' not in hl[:m.start()] and not re.search(r'\b(struct|enum|union)\s*\w*\s*$', hl[:m.start()]):
                    ln = t.count('\n', 0, last + head.find(hl) + hl.find(m.group(1), m.start())) + 1
                    loc = keep_line(ln) if keep_line else ln
                    if loc: res.append((m.group(1), loc))
            depth += 1
        elif c == '}':
            depth -= 1
            if depth == 0: last = i + 1
        elif c == ';' and depth == 0:
            last = i + 1
    return res

def bodies(path):
    """name -> whitespace-normalised source of the whole definition (first occurrence)."""
    try: t = strip(open(path, encoding='latin-1').read())
    except FileNotFoundError: return {}
    res = {}; lines = t.split('\n')
    starts = defs(t)
    for n, ln in starts:
        if n in res: continue
        pos = sum(len(x) + 1 for x in lines[:ln - 1])
        i = t.find('{', pos); depth = 0; j = i
        while j < len(t):
            if t[j] == '{': depth += 1
            elif t[j] == '}':
                depth -= 1
                if depth == 0: break
            j += 1
        res[n] = re.sub(r'\s+', '', t[pos:j + 1])
    return res

def raw_defs(path):
    try: return defs(open(path, encoding='latin-1').read())
    except FileNotFoundError: return None

def pp_defs(path, src):
    """Definitions that survive preprocessing, mapped back to original line numbers in src."""
    text = open(path, encoding='latin-1').read()
    lines = text.split('\n'); mapping = []; cur_f, cur_l = None, 0
    for l in lines:
        m = re.match(r'# (\d+) "([^"]+)"', l)
        if m: cur_f, cur_l = m[2], int(m[1]); mapping.append((None, None)); continue
        mapping.append((cur_f, cur_l)); cur_l += 1
    clean = '\n'.join('' if l.startswith('# ') else l for l in lines)
    base = os.path.basename(src)
    def keep(ln):
        f, l = mapping[ln - 1] if ln - 1 < len(mapping) else (None, None)
        return l if f and os.path.basename(f) == base else None
    return defs(clean, keep)

FILE_ORIGIN = {}
for f in files:
    head = open(f'{repo}/{f}', encoding='latin-1').read(3000)
    b = os.path.basename(f)
    in_old = any(os.path.exists(f'{OLD}/{d}/{b}') for d in ('src', 'app', 'Led_Effect_Gruop_Code'))
    if re.search(r'@author\s*chengjie', head): o = ('new', '🟩 本项目新写（chengjie）')
    elif 'ARM Limited' in head and 'Keil' not in head: o = ('vendor', '⬜ ARM / NXP 官方')
    elif 'Keil' in head: o = ('vendor', '⬜ Keil USB 例程（NXP 芯片）')
    elif re.search(r'NXP LPC11xx USB example|NXP Semiconductors', head) and 'gtech' not in head.lower(): o = ('vendor', '⬜ NXP USB 例程')
    elif 'World Peace' in head: o = ('agent', '⬜ 方案商（世平 World Peace）')
    elif 'Cypress' in head: o = ('old', '⬜ 公司老代码（从 Cypress 芯片项目搬来）')
    elif not in_old: o = ('new2', '🟩 本项目新加（未署名）')
    else: o = ('old', '⬜ 公司老代码（M300S 就有）')
    FILE_ORIGIN[f] = (o, in_old)

ax = {}
for a in axf:
    ax.setdefault((a['obj'].split('(')[0], a['name']), a)

out = []
for f in files:
    b = os.path.basename(f); obj = b[:-2].lower() + '.o'
    (okey, olabel), in_old = FILE_ORIGIN[f]
    raw = raw_defs(f'{repo}/{f}')
    pp = pp_defs(f'{prov}/pp/{b}.i', f)
    ppn = {n for n, _ in pp}
    oldn = None; oldb = {}; newb = bodies(f'{repo}/{f}')
    if in_old:
        for d in ('src', 'app', 'Led_Effect_Gruop_Code'):
            r = raw_defs(f'{OLD}/{d}/{b}')
            if r is not None: oldn = {n for n, _ in r}; oldb = bodies(f'{OLD}/{d}/{b}'); break
    srclines = open(f'{repo}/{f}', encoding='latin-1').read().split('\n')
    seen = set(); fl = []
    for n, ln in raw:
        if (n, ln) in seen: continue
        seen.add((n, ln))
        a = ax.get((obj, n))
        if n not in ppn: st = 'off'
        elif a is None:
            ctx = ' '.join(srclines[max(0, ln - 3):ln])
            st = 'inline' if re.search(r'\b(__inline|inline|__INLINE|__STATIC_INLINE)\b', ctx) else 'gone'
        elif a['reachable']: st = 'used'
        else: st = 'dead'
        who = None
        if oldn is not None: who = ('same' if oldb.get(n) == newb.get(n) else 'changed') if n in oldn else 'added'
        fl.append(dict(name=n, line=ln, status=st, size=a['size'] if a else 0, who=who,
                       callers=a['callers'] if a else [], root=a['root'] if a else []))
    # same name defined in several #if branches: only the compiled one keeps its status
    names = {}
    for x in fl: names.setdefault(x['name'], []).append(x)
    for n, xs in names.items():
        if len(xs) > 1 and any(x['status'] != 'off' for x in xs):
            ppl = {ln for nn, ln in pp if nn == n}
            for x in xs:
                if x['line'] not in ppl: x['status'] = 'off'
    out.append(dict(file=f, origin=okey, label=olabel, in_old=in_old, funcs=fl))
json.dump(out, open(f'{prov}/inventory.json', 'w'), ensure_ascii=False, indent=1)
tot = {}
for F in out:
    for x in F['funcs']: tot[x['status']] = tot.get(x['status'], 0) + 1
print(tot)
for F in out:
    for x in F['funcs']:
        if x['status'] in ('inline', 'gone'): print(F['file'], x['status'], x['name'])
