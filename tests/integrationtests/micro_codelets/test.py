#!/env/python3

import os
import sys
for i in range(int(sys.argv[1])):
    os.system(f"python3 inline_codelet.py patterns/codelet_{i:08d}.c")
