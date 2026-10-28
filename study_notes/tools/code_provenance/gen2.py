"""Page data with final-origin classification (origins.py)."""
import json, re, sys
sys.path.insert(0, sys.argv[2])
from origins import O, CATS
repo, prov = sys.argv[1], sys.argv[2]
I = json.load(open(f'{prov}/inventory.json'))
notin = [l.strip() for l in open(f'{prov}/notin.txt') if l.strip()]
missing = [F['file'] for F in I if F['file'] not in O] + [f for f in notin if f not in O]
assert not missing, missing
files = []
for F in I:
    cat, base, ev = O[F['file']]
    c = {}
    for x in F['funcs']: c[x['status']] = c.get(x['status'], 0) + 1
    files.append(dict(file=F['file'], cat=cat, base=base, ev=ev, in_proj=True, in_old=F['in_old'],
        lines=sum(1 for _ in open(f"{repo}/{F['file']}", encoding='latin-1')),
        counts=c, used_b=sum(x['size'] for x in F['funcs'] if x['status'] == 'used'),
        dead_b=sum(x['size'] for x in F['funcs'] if x['status'] == 'dead'), funcs=F['funcs']))
files.append(dict(file='src/startup_LPC11Uxx.s', cat=O['src/startup_LPC11Uxx.s'][0], base=O['src/startup_LPC11Uxx.s'][1], ev=O['src/startup_LPC11Uxx.s'][2], in_proj=True, in_old=True, asm=True,
    lines=sum(1 for _ in open(f'{repo}/src/startup_LPC11Uxx.s', encoding='latin-1')), counts={}, used_b=0, dead_b=0, funcs=[]))
for f in notin:
    if f.endswith('.s'): continue
    cat, base, ev = O[f]
    files.append(dict(file=f, cat=cat, base=base, ev=ev, in_proj=False, in_old=None,
        lines=sum(1 for _ in open(f'{repo}/{f}', encoding='latin-1')), counts={}, used_b=0, dead_b=0, funcs=[]))
json.dump(dict(files=files, cats=CATS), open(f'{prov}/page_data2.json', 'w'), ensure_ascii=False)
from collections import Counter
print(Counter((x['cat'], x['in_proj']) for x in files))
