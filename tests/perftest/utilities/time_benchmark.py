#!/env/python3

import os
import sys
import pandas as pd
from tqdm import tqdm

max_tolerance = 0.15  # yeah yeah i know this is too high
n_exec = 7
tmp_file = "___tmp_file_"


# README:
# Files must produce an output with the following format:
# name,features,flops,[time,gflops|counter0,counter1...]
# depending on compilation with TIME or PAPI counters


def calculate_mean(bench, comp_opts, output, macveth, t, executions, tolerance=0.05):
    bench_dir = bench.split("/")[0]
    os.chdir("%s" % bench_dir)
    bench = bench.split("/")[1]
    print("Executing %s times %s:" % (str(executions), bench))
    pbar = tqdm(total=executions)
    for exe in range(executions):
        pbar.update(0.25)
        os.system("./%s > %s%s" % (bench, tmp_file, str(exe)))
        pbar.update(0.75)
    pbar.close()
    df_arr = []
    for exe in range(executions):
        df = pd.read_csv("%s%s" % (tmp_file, exe))
        df_arr += [df]
    name = df_arr[0]['name']
    features = df_arr[1]['features']

    if (t == "TIME"):
        s = pd.concat(df_arr).groupby(level=0).std() / \
            pd.concat(df_arr).groupby(level=0).mean()
        for r in s['time']:
            if (r > tolerance):
                print("[WARNING] Deviation too high = " + str(r))

    m = pd.concat(df_arr).groupby(level=0).mean()
    if (t == "TIME"):
        m['gflops'] = m.flops/m.time/1e9
    m['name'] = name + "_" + [str(i) for i in features]
    m['compilation'] = comp_opts
    m['macveth'] = macveth
    # be clean
    os.system("rm %s*" % (tmp_file))
    os.chdir("..")
    # write output
    m.to_csv(output)


if __name__ == "__main__":
    bench = "alexnet"
    args = ""

    if (len(sys.argv) < 6):
        print("Usage: python3 time_benchmark.py <executable> <compilation_opts> <output> <macveth> <type> <nexec>")
        exit(0)

    bench = sys.argv[1]
    comp_opts = sys.argv[2]
    output = sys.argv[3]
    macveth = int(sys.argv[4])
    t = sys.argv[5]
    if (len(sys.argv) == 7):
        n_exec = int(sys.argv[6])

    calculate_mean(bench, comp_opts, output, macveth, t, n_exec, max_tolerance)
