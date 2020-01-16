#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# File              : unittest.py
# Author            : Marcos Horro <marcos.horro@udc.gal>
# Date              : Mér 15 Xan 2020 15:33:25 MST
# Last Modified Date: Mér 15 Xan 2020 21:58:58 MST
# Last Modified By  : Marcos Horro <marcos.horro@udc.gal>

import os
import filecmp

# Some declarations
build_path = "../build"
unittest_path = "unittest"
ft = ".c"
exp_sufix = "_exp.c"
comp_sufix = "_comp.c"


def compile_macveth():
    # Compiling MACVETH
    os.system("make -C %s " % build_path)
    os.system("cp %s/macveth ." % build_path)
    return True


def compile_test(org_file, out_file):
    # Compiling the tests
    os.system("./macveth %s/%s -o=%s/%s" %
              (unittest_path, org_file, unittest_path, out_file))
    return True


def test_simd_code(exp_out, comp_out):
    # Aggressive comparison between exepected output and compiled version: files
    # must be exactly the same. Maybe we should be less aggressive
    return filecmp.cmp(exp_out, comp_out)


def test_output(exp_out, comp_out):
    return False


def print_results(passed, failed, tests):
    # Printing results
    print("passed tests = %3d / %3d" % (len(passed), len(tests)))
    print("failed tests = %3d / %3d" % (len(failed), len(tests)))

    if (len(failed) == 0):
        print("\tGood work!")
        exit(0)
    elif (len(failed) == 1):
        print("\tAlmost...")

    print("failed tests:")
    print("=============")
    for fail in failed:
        print("\t" + fail)


failed_tests = []
passed_tests = []

# Parsing all the available tests in unittest
# Create list with all the unit test
tests_name = []
for r, d, f in os.walk(unittest_path):
    for file in f:
        if exp_sufix in file:
            tests_name.append(file.split(exp_sufix)[0])

# Compiling macveth
compile_macveth()

# Compiling the original codes
for test in tests_name:
    comp_test = test + comp_sufix
    compile_test(test + ft, test + comp_sufix)
    if (test_simd_code(test, comp_test)):
        failed_tests.append(comp_test)
    else:
        passed_tests.append(comp_test)

print_results(passed_tests, failed_tests, tests_name)
