#!/bin/python3

import itertools as it
import os
import sys
import pandas as pd
from tqdm import tqdm
from ..utilities import time_benchmark as tbench

asm_path = "asm"
test_path = "test"
bin_path = "bin"
results_path = "results"

# This is another approach...


def gen_test(data_type, n, y_idx, A_idx, x_idx):
    d = {2: "two", 3: "three", 4: "four"}
    test = ""
    test += "#include \"vpack.h\"\n\n"
    test += "#define restrict __restrict\n"
    test += "#ifdef DATA_TYPE\n"
    test += "  #undef DATA_TYPE\n"
    test += "#endif\n"
    test += "#define DATA_TYPE %s\n\n" % data_type
    test += "void %s_vpack(double *restrict A, double *restrict x, double *restrict y) {\n" % d[n]
    test += "#pragma macveth\n"
    for i in range(n):
        test += "\ty[%d] += A[%d] * x[%d];\n" % (y_idx[i], A_idx[i], x_idx[i])
    test += "#pragma endmacveth\n"
    test += "}\n"
    return test


def gen_all_combinations(n_elems, data_type, start_range=0, length=32, prefix_file="vpack"):
    y_idx = [5] * n_elems
    A_idx = range(4, 4 + n_elems)
    for i in it.combinations(range(start_range, start_range + length), n_elems):
        x_idx = list(i)
        f = gen_test(data_type, n_elems, y_idx, A_idx, x_idx)
        s = 0
        for x in x_idx:
            s |= (1 << x)
        suffix = f"{s:0{length}b}"


def clean_files():
    os.system("rm -Rf ")


os.mkdir(bin_path)
os.mkdir(asm_path)
os.mkdir(test_path)
os.mkdir(results_path)

# for n_elems in range(2, 5):
gen_all_combinations(4, "double")
