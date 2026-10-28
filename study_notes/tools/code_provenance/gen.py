"""Write the data file for the page (study_notes/easy/代码来源清单.html)."""
import json, os, re, sys
repo, prov = sys.argv[1], sys.argv[2]
I = json.load(open(f'{prov}/inventory.json'))
notin = [l.strip() for l in open(f'{prov}/notin.txt') if l.strip()]

ST = {'used': '✅ 在用', 'inline': '✅ 在用（被内联）', 'dead': '⚠️ 编进去了但没人调用',
      'gone': '🗑️ 编译了但被链接器删掉（没人引用）', 'off': '⛔ 没编译（被 #if 关掉）'}
WHO = {'same': '老项目原样', 'changed': '老项目有，本项目改过', 'added': '本项目在老文件里新加', None: '—'}

def head_info(f):
    h = open(f'{repo}/{f}', encoding='latin-1').read(1500)
    m = re.search(r'@author\s*(\w+)', h) or re.search(r'Writer\s+(\w+)', h)
    return m.group(1) if m else ''

rows = []
for F in I:
    c = {}
    for x in F['funcs']: c[x['status']] = c.get(x['status'], 0) + 1
    used_b = sum(x['size'] for x in F['funcs'] if x['status'] == 'used')
    dead_b = sum(x['size'] for x in F['funcs'] if x['status'] == 'dead')
    F['counts'] = c; F['used_b'] = used_b; F['dead_b'] = dead_b; F['author'] = head_info(F['file'])
    F['lines'] = sum(1 for _ in open(f"{repo}/{F['file']}", encoding='latin-1'))

md = []
md.append('# 代码来源清单（IRPT_V4）\n')
md.append('> 自动生成：`List/Sample.map` + `output/Sample.axf`（2026-10-20 编译的那一版）+ 源码预处理（按 `inc/my_board.h` 里的 `IRPT_V4`）+ 和公司老项目 `M300S/MG380U_V1.0` 逐个函数比对。网页版（能筛选）：见 `study_notes/easy/README.md` 里的“代码来源清单”。\n')
md.append('## 怎么读\n')
md.append('| 标记 | 意思 |\n|---|---|')
for k, v in ST.items(): md.append(f'| {v} | ' + {
    'used': '从复位入口或中断向量表顺着调用关系能走到它，程序真的会执行',
    'inline': '函数体被编译器直接塞进了调用它的地方，单独看不到，但在用',
    'dead': '编译进了程序、占着 Flash，但从入口走不到它：**删掉也不影响功能**（删之前仍要编译 + 实测确认）',
    'gone': '编译了，但没有任何地方引用，链接器自动扔掉了，不占 Flash',
    'off': '外面包着 `#if 0`、`#if (defined(别的板型))` 等，IRPT_V4 下根本没编译'}[k] + ' |')
md.append('\n“和老项目比”一栏：**老项目原样** = M300S 里一字不差（不算注释和空白）；**改过** = M300S 也有但内容不同；**新加** = M300S 的同名文件里没有这个函数。整个文件是本项目新写的，这一栏是 —。\n')
md.append('⚠️ 局限：只认得直接调用和存在 Flash 里的函数指针；宏里拼出来的调用、汇编里的跳转都按这两种方式处理，一般不会漏，但删任何函数前都要先编译、再实测。\n')

tot = {}
for F in I:
    for k, v in F['counts'].items(): tot[k] = tot.get(k, 0) + v
md.append('## 总览\n')
md.append(f"工程里 {len(I)} 个 .c 文件、{sum(len(F['funcs']) for F in I)} 个函数：" + '，'.join(f"{ST[k]} {tot.get(k,0)}" for k in ST) + '。\n')
md.append('| 文件 | 来源 | 署名 | M300S 有？ | 行数 | ✅ | ⚠️ 没人调用 | ⛔ 没编译 | 在用代码 | 没人调用的代码 |\n|---|---|---|---|---|---|---|---|---|---|')
for F in I:
    c = F['counts']
    md.append(f"| `{F['file']}` | {F['label']} | {F['author']} | {'是' if F['in_old'] else '否'} | {F['lines']} | {c.get('used',0)+c.get('inline',0)} | {c.get('dead',0)} | {c.get('off',0)} | {F['used_b']} B | {F['dead_b']} B |")
md.append('\n### 仓库里有、但不在 Keil 工程里的文件（根本不参与编译）\n')
md.append('、'.join(f'`{f}`' for f in notin) + '\n')
md.append('其中 `src/my_hallkey_func.c`、`src/my_i2c.c` 是 chengjie 写的（霍尔按键 / I²C），目前没加进工程，**T1A 换 GTRY164 霍尔芯片时很可能用得上**，问杰哥。\n')

md.append('## 逐个文件\n')
for F in I:
    md.append(f"### `{F['file']}`　{F['label']}\n")
    if not F['funcs']:
        md.append('只有数据（表格、描述符），没有函数。\n'); continue
    md.append('| 行 | 函数 | 状态 | 和老项目比 | 大小 | 谁调用它 |\n|---|---|---|---|---|---|')
    for x in sorted(F['funcs'], key=lambda x: x['line']):
        who = WHO[x['who']] if F['in_old'] else '—'
        call = '、'.join(x['callers'][:5]) or ('、'.join(x['root']) if x['root'] else '')
        md.append(f"| {x['line']} | `{x['name']}` | {ST[x['status']]} | {who} | {x['size'] or ''} | {call} |")
    md.append('')
json.dump(dict(files=I, notin=notin, tot=tot), open(f'{prov}/page_data.json', 'w'), ensure_ascii=False)
print(tot, len(md))
