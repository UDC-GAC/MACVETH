#!/env/python3

import os

path = ":workspace/MACVETH/tests/perf_test"

archs = {
    "broadwell": "marcos.horro@pluton.des.udc.es",
    "cascadelake": "marcos.horro@pluton.des.udc.es",
    "zen": " horro@193.144.50.143"
}

for arch in archs.keys():
    d = arch + "_results"
    os.system("rm -r %s" % d)
    os.system("mkdir -p %s" % d)
    full_path = archs[arch] + path
    os.system("scp -r %s %s" % (full_path, d))
