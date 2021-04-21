#!/env/python3

import sys


if len(sys.argv) != 2:
    print("Needed 1 argument")
    sys.exit(1)

input_args = sys.argv[1].split(" ")
codelet_file = ""
if len(input_args) == 1:
    codelet_file = input_args[0]
else:
    for arg in input_args:
        if "MICRO_CODELET_FILE_NAME" in arg:
            codelet_file = arg.split("=")[1].replace("\\\"", "")

micro_codelet_template = "micro_codelet.c"
output_file = "micro_" + codelet_file.split("/")[-1]

inline_code = ""
with open(codelet_file) as f:
    inline_code = f.readlines()

if inline_code == "":
    print("Error: codelet not found")
    sys.exit(1)

with open(output_file, "w") as out:
    with open(micro_codelet_template) as template:
        for line in template:
            if line != "#include MICRO_CODELET_FILE_NAME\n":
                out.write(line)
            else:
                out.writelines(inline_code)
sys.exit(0)
