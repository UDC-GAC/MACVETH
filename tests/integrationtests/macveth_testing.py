#!/usr/bin/env python3
# -*- coding: utf-8 -*-

# Some path declarations
import os
import sys
import filecmp
import re
import subprocess

# Some path declarations
build_path = "../../build/"
macveth_bin = f"{build_path}macveth"
macveth_path = "macveth_dir/"
fail_path = "failed_tests/"
pass_path = "passed_tests/"
tmp_path = "tmpfiles/"
test_report = "test_report.txt"
log_file = "macveth_compiler.log"
# File extensions
file_t = ".c"
comp_suffix = f"_macveth{file_t}"

# Compiler options: for testing the outputs obtained
cc = "gcc"
gcc_version = os.popen(f"{cc} --version").read().split("\n")[0].split(" ")[-1].split(".")[0]

if gcc_version == "" or int(gcc_version) < 9:
    cc = ""
    for i in range(9,12):
        tmp = f"gcc-{i}"
        if os.popen(f"{tmp} --version").read() != "":
            cc = tmp

if cc=="":
    print("Needed gcc >= 9")
    exit(1)

print(f"gcc version: {gcc_version}")

# Clang tool takes as the relative path from where the CMake does. I hate this
# behavior; will have a look at this sometime someday
# mv_poly_flags = "
# --extra-arg-before='-I/home/markoshorro/workspace/MACVETH/tests/utilities'
# --misa=avx2 "
mv_poly_flags = " --misa=avx2 "
cwd = os.getcwd()
mv_poly_flags_end = f" -- -I{cwd}/utilities"


def poly_flags(p):
    return (
        f" -O3 -mavx2 -I{p} -I utilities utilities/polybench.c -DPOLYBENCH_DUMP_ARRAYS"
    )


def compile_cmd(flags, file, output):
    # Get the compilation line
    os.system(f"{cc} {flags} {file} -o {output} ")


def run_test(test, output):
    os.system(f"./{test} 2> {output}")


def compile_test_with_macveth(org_file, out_file, args=" "):
    # Compiling the tests
    os.system(
        f"{macveth_bin} {args} {org_file} -o={out_file} {mv_poly_flags_end} 2>> {log_file}"
    )
    # For some reason, sometimes there is a bad descriptor error when compiling...
    line = subprocess.check_output(["tail", "-1", log_file])
    if "LLVM ERROR" in str(line):
        return False
    return True


def test_output(exp_out, comp_out, code=False):
    # Aggressive comparison between expected output and compiled version: files
    # must be exactly the same. Maybe we should be less aggressive when it comes
    # to compare code
    # @return True if same values, False otherwise
    if code:
        with open(exp_out) as f1, open(comp_out) as f2:
            s1 = ""
            for l in f1:
                l = re.sub("//.*", "", l)
                s1 += l.replace(" ", "").replace("\n", "").replace("\t", "")
            s2 = ""
            for l in f2:
                l = re.sub("//.*", "", l)
                s2 += l.replace(" ", "").replace("\n", "").replace("\t", "")
        return s1 == s2
    return filecmp.cmp(exp_out, comp_out)


def execute_test(path_suite, test, compiler_flags=" -mavx2 -mfma "):
    """Run test

    Args:
        test (str): Name of the test without the extension
    """

    if not os.path.isdir(tmp_path):
        os.mkdir(tmp_path)

    macveth_t = tmp_path + path_suite.replace("/", "") + "_" + test + comp_suffix
    org_t = path_suite + test + file_t

    # Compile with macveth the test
    n_attemps = 3
    while n_attemps > 0:
        if compile_test_with_macveth(org_t, macveth_t, mv_poly_flags):
            break
        n_attemps -= 1
    if n_attemps == 0:
        print("error...")
        exit(1)
    print("{:<80}".format("execution test of " + org_t + "..."), end="")

    pref = tmp_path + test

    # Compile and run the original
    tmp_org_bin = f"{pref}_org.bin"
    tmp_org_out = f"{pref}_org.output"
    compile_cmd(poly_flags(path_suite), org_t, tmp_org_bin)
    run_test(tmp_org_bin, tmp_org_out)

    # Compile and run macveth's version
    tmp_mv_bin = f"{pref}_mv.bin"
    tmp_mv_out = f"{pref}_mv.output"
    compile_cmd(poly_flags(path_suite) + compiler_flags, macveth_t, tmp_mv_bin)
    run_test(tmp_mv_bin, tmp_mv_out)

    if test_output(tmp_org_out, tmp_mv_out):
        if not os.path.isdir(pass_path):
            os.mkdir(pass_path)
        os.system(f"mv {macveth_t} {pass_path}")
        print("OK")
    else:
        print("ERROR in test")
        if not os.path.isdir(fail_path):
            os.mkdir(fail_path)
        os.system(f"mv {macveth_t} {fail_path}")
        os.system(f"cp {org_t} {fail_path}")
        os.system(f"mv {tmp_mv_out} {tmp_org_out} {fail_path}")
        sys.exit(1)


# Parse arguments
if len(sys.argv) != 3:
    print("Wrong number of arguments")
    sys.exit(1)

# Run test
path_suite = sys.argv[1]
test_name = sys.argv[2][:-2]
execute_test(path_suite, test_name)
