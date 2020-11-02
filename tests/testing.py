'''
Filename: tests/testing.py
Created Date: Friday, October 9th 2020, 4:53:05 pm
Author: Marcos Horro

Copyright (c) 2020 Colorado State University
'''

#!/usr/bin/env python3
# -*- coding: utf-8 -*-


# Some path declarations
import os
import filecmp
import re
import subprocess
from utils.utilities import pr_col
from utils.utilities import colors as c

# Some path declarations
build_path = "../build"
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
mv_poly_flags = " --extra-arg-before='-I/home/markoshorro/workspace/MACVETH/tests/utilities' --misa=avx2 "

# Discussion:
# is POLYBENCH_USE_C99_PROTO needed?
# Should not, we do not want the compiler to vectorize anything as we are
# already emitting SIMD code, right?


def clean_previous_files():
    os.system("rm -Rf %s" % tmp_path)
    os.system("rm -Rf %s" % macveth_path)
    os.system("rm -Rf %s" % fail_path)
    # Init directories
    os.system("mkdir -p %s" % fail_path)
    os.system("mkdir -p %s" % tmp_path)
    os.system("mkdir -p %s" % macveth_path)
    os.system("echo "" > %s" % test_report)


def clean_tmp_files():
    os.system("rm -Rf %s" % tmp_path)


def poly_flags(p):
    return (
        " -I%s -I utilities utilities/polybench.c -DPOLYBENCH_DUMP_ARRAYS" %
        (p))


def comp_cmd(flags, file, output):
    # Get the compilation line
    # print("(cd %s && %s %s %s -o ../%s)" % (path, cc, flags, file, output))
    os.system("%s %s %s -o %s " % (cc, flags, file,
                                   output))


def run_test(test, output):
    os.system("./%s 2> %s" % (test, output))


def compile_macveth():
    # Compiling MACVETH
    out = os.system("make -j8 -C %s " % build_path)
    if (out != 0):
        print("Something went wrong compiling MACVETH! Exiting...")
        exit(0)
    return True


def compile_test_with_macveth(org_file, out_file, args=" "):
    # Compiling the tests
    # print("DEBUG: macveth %s %s -o=%s 2>> macveth_compiler.log" %
    #      (args, org_file, out_file))
    os.system("macveth %s %s -o=%s 2>> macveth_compiler.log" %
              (args, org_file, out_file))
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


def print_results(name, passed, failed, tests):
    # Printing results obtained
    with open(test_report, "a") as f:
        f.write("=============\n")
        f.write("results for %s\n" % name)
        f.write("\tpassed tests = %3d / %3d\n" % (len(passed), len(tests)))
        f.write("\tfailed tests = %3d / %3d\n" % (len(failed), len(tests)))

        if (len(failed) == 0) and (len(tests) > 0):
            f.write("\tGood work!\n")

        f.write("\tpassed tests:\n")
        f.write("\t=============\n")
        for tpass in passed:
            # If the test passes, we do not want to have a copy of it because is the
            # same as the expected value, therefore redundant. Be clean
            f.write("\t\t%s\n" % tpass)

        if (len(failed) == 0):
            return
        f.write("\tfailed tests:\n")
        f.write("\t=============\n")
        for fail in failed:
            os.system("cp %s %s\n" % (fail, fail_path))
            f.write("\t\t%s\n" % fail)


def unittest_suite():
    #########################
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
        org_test = unittest_path + test + file_t
        exp_test = unittest_path + test + exp_sufix
        comp_test = macveth_path + test + comp_sufix
        compile_test_with_macveth(org_test, comp_test)
        print("{:<80}".format("execution test of " + org_test + "..."), end="")
        if (test_output(exp_test, comp_test, True)):
            passed_tests.append(comp_test)
            pr_col(c.fg.green, " [PASS]")
        else:
            failed_tests.append(comp_test)
            pr_col(c.fg.red, " [FAIL]")

    # Print results obtained
    print_results("unittest", passed_tests, failed_tests, tests_name)


def exectest_suite(path_suite, kword="test", compiler_flags=" -mavx2 -mfma "):
    ########################
    # Fulltests
    failed_tests = []
    passed_tests = []

    # Parsing all the available tests in unittest
    # Create list with all the unit test
    tests_name = []
    for r, d, f in os.walk(path_suite):
        for file in f:
            if (kword) in file:
                tests_name.append(file.split(".")[0])

    # Compiling the original codes
    for test in tests_name:
        # File names
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
            exit(-1)
        print("{:<80}".format("execution test of " + org_t + "..."), end="")

        pref = tmp_path + test

        # Compile and run the original
        tmp_org_bin = pref + "_org.bin"
        tmp_org_out = pref + "_org.output"
        comp_cmd(poly_flags(path_suite), org_t, tmp_org_bin)
        run_test(tmp_org_bin, tmp_org_out)

        # Compile and run macveth's version
        tmp_mv_bin = pref + "_mv.bin"
        tmp_mv_out = pref + "_mv.output"
        comp_cmd(poly_flags(path_suite) +
                 compiler_flags, macveth_t, tmp_mv_bin)
        run_test(tmp_mv_bin, tmp_mv_out)

        if (test_output(tmp_org_out,
                        tmp_mv_out)):
            passed_tests.append(macveth_t)
            pr_col(c.fg.green, " [PASS]")
        else:
            failed_tests.append(macveth_t)
            pr_col(c.fg.red, " [FAIL]")

    # Write results obtained
    print_results(path_suite, passed_tests, failed_tests, tests_name)


# Clean everything
clean_previous_files()

# Compiling macveth
compile_macveth()

# Fulltest
exectest_suite(fulltest_path)

# "MUST PASS" tests
exectest_suite(mustpass_path, "must")

# Print results
os.system("cat %s" % test_report)

clean_tmp_files()
