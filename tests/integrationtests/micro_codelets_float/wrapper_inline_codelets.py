#!/env/python3

import os
import sys

for i in range(int(sys.argv[1]), int(sys.argv[2])):
    os.system(f"python3 inline_codelet.py patterns/codelet_{i:08d}.c")
