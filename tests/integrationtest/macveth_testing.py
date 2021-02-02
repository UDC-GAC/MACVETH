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
unittest_path = "unittest/"
fulltest_path = "fulltest/"
mustpass_path = "mustpass/"
macveth_path = "macveth_dir/"
fail_path = "failed_tests/"
tmp_path = "tmpfiles/"
test_report = "test_report.txt"

# File extensions
file_t = ".c"
exp_sufix = "_exp" + file_t
comp_sufix = "_comp" + file_t

# Compiler options: for testing the outputs obtained
cc = "gcc"

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
        " -I%s -I utilities utilities/polybench.c -DPOLYBENCH_DUMP_ARRAYS" %
        (p))


def comp_cmd(flags, file, output):
    # Get the compilation line
    # print("(cd %s && %s %s %s -o ../%s)" % (path, cc, flags, file, output))
    os.system(f"{cc} {flags} {file} -o {output} ")


def run_test(test, output):
    os.system(f"./{test} 2> {output}")


def compile_test_with_macveth(org_file, out_file, args=" "):
    # Compiling the tests
    # print("DEBUG: macveth %s %s -o=%s 2>> macveth_compiler.log" %
    #      (args, org_file, out_file))
    os.system(
        f"{macveth_bin} {args} {org_file} -o={out_file} {mv_poly_flags_end} 2>> macveth_compiler.log")
    # For some reason, sometimes there is a bad descriptor error when compiling...
    line = subprocess.check_output(['tail', '-1', "macveth_compiler.log"])
    if "LLVM ERROR" in str(line):
        return False
    return True


def test_output(exp_out, comp_out, code=False):
    # Aggressive comparison between exepected output and compiled version: files
    # must be exactly the same. Maybe we should be less aggressive when it comes
    # to compare code
    # @return True if same values, False otherwise
    if (code):
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
    macveth_t = macveth_path + test + comp_sufix
    org_t = path_suite + test + file_t

    # Compile with macveth the test
    n_attemps = 3
    while (n_attemps > 0):
        if compile_test_with_macveth(org_t, macveth_t, mv_poly_flags):
            break
        n_attemps -= 1
    if (n_attemps == 0):
        print("error...")
        exit(1)
    print("{:<80}".format("execution test of " + org_t + "..."), end="")

    pref = tmp_path + test

    # Compile and run the original
    tmp_org_bin = f"{pref}_org.bin"
    tmp_org_out = f"{pref}_org.output"
    comp_cmd(poly_flags(path_suite), org_t, tmp_org_bin)
    run_test(tmp_org_bin, tmp_org_out)

    # Compile and run macveth's version
    tmp_mv_bin = f"{pref}_mv.bin"
    tmp_mv_out = f"{pref}_mv.output"
    comp_cmd(poly_flags(path_suite) +
             compiler_flags, macveth_t, tmp_mv_bin)
    run_test(tmp_mv_bin, tmp_mv_out)

    if (test_output(tmp_org_out,
                    tmp_mv_out)):
        print("OK")
    else:
        print("ERROR in test")
        sys.exit(1)


# Parse arguments
if (len(sys.argv) != 3):
    print("Wrong number of arguments")
    sys.exit(1)

# Run test
path_suite = sys.argv[1]
test_name = sys.argv[2][:-2]
execute_test(path_suite, test_name)
