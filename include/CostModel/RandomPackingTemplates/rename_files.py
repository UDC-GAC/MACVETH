# Copyright 2021 Marcos Horro
#
# Licensed under the Apache License, Version 2.0 (the "License")
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

import sys
import glob
import os

archs = ["cascadelake", "skylake", "zen", "zen2", "zen3", "broadwell", "haswell"]
isas = ["avx2", "avx", "sse", "avx512f"]
data_type = ["float", "double"]


def check_naming(name):
    s = name.split("_")
    if s[0] not in archs:
        return False
    if s[1] not in isas:
        return False
    if s[2] not in data_type:
        return False
    if s[3][0] != "n":
        return False
    return True


if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Need two arguments")
        exit(1)
    arch = sys.argv[1]
    if arch not in archs:
        print(f"Bad architecture: {arch}")
        exit(1)
    isa = sys.argv[2]
    if isa not in isas:
        print(f"Bad ISA: {isa}")
        exit(1)
    keep = True
    if len(sys.argv) == 4:
        keep = bool(int(sys.argv[3]))
    for file in glob.glob("*.mrt"):
        if not check_naming(file):
            # then if has form rvp_<data>_<n>elems_<n>nnz_<cont>_<version>.mrt
            datatype = file.split("_")[1]
            nelems = file.split("_")[3][0]
            ending = file.split("nnz_")[-1]
            new_name = f"{arch}_{isa}_{datatype}_n{nelems}_{ending}"
            if keep:
                os.system(f"cp {file} {new_name}")
            else:
                os.system(f"mv {file} {new_name}")
