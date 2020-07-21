#!/env/python3

import sys
import os
import itertools as it
import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt
import numpy as np

if (len(sys.argv) < 5):
    print("Usage: python3 comp_exec_analyze.py <bench> <res_dir> <comp> <ops> <nexec>")
    exit(0)

bench = sys.argv[1]

bench_dir = bench + "/"
results_dir = "results" + sys.argv[2] + "/"
output_dir = "output/"

os.system("mkdir -p %s" % results_dir + bench)
os.system("mkdir -p %s" % output_dir)

comp = list(sys.argv[3].split(","))
vect = list(sys.argv[4].split(","))
n_exec = 7
if (len(sys.argv) == 6):
    n_exec = int(sys.argv[5])

s = "python3 time_benchmark.py"

os.system("make -C %s clean" % (bench))

for c in it.product(comp, vect):
    os.system("make -C %s %s COMP=%s VECT=%s" % (bench, bench, c[0], c[1]))
    comp_opts = c[0] + "_" + c[1]
    executable = bench_dir + c[0] + "_" + c[1] + "_" + bench + ".o"
    output = results_dir + executable[:-2] + ".csv"
    os.system("%s %s %s %s %s %d" %
              (s, executable, comp_opts, output, "0", n_exec))
    executable = bench_dir + c[0] + "_" + c[1] + "_" + bench + "_macveth.o"
    output = results_dir + executable[:-2] + ".csv"
    os.system("%s %s %s %s %s %d" %
              (s, executable, comp_opts, output, "1", n_exec))
