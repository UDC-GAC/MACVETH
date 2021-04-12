#!/env/python3

import sys


def generate_codelet_1d(t1, t2, orig_y="ORIG_y", orig_A="ORIG_A", orig_x="ORIG_x"):
    print(f"for (i = {t2[1]}; i <= {t2[0]}; ++i) {{")
    print(
        f"    y[{t1[0]}*i + {orig_y}] += A[i + {orig_A}] * x[{t1[1]}*i + {orig_x}];")
    print(f"}}")


def generate_codelet_2d(t1, t2, orig_y="ORIG_y", orig_A="ORIG_A", orig_x="ORIG_x"):
    print(f"for (i = {t2[2]}; i <= {t2[0]}; ++i) {{")
    print(f"    for (j = {t2[3]}; j <={t2[1]}; ++j) {{")
    print(
        f"        y[{t1[0]}*i + {t1[1]}*j + {orig_y}] += A[{int(t2[1])+1}*i + j + {orig_A}] * x[{t1[2]}*i + {t1[3]} + {orig_x}];")
    print(f"     }}")
    print(f"}}")


n_codelets = -1
if (len(sys.argv) > 1):
    n_codelets = int(sys.argv[1])

f = open("pattern-info.txt")
# number of file
n = 0
for l in f:
    if (n_codelets != -1) and (n >= n_codelets):
        sys.exit(0)
    l = l.replace("],", "];")
    tokens = l.split(";")
    t1 = tokens[0].replace("[", "").replace("]", "").split(",")
    t2 = tokens[1].replace("[", "").replace("]", "").split(",")
    tokens = tokens[2].split(",")
    count = tokens[0]
    flops = tokens[1]
    file_name = f"codelet_{int(n):08d}.c"
    with open(file_name, "w+") as f:
        sys.stdout = f
        if len(t1) > 2:
            generate_codelet_2d(t1, t2)
        else:
            generate_codelet_1d(t1, t2)
    n = n + 1
