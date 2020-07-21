#!/env/python3

import os
import sys
import pandas as pd
from tqdm import tqdm

tolerance = 0.10
n_exec = 7

bench = "alexnet"
args = ""

if (len(sys.argv) < 5):
    print("Usage: python3 time_benchmark.py <executable> <compilation_opts> <output> <macveth>")
    exit(0)

bench = sys.argv[1]
comp_opts = sys.argv[2]
output = sys.argv[3]
macveth = int(sys.argv[4])

tmp_file = "___tmp_file_"

# README:
# Files must produce an output with the following format:
# name,features,flops,time,gflops


def calculate_mean(bench, executions):
    bench_dir = bench.split("/")[0]
    os.chdir("%s" % bench_dir)
    bench = bench.split("/")[1]
    print("Executing %s times %s:" % (str(executions), bench))
    pbar = tqdm(total=executions)
    for exe in range(executions):
        pbar.update(0.5)
        os.system("./%s > %s%s" % (bench, tmp_file, str(exe)))
        pbar.update(0.5)
    pbar.close()
    df_arr = []
    for exe in range(executions):
        df = pd.read_csv("___tmp_file_%s" % exe)
        df_arr += [df]
    name = df_arr[0]['name']
    features = df_arr[1]['features']

    s = pd.concat(df_arr).groupby(level=0).std() / \
        pd.concat(df_arr).groupby(level=0).mean()
    for r in s['time']:
        if (r > tolerance):
            print("[WARNING] Deviation too high = " + str(r))

    m = pd.concat(df_arr).groupby(level=0).mean()
    m['name'] = name + "_" + features
    m['gflops'] = m.flops/m.time/1e9
    m['compilation'] = comp_opts
    m['macveth'] = macveth
    # be clean
    os.system("rm %s*" % (tmp_file))
    os.chdir("..")
    # write output
    m.to_csv(output)


# main
calculate_mean(bench, n_exec)
