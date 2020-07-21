#!/env/python3

import os
import sys
import pandas as pd
import seaborn as sns
from tqdm import tqdm

tolerance = 0.10
executions = 7

bench = "alexnet"
args = ""

if (len(sys.argv) < 3):
    print("Usage: python3 time_benchmark.py <executable> <output>")

bench = sys.argv[1]
output = sys.argv[2]


def calculate_mean():
    df_arr = []
    for exe in range(executions):
        df = pd.read_csv("___tmp_file_%s" % exe)
        df_arr += [df]
    name = df_arr[0]['name']
    features = df_arr[1]['features']

    s = pd.concat(df_arr).groupby(level=0).std() / \
        pd.concat(df_arr).groupby(level=0).mean()
    print(s)
    for r in s['time']:
        if (r > tolerance):
            print("[ERROR] Deviation too high = " + str(r))

    m = pd.concat(df_arr).groupby(level=0).mean()
    m['name'] = name
    m['features'] = features
    m['gflops'] = m.flops/m.time/1e9

    m.to_csv(output)
    os.system("rm ___tmp_file_*")


pbar = tqdm(total=executions)
for exe in range(executions):
    pbar.update(0.5)
    os.system("./%s > ___tmp_file_%s" % (bench, str(exe)))
    pbar.update(0.5)
pbar.close()

calculate_mean()
