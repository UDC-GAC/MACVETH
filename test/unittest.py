#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# File              : unittest.py
# Author            : Marcos Horro <marcos.horro@udc.gal>
# Date              : Mér 15 Xan 2020 15:33:25 MST
# Last Modified Date: Mér 15 Xan 2020 21:58:58 MST
# Last Modified By  : Marcos Horro <marcos.horro@udc.gal>

import os

# Some declarations
build_path = "../build"
unittest_path = "unittest"
ft = ".c"
exp_sufix = "_exp.c"
mv_prefix = "mv_"

# Compiling the compiler: assume that cmake has been already called


def compile_macveth():
    # Compiling MACVETH
    os.system("make -C %s " % build_path)
    os.system("cp %s/macveth ." % build_path)

    return True


# Compiling the org_file and print it in the out_file
def compile_test(org_file, out_file):

    return True

# Compare codes: if True, then there are differences between those two files, if
# False then files are equal


def test_simd_code(expected, compiled):
    return False


failed_tests = []
passed_tests = []


# Parsing all the available tests in unittest
# Create list with all the unit test
tests_name = []
for r, d, f in os.walk(unittest_path):
    for file in f:
        if exp_sufix in file:
            tests_name.append(file.split(exp_sufix)[0])

compile_macveth()
for test in tests_name:
    compile_test(test+ft, test + exp_sufix)

for test in tests_name:
    comp_test = mv_prefix + test
    compile_macveth(test, comp_test)
    if (test_simd_code(test, comp_test)):
        failed_tests.append(comp_test)
    else:
        passed_tests.append(comp_test)

# Printing results
print("passed tests = %3d / %3d" % (len(passed_tests), len(tests_name)))
print("failed tests = %3d / %3d" % (len(failed_tests), len(tests_name)))

if (len(failed_tests) == 0):
    print("\tGood work!")
    exit(0)
elif (len(failed_tests) == 1):
    print("\tAlmost...")

print("failed tests:")
print("=============")
for fail in failed_tests:
    print("\t" + fail)
