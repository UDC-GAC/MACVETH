#!/env/python

import seaborn as sns
import pandas as pd
import sys
import os
from tqdm import tqdm

# Format of the CSV files
output_dir = "output/"

os.system("mkdir -p %s" % output_dir)

if (len(sys.argv) < 3):
    print(
        "Usage: python3 process_data.py <output> <file> [<file2> [<file3> ...]]")
    exit(0)

output_file = output_dir + sys.argv[1]
files = [pd.read_csv(f) for f in sys.argv[2:]]
df = pd.concat(files).reset_index()
df.drop("index", axis=1, inplace=True)
f = sns.catplot(data=df, x="name", y="gflops", hue="compilation")
f.savefig(output_file)
