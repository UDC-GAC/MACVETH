# Copyright 2021 Marcos Horro
#
# Licensed under the Apache License, Version 2.0 (the "License");
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

header = ""
with open("header.c", "r") as f:
    header = f.readlines()

footer = ""
with open("footer.c", "r") as f:
    footer = f.readlines()

for i in range(2, 9):
    for c in range(2 ** (i - 1)):
        output_file = "randompacking_"
        body = ["\n#pragma macveth\n"]
        output_file += f"n{i}_"
        values = [0] * i
        contiguity = list(map(lambda x: int(x), list(f"{c:0{i-1}b}"))) if i > 1 else []
        rev_contiguity = contiguity[::-1]
        for v in range(1, len(contiguity) + 1):
            offset = 1 if rev_contiguity[v - 1] == 1 else 2
            values[v] = values[v - 1] + offset

        output_file += "_".join(list(map(lambda x: str(x), contiguity)))

        for n in range(i):
            body.append(f"  tmp += x[{values[n]}];\n")
        body.append("#pragma endmacveth\n")
        lines = header + body + footer
        with open(f"{output_file}.c", "w") as f:
            f.writelines(lines)

