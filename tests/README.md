# MACVETH Testing

This document comments some features of the testing suite for the MACVETH
compiler.

## Main dependencies:

In order to execute manually the test suite you must have installed, at least:

- Python >=3.6.0

## Getting started:

In order to execute the suite:

```
$> python3 testing.py [options]
```

Main options available:

    --all|-a: execute all tests (default)
    --help|-h: show help message
    --full|-f: execute "fulltests" (description below)
    --must|-m: execute "mustpass" (description below)

- Terminology:

---

Some terms which will be used in this document:

    * Case: certain configuration or setup of a program to evaluate. It can be
      composed of more than one file

## Description:

Refer to `TESTS_INFO.md` file in order to read more details about the tests.

## How to write new tests:

There are two main sets: unittest and fulltest. Then, there is another
"optional" suite meant to describe the cases that compiler _MUST_ pass. Its name
is self-descriptive: "mustpass"

## Fulltest

If you want to assess the correctness of a certain code being produced, you will
want to write the code in 'fulltest'. You will just have to write a file:

    * Original code: you will use a template created for this, using the
      PolyBench-style kernels. This way is easier to compare the outputs

## Mustpass

As its name suggests, this set of tests describe cases that the compiler must
not fail. It follows the same procedure as the "Fulltest"s: compare the output
of the scalar and SIMD code. Therefore, the only difference is the directory
where these tests are.

## Unittest

You will want to add a new test for testing the shape of the code itself by
creating a new case in 'unittest'

    * Original code: for being compiled by macveth
    * SIMD code: the expected output to be compared to macveth's output. Must
      end with the suffix "_exp.c" in order to the script to recognize it.

This way, the testing script will compile the original code and generate a SIMD
version of it and will compare it against the "\_exp.c" version, ignoring any
withespace, newline, tab or comment in the code.
