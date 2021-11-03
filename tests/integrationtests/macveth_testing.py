#!/usr/bin/env python3
# -*- coding: utf-8 -*-

# Some path declarations
import os
import sys
import filecmp
import re
import subprocess

# Some path declaration
cwd = os.getcwd()
env_pwd = os.environ["PWD"]
offset_path_mv = ""
if cwd != env_pwd:
    # we are executing from somewhere else..
    offset_path_mv = "../tests/integrationtests/"
build_path = "../../build/"
macveth_bin = f"{build_path}macveth"
macveth_path = "macveth_dir/"
fail_path = "failed_tests/"
pass_path = "passed_tests/"
tmp_path = f"tmpfiles/"
test_report = "test_report.txt"
log_file = "macveth_compiler.log"

# File extensions
file_t = ".c"
comp_suffix = f"_macveth{file_t}"


class MVError(Exception):
    pass


# Compiler options: for testing the outputs obtained
def get_gcc_binary():
    cc = "gcc"
    gcc_version = (
        os.popen(f"{cc} --version").read().split("\n")[0].split(" ")[-1].split(".")[0]
    )

    if gcc_version == "" or int(gcc_version) < 9:
        cc = ""
        for i in range(9, 12):
            tmp = f"gcc-{i}"
            if os.popen(f"{tmp} --version").read() != "":
                cc = tmp

    if cc == "":
        print("Needed gcc >= 9")
        exit(1)
    print(f"gcc version: {gcc_version}")
    return cc


import sys

cc = get_gcc_binary()
# Clang tool takes as the relative path from where the CMake does. I hate this
# behavior; will have a look at this sometime someday
# mv_poly_flags = "
# --extra-arg-before='-I/home/markoshorro/workspace/MACVETH/tests/utilities'
# --misa=avx2 "
mv_poly_flags = ["--misa=avx2", "--no-format"]
mv_poly_flags_end = ["--", f"-I{cwd}/utilities"]


def poly_flags(p):
    flags = [
        "-O3",
        "-mavx2",
        f"-I{p}",
        "-I",
        "utilities",
        "utilities/polybench.c",
        "-DPOLYBENCH_DUMP_ARRAYS",
    ]
    return flags


def compile_cmd(flags, file, output):
    # Get the compilation line
    args = [f"{cc}", *flags, f"{file}", "-o", f"{output}"]
    tmp = subprocess.run(args, stderr=subprocess.PIPE).stderr
    for line in tmp.decode("utf-8").split("\n"):
        print(line)
        if "error" in line:
            raise MVError


def run_test(test, output):
    with open(output, "w") as f:
        tmp = subprocess.run([f"./{test}"], stderr=subprocess.PIPE).stderr
        f.write(tmp.decode("utf-8"))


def compile_test_with_macveth(org_file, out_file, args=[]):
    # Compiling the tests
    args = [*args, f"{org_file}", f"-o={offset_path_mv}/{out_file}", *mv_poly_flags_end]
    tmp = subprocess.run(
        [f"./{macveth_bin}", *args], shell=False, stderr=subprocess.PIPE
    ).stderr
    for line in tmp.decode("utf-8").split("\n"):
        print(line)
        if "MACVETH FATAL ERROR" in line or "LLVM ERROR" in line or "failed" in line:
            raise MVError


def test_output(exp_out, comp_out, code=False):
    # Aggressive comparison between expected output and compiled version: files
    # must be exactly the same. Maybe we should be less aggressive when it comes
    # to compare code
    # @return True if same values, False otherwise
    assert os.stat(exp_out).st_size != 0
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


def execute_test(path_suite, test, compiler_flags=["-mavx2", "-mfma"]):
    """Run test

    Args:
        test (str): Name of the test without the extension
    """

    if not os.path.isdir("tmpfiles/"):
        os.mkdir("tmpfiles")

    macveth_t = tmp_path + path_suite.replace("/", "") + "_" + test + comp_suffix
    org_t = path_suite + test + file_t

    print("{:<80}".format("execution test of " + org_t + "..."), end="")
    try:
        # Compile with macveth the test
        compile_test_with_macveth(org_t, macveth_t, mv_poly_flags)
        pref = f"{tmp_path}{test}"

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
    except MVError as err:
        print(f"error while compiling: {err}")
        sys.exit(1)

    if test_output(tmp_org_out, tmp_mv_out):
        if not os.path.isdir(pass_path):
            os.mkdir(pass_path)
        # os.system(f"mv {macveth_t} {pass_path}")
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
