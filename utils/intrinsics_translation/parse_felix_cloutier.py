#!/env/python3

import requests
import re
from bs4 import BeautifulSoup
from tqdm import tqdm

base_url = "https://www.felixcloutier.com/x86/"
html_text = requests.get(base_url).text
base_soup = BeautifulSoup(html_text, 'html.parser')

return_vals = [
    "__m64",
    "__m128i",
    "__m128",
    "__m128d",
    "__m256i",
    "__m256",
    "__m256d",
    "__m512i",
    "__m512",
    "__m512d",
    "void",
    "char",
    "extern unsigned char",
    "int",
    "int64_t",
    "double",
    "float",
    "__mmask8",
    "__mmask16",
    "__mmask32",
    "__mmask64",
]

iform_suffixes_map = {
    "__m128": "XMM",
    "__m128i": "XMM",
    "__m128d": "XMM",
    "__m256": "YMM",
    "__m256i": "YMM",
    "__m256d": "YMM",
    "__m512": "ZMM",
    "__m512i": "ZMM",
    "__m512d": "ZMM",
}

L = {}

# ad-hoc parsing: this page only has two Intel links; rest are to ASM webpages

for link in base_soup.find_all('a'):
    if "intel" in link.get('href'):
        continue
    asm_url = base_url + link.get('href')[2:]
    html_text = requests.get(asm_url).text
    asm_soup = BeautifulSoup(html_text, 'html.parser')
    if (asm_soup.find(id="intel-c-c++-compiler-intrinsic-equivalent") == None):
        continue
    # HTML does not have a lot of sematic more than above so, hardcore pattern
    # matching for AVX<= instructions
    sse = False
    other = False
    lines = asm_soup(text=re.compile(r' __m'))
    if len(lines) == 0:
        # SSE
        lines = asm_soup(text=re.compile(r' _mm'))
        sse = True
    if len(lines) == 0:
        lines = asm_soup(text=re.compile(r' _'))
        other = True
    if len(lines) == 0:
        print("ASM " + link.get('href')[2:-5] + " not found...")
        continue
    if (sse):
        print("SSE", sep=" ")
    if (other):
        print("Other", sep=" ")
    print("ASM " + link.get('href')[2:-5])
    L[link.get('href')[2:-5]] = lines

tmp = {}
for k in L.keys():
    for inst in L[k]:
        tok = inst.split(" ")
        if ":" in k:
            asm = tok[0]
        else:
            asm = k
        # FMADD/SUB case
        if "xxx" in asm:
            asm.replace("xxx", "231")
        ret = ""
        intrinsics = ""
        for t in tok:
            if t in return_vals:
                ret = t
            elif t[0] == "_":
                bracket = t.find("(")
                if (bracket != -1):
                    intrinsics = t[:bracket]
                else:
                    instrinsics = t
            elif t[0] == "(":
                break
        asm_iform = asm + get_ops()
        tmp[asm_iform] = [ret, intrinsics]
        print("asm " + asm + " = " + str(tmp[asm]))
