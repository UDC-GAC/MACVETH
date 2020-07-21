#!/env/python3

import sys
import os
import itertools as it
import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt
import numpy as np

if (len(sys.argv) < 4):
    print("Usage: python3 comp_exec_analyze.py <bench> <output> <analyzeonly> <compexeconly>")

bench = sys.argv[1]
analyze_only = int(sys.argv[3])
comp_exec_only = int(sys.argv[4])

bench_dir = bench + "/"
results_dir = "results/"
output_dir = "output/"

os.system("mkdir -p %s" % results_dir)
os.system("mkdir -p %s" % output_dir)

comp = ["gcc", "icc"]
vect = ["nofast", "fast"]

s = "python3 time_benchmark.py"

files = []
if (not analyze_only):
    os.system("make -C %s clean" % (bench))
for c in it.product(comp, vect):
    if (not analyze_only):
        os.system("make -C %s %s COMP=%s VECT=%s" % (bench, bench, c[0], c[1]))
    comp_opts = c[0] + "_" + c[1]
    executable = bench_dir + c[0] + "_" + c[1] + "_" + bench + ".o"
    output = results_dir + executable[:-2] + ".csv"
    files += [output]
    if (not analyze_only):
        os.system("%s %s %s %s %s" % (s, executable, comp_opts, output, "0"))
    executable = bench_dir + c[0] + "_" + c[1] + "_" + bench + "_macveth.o"
    output = results_dir + executable[:-2] + ".csv"
    files += [output]
    if (not analyze_only):
        os.system("%s %s %s %s %s" % (s, executable, comp_opts, output, "1"))

if (comp_exec_only):
    exit(0)

output_file = output_dir + sys.argv[2]
files = [pd.read_csv(f) for f in files]
df = pd.concat(files).reset_index()
df.drop("index", axis=1, inplace=True)
df["type"] = df.compilation + \
    ["_macveth" if i == 1 else "_org" for i in df.macveth]

comp_mv = []
comp_nomv = []
hue_order = []
palette = []
start_color = 0
for c in comp:
    comp_mv = list(
        np.unique([r for r in df.type if c in r and "macveth" in r]))
    comp_nomv = list(
        np.unique([r for r in df.type if c in r and not "macveth" in r]))
    hue_order += comp_nomv + comp_mv
    palette += sns.cubehelix_palette(len(hue_order), start=start_color)
    start_color += (3 / len(comp))

f = sns.catplot(data=df, x="name", y="gflops", hue="type", hue_order=hue_order,
                kind="bar", palette=palette)
f.set_xticklabels(rotation=45, horizontalalignment="right")
plt.show()
# f.savefig(output_file)
