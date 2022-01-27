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


import glob

bugs = glob.glob("bug_*.code")

with open("code_header.c", "r") as f:
    header = f.readlines()

with open("code_footer.c", "r") as f:
    footer = f.readlines()

for bug in bugs:
    file_name = bug[:-3]
    print(f"File: {bug} to {file_name}")
    lines = []
    with open(bug, "r") as f:
        lines = f.readlines()

    lines = header + lines + footer
    with open(file_name, "w") as write_file:
        write_file.writelines(lines)
