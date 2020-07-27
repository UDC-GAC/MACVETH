#!/env/python3

import sys
import os
import itertools as it
import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt
import numpy as np

if (len(sys.argv) < 6):
    print("Usage: python3 analyze.py <bench> <arch> <comp> <ops> <output>")
    exit(0)

bench = sys.argv[1]
arch = sys.argv[2]

bench_dir = bench + "/"
results_dir = "results_" + arch + "/results/" + bench_dir
output_dir = "output/"

os.system("mkdir -p %s" % output_dir)

comp = list(sys.argv[3].split(","))
vect = list(sys.argv[4].split(","))

output_file = output_dir + sys.argv[5]

files = []
for c in it.product(comp, vect):
    files += [results_dir + c[0] + "_" + c[1] + "_" + bench + ".csv"]
    files += [results_dir + c[0] + "_" + c[1] + "_" + bench + "_macveth.csv"]

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
f.savefig(output_file + ".pdf")
df['arch'] = arch
df.to_csv(output_file + ".csv")
