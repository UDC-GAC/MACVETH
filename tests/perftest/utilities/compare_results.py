#!/env/python3

import sys
import os
import itertools as it
import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt
import numpy as np

if (len(sys.argv) < 2):
    print("Usage: python3 compare_results.py <file> <file2> [...]")
    exit(0)

files = sys.argv[1:]

files = [pd.read_csv(f) for f in files]

df = pd.concat(files).reset_index()
df.drop("index", axis=1, inplace=True)
df["type"] = df.arch + "_" + df.type

comp_mv = []
comp_nomv = []
hue_order = []
palette = []
start_color = 0
archs = np.unique(df.arch)
for arch in archs:
    comp_mv = list(
        np.unique([r for r in df.type if arch in r and "macveth" in r]))
    comp_nomv = list(
        np.unique([r for r in df.type if arch in r and not "macveth" in r]))
    hue_order += comp_nomv + comp_mv
    palette += sns.cubehelix_palette(len(hue_order), start=start_color)
    start_color += (3 / len(archs))

f = sns.catplot(data=df, x="name", y="gflops", hue="type", hue_order=hue_order,
                kind="bar", palette=palette)
f.set_xticklabels(rotation=45, horizontalalignment="right")
plt.show()
