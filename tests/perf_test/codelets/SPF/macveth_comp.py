#!/env/python3

import os

# bench = {"GL7d10-d1compr.spf": "GL7d10.spf",
#          "GD98_a-d1compr": "",
#          "S80PI_n1-d1compr.spf": "",
#          "S80PI_n1-d1compr-leftover.spf": "",
#          "S80PI_n1-d1g4.ast": ""}
bench = {
    # "GL7d10-d1compr.spf": "GL7d10.spf",
    # "GD98_a-d1compr.spf": "GD98_a.spf",
    "S80PI_n1-d1g4.ast": "S80PI_n1.rb",
}
flags = "--simd-cost-model=unlimited"
dyn_libs = "LD_LIBRARY_PATH=/home/markoshorro/anaconda3/lib/:$LD_LIBRARY_PATH"

for b in bench:
    # compile with macveth
    os.system("macveth %s.c %s -o %s.mv.c" % (b, flags, b))

    # makefile
    os.system("make %s" % b)
    os.system("make %s.mv" % b)

    print("%s ./%s %s" % (dyn_libs, b, bench[b]))
    os.system("%s ./%s %s" % (dyn_libs, b, bench[b]))
    print("%s ./%s.mv %s" % (dyn_libs, b, bench[b]))
    os.system("%s ./%s.mv %s" % (dyn_libs, b, bench[b]))

    # execute and store result
    #os.system("make exec")
