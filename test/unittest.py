#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# File              : unittest.py
# Author            : Marcos Horro <marcos.horro@udc.gal>
# Date              : Mér 15 Xan 2020 15:33:25 MST
# Last Modified Date: Mér 15 Xan 2020 21:58:58 MST
# Last Modified By  : Marcos Horro <marcos.horro@udc.gal>

import os
import filecmp

# Some path declarations
build_path = "../build"
unittest_path = "unittest/"
fulltest_path = "fulltest/"
macveth_path = "macveth/
fail_path = "failed_tests/"
tmp_path = "tmpfiles/"

os.system("mkdir -p %s" % fail_path)
os.system("mkdir -p %s" % tmp_path)
os.system("mkdir -p %s" % macveth_path)

# File extensions
file_t = ".c"
exp_sufix = "_exp" + file_t
comp_sufix = "_comp" + file_t
# Compiler options: for testing the outputs obtained
cc = "gcc"
poly_flags = " -I utilities utilities/polybench.c -DPOLYBENCH_DUMP_ARRAYS "
# FIXME:
# Clang tool takes as the relative path from where the CMake does. I hate this
# behavior; will have a look at this sometime someday
mv_poly_flags = " --extra-arg-before='-Itest/fulltest/utilities' "

# Discussion:
# is POLYBENCH_USE_C99_PROTO needed?
# Should not, we do not want the compiler to vectorize anything as we are
# already emitting SIMD code, right?


def comp_cmd(path, cc, flags, file, output):
    # Get the compilation line
    # print("(cd %s && %s %s %s -o ../%s)" % (path, cc, flags, file, output))
    os.system("(cd %s && %s %s %s -o ../%s)" % (path, cc, flags, file, tmp_path
                                                + output))


def run_test(path, test, output):
    os.system("./%s 2> %s" % (tmp_path + test, tmp_path + output))


def compile_macveth():
    # Compiling MACVETH
    os.system("make -C %s " % build_path)
    os.system("cp %s/macveth ." % build_path)
    return True


def compile_test(p, org_file, out_file, args=""):
    # Compiling the tests
    os.system("./macveth %s %s -o=%s" %
              (args, (p + org_file), (p + out_file)))
    # FIXME more than urgently
    # HAHA please remove this ASAP, please
    os.system("sed -i '1i#include <immintrin.h>' %s" % p + out_file)
    return True


def test_output(path, exp_out, comp_out, code=False):
    # Aggressive comparison between exepected output and compiled version: files
    # must be exactly the same. Maybe we should be less aggressive when it comes
    # to compare code
    # @return True if same values, False otherwise
    return filecmp.cmp(path + exp_out, path + comp_out)


def print_results(name, passed, failed, tests):
    # Printing results obtained
    print("results for %s" % name)
    print("\tpassed tests = %3d / %3d" % (len(passed), len(tests)))
    print("\tfailed tests = %3d / %3d" % (len(failed), len(tests)))

    if (len(failed) == 0):
        print("\tGood work!")
        exit(0)
    elif (len(failed) == 1):
        print("\tAlmost...")

    print("\tfailed tests:")
    print("\t=============")
    for fail in failed:
        os.system("cp %s %s" % (name+fail, fail_path))
        print("\t\t" + fail)

    for tpass in passed:
        # If the test passes, we do not want to have a copy of it because is the
        # same as the expected value, therefore redundant. Be clean
        os.system("rm %s" % (name + tpass))


# Compiling macveth
compile_macveth()

########################
# Unittests
failed_tests = []
passed_tests = []

# Parsing all the available tests in unittest
# Create list with all the unit test
tests_name = []
for r, d, f in os.walk(unittest_path):
    for file in f:
        # We are only interested in those tests which have a expected output,
        # otherwise we are not able to compare to anything
        if exp_sufix in file:
            tests_name.append(file.split(exp_sufix)[0])

# Compiling the original codes
for test in tests_name:
    comp_test = test + comp_sufix
    exp_test = test + exp_sufix
    compile_test(unittest_path, test + file_t, test + comp_sufix)
    if (test_output(unittest_path, exp_test, comp_test)):
        passed_tests.append(comp_test)
    else:
        failed_tests.append(comp_test)

# Print results obtained
print_results("unittest", passed_tests, failed_tests, tests_name)

########################
# Fulltests
failed_tests = []
passed_tests = []

# Parsing all the available tests in unittest
# Create list with all the unit test
tests_name = []
for r, d, f in os.walk(fulltest_path):
    for file in f:
        if ("test" and file_t) in file:
            tests_name.append(file.split(".")[0])

# Compiling the original codes
for test in tests_name:
    # File names
    macveth_t = test + comp_sufix
    org_t = test + file_t

    # Compile with macveth the test
    compile_test(fulltest_path, org_t, macveth_t, mv_poly_flags)

    # Compile and run the original
    tmp_org_bin = test + "_org.bin"
    tmp_org_out = test + "_org.output"
    comp_cmd(fulltest_path, cc, poly_flags, org_t, test + "_org")
    run_test(fulltest_path, tmp_org_bin, tmp_org_out)

    # Compile and run macveth's version
    tmp_mv_bin = test + "_org.bin"
    tmp_mv_out = test + "_org.output"
    comp_cmd(fulltest_path, cc, poly_flags +
             " -mavx2 -mfma ", macveth_t, tmp_mv_bin)
    run_test(fulltest_path, tmp_mv_bin, tmp_mv_out)

    if (test_output(tmp_path, tmp_org_out,
                    tmp_mv_out)):
        passed_tests.append(macveth_t)
    else:
        failed_tests.append(macveth_t)


# Print results obtained
print_results("fulltest", passed_tests, failed_tests, tests_name)
