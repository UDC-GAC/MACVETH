#!/env/python3

import sys
import os
import itertools as it
import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt
import numpy as np
import utilities.time_benchmark as tbench

if (len(sys.argv) < 7):
    print("Usage: python3 comp_exec.py <bench> <res_dir> <comp> <ops> <mvflags> <nexec>")
    print("\t\te.g. python3 comp_exec.py alexnet tmp_dir gcc nofast TIME '-march=avx2'")
    exit(0)

bench = sys.argv[1]

bench_dir = bench + "/"
results_dir = "results" + sys.argv[2] + "/"
output_dir = "output/"

os.system("mkdir -p %s" % results_dir + bench)
os.system("mkdir -p %s" % output_dir)

comp = list(sys.argv[3].split(","))
vect = list(sys.argv[4].split(","))
t = sys.argv[5]
mvflags = sys.argv[6]

n_exec = 7
if (len(sys.argv) == 8):
    n_exec = int(sys.argv[7])


os.system("make -C %s clean" % (bench))

for c in it.product(comp, vect):

    print("make -C %s %s COMP=%s VECT=%s TYPE=%s MVFLAGS=%s" %
          (bench, bench, c[0], c[1], t, mvflags))
    os.system("make -C %s %s COMP=%s VECT=%s TYPE=%s MVFLAGS=%s" %
              (bench, bench, c[0], c[1], t, mvflags))
    comp_opts = c[0] + "_" + c[1]

    # No macveth
    executable = bench_dir + comp_opts + "_" + bench + "_" + t + ".o"
    output = results_dir + executable[:-2] + ".csv"
    tbench.calculate_mean(executable, comp_opts, output, "0", t, n_exec)

    # Macveth version
    executable = bench_dir + comp_opts + "_" + bench + "_" + t + "_macveth.o"
    output = results_dir + executable[:-2] + ".csv"
    tbench.calculate_mean(executable, comp_opts, output, "1", t, n_exec)
