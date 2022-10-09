<p align="center">
  <img src="https://github.com/UDC-GAC/MACVETH/blob/develop/docs/report/img/MACVETHLOGO.svg">
</p>

# MACVETH - Multi-dimensional Array C-compiler for VEctorizing Tensors in HPC
[![CMake Compilation and Test](https://github.com/markoshorro/MACVETH/actions/workflows/cmake.yml/badge.svg)](https://github.com/markoshorro/MACVETH/actions/workflows/cmake.yml)
[![codecov](https://codecov.io/gh/markoshorro/MACVETH/branch/master/graph/badge.svg?token=BRvGSUAVby)](https://codecov.io/gh/markoshorro/MACVETH)
[![License: Apache License, Version 2.0][apache-badge]][apache-link]

MACVETH is a source-to-source compiler for C/C++ codes to compilable
SIMD-fashion codes. It is platform or ISA and architecture dependent.

MACVETH stands for Multi-Array C-compiler for VEctorizing Tensors in HPC
applications. Besides, Macbeth, tragedy written by William Shakespeare,
represents the detrimental effects of human's narcissism and vanity when looking
for the power for its own benefits. In contraposition, MACVETH is composed of
many intermediate representations seeking for a common purpose: optimize
vectorization.

## Environments tested:

- Clang/LLVM >= 11
- CMake >= 3.12
- GNU/GCC >= 8.3.0
- Doxygen >= 1.8.13
- lcov >= 1.14

For Clang/LLVM sources are needed. If using Ubuntu/Debian, you can add them by
just installing `*-dev` versions.

### How to build and install LLVM/Clang:

Building LLVM/Clang from sources [can not be done in-tree](https://llvm.org/docs/CMake.html#quick-start). IMHO, I think it
is better for your health installing LLVM/Clang (or any other huge framework in
general) using repositories. Nonetheless, if you find yourself in that extraordinary
position (e.g., shared environment without superuser privileges) ensure that you
install, at least, `clang` and `clang-tools-extra`. Be also sure to have LLVM/Clang's libraries in `LIBRARY_PATH` (Linux).

## Getting started:

Simple guide to get started with the compiler.

### Compilation of the project:

Recommended steps for a clean building of the project (for Linux systems):

```
$> mkdir -p build && cd build
$> cmake -DCMAKE_C_COMPILER=[C compiler] -DCMAKE_CXX_COMPILER=[C++ compiler] -G "Unix Makefiles" ..
$> make -j8 && sudo ln -sf $PWD/macveth /usr/local/bin/macveth
```

Change `-j8` for a lower number if your system is not capable.
If LLVM is built out-of-tree, then you will need `-DLLVM_DIR=/path` in the
`cmake` command, as well as having `LIBRARY_PATH` and `LD_LIBRARY_PATH`
environment variables set with, at least, the path for LLVM/Clang's libraries.

These steps will create an executable in the same folder called `macveth`, and also
will create a symbolic link to its executable.

You have also an example script with these tasks in [`build.sh`](build.sh).

### Executing:

User may execute the compiler just typing:

`$> macveth <input_file.c>`

This will generate a macro-fashion SIMD code (if possible) onto a file named
`macveth_output.c/cpp`.

For displaying all the available options, please type:

`$> macveth --help`

If the program has includes, they can be specified using `--extra-arg-before`
Clang's option in order to [not break the parsing](#compilation-database), e.g., we have `bar.cpp` such as:

```
#include "mylib/foo.h"
void bar() {
#pragma macveth
    ...
#pragma endmacveth
}
```

Which includes `mylib/foo.h`. If this last one is not in the same folder as
`bar.cpp`, we will need to specify the path using the commented option above,
e.g.:

`$> macveth --extra-arg-before='-I/path/to/mylib' bar.cpp`

It can be also done using `--` after `macveth` command, as we later explain in
[Fixed Compilation Database](#fixed-compilation-database), e.g.:

`$> macveth bar.cpp -- -I/path/to/mylib`

## Documentation

MACVETH is documented using Doxygen. As the repository is not public, there is
no webpage to navigate through the code. When building the project, it is
generated a series of HTML with all code documentation; main page can be
located in `build/doxygen/html/index.html`. Configuration file for Doxygen can be found in `doc/`
directory.

Besides, in this folder there are additional resources for
understanding the intermediate representations created and used in this
compiler, also located in `doc/report`. This is meant to be a white paper so it
should be in constant evolution.

## CLI Options:

In this section we will comment in detail all the options available:

    --debug-file=<string>            - Output file to print the debug information
    --debug-mv                       - Print debug information
    --extra-arg=<string>             - Additional argument to append to the compiler command line
    --extra-arg-before=<string>      - Additional argument to prepend to the compiler command line
    --fma                            - Support for FMA instructions
    --format-fallback-style=<string> - The name of the predefined style used as a
                                       fallback in case clang-format is invoked with
                                       -format-style=file, but can not find the .clang-format
                                       file to use.
                                       Use -format-fallback-style=none to skip formatting.
    --format-style=<string>          - Coding style, currently supports:
                                         LLVM, GNU, Google, Chromium, Microsoft, Mozilla, WebKit.
                                       Use -format-style=file to load style configuration from
                                       .clang-format file located in one of the parent
                                       directories of the source file (or current
                                       directory for stdin).
                                       Use -format-style="{key: value, ...}" to set specific
                                       parameters, e.g.:
                                         -format-style="{BasedOnStyle: llvm, IndentWidth: 8}"
    --func=<string>                  - Target function
    --input-cdag=<string>            - Input file to read the custom CDAG placement
    --march=<value>                  - Target architecture
      =native                        -   Detect the architecture
      =nehalem                       -   Intel nehalem (2009) architecture (tock): SSE4.2
      =westmere                      -   Intel westmere (2010) architecture (tick): SSE4.2
      =sandybridge                   -   Intel sandybridge (2011) architecture (tock): AVX
      =ivybridge                     -   Intel ivybridge (2012) architecture (tick): AVX
      =haswell                       -   Intel haswell (2013) architecture (tock): AVX2
      =broadwell                     -   Intel broadwell (2014) architecture (tick): AVX2
      =skylake                       -   Intel skylake (2015) architecture (tock): AVX512
      =kabylake                      -   Intel Kaby Lake (2016) architecture (tock): AVX2
      =coffeelake                    -   Intel Coffee Lake (2017) architecture (tock): AVX2
      =cascadelake                   -   Intel Cascade Lake (2019) architecture (tock): AVX512
      =icelake                       -   Intel Ice Lake (2020) architecture (tick): AVX512
      =znver1                        -   AMD Zen1 (2019) architecture: AVX2
      =znver2                        -   AMD Zen2 (2020) architecture: AVX2
      =znver3                        -   AMD Zen3 (2020) architecture: AVX2
      =amd                           -   AMD architecture not specified
      =intel                         -   Intel architecture not specified
    --min-redux-size=<int>           - Advanced option: minimum number of reductions to pack together
    --misa=<value>                   - Target ISA
      =native                        -   Detect ISA of the architecture
      =sse                           -   SSE ISA
      =avx                           -   AVX ISA
      =avx2                          -   AVX2 ISA
      =avx512                        -   AVX512 ISA
    --no-format                      - MACVETH by defaults reformats code as clang-format does, using LLVM style. If this option is enabled, then no reformatting is applied
    --no-headers                     - If set, do *not* include header files, such as <immintrin.h>
    --no-svml                        - Disable Intrinsics SVML
    --nofma                          - Explicitly tell to not generate FMA instructions even if architecture supports them
    --nofuse                         - Disable the fusion of reductions
    --novec-orphan-redux             - Disable the vectorization of orphan reductions
    -o=<string>                      - Output file to write the code, otherwise it will just print int std output
    -p=<string>                      - Build path
    --redux-win-size=<int>           - Advanced option: size of window of reductions to consider
    --scatter                        - Support AVX512 scatter instructions
    --simd-cost-model=<value>        - SIMD cost model
      =conservative                  -   Vectorize if and only if the sequential estimation is worse than the vectorized
      =aggressive                    -   Vectorize partially if beneficial according to cost model
      =unlimited                     -   Unlimited SIMD cost, i.e., vectorize regardless the cost
    --simd-info=<string>             - Report with all the SIMD information
    --simd-info-missed=<string>      - Report with all missed SIMD opportunities
  

## Region-of-interest in our code:

The compiler recognizes the region-of-interest (ROI from now on) by the
delimitation of the pragmas:

    #pragma macveth [options]
    /*
        region-of-interest
    */
    #pragma endmacveth
    ...
    #pragma macveth [options]
    /*
        another region-of-interest
    */
    #pragma endmacveth

A program can hold different scops, even a function, but scops can never be nested.

Available options for scops are:

    - unroll [var0 val0] [[var1 val1] [...]]: explicitly tell the compiler the
    unroll factor of each dimension of the nested loop/s. val# can be either 'full'
    or a positive integer. This is the default option so it does not have to be explicit.

    - nounroll: avoid unrolling the code within. This may be useful if we have
    a irregular code and we just want to vectorize it. This has the same behavior as unrolling with factor 1.

    - unroll_and_jam: perform unroll-and-jam in the loops within the scop.

    - scalar/nosimd: do not generate SIMD code; useful, for instance, for only unrolling code.

## Troubleshooting

In this section we will describe how to tackle common errors when building and
using MACVETH.

### Compilation database

MACVETH is a clang-based tool. Thus, when clang generates de AST for analysis,
it actually parses and **compiles** the code. A common output error message we
could get be like:

```
Error while trying to load a compilation database:
Could not auto-detect compilation database for file "SOMEFILE"
No compilation database found in PATH or any parent directory
fixed-compilation-database: Error while opening fixed database: No such file or directory
json-compilation-database: Error while opening JSON database: No such file or directory
Running without flags.
```

This means that clang is missing a _compilation database_, but what is this? It
can be summarized as a collection of flags needed to compile the source/s
properly. It can be defined in two forms: fixed or as a JSON. Both are valid,
and any of them should get rid of this error. Next we are explaining briefly
both ways, but you could also take a look at [Eli Bendersky's
posts](https://eli.thegreenplace.net/2014/05/21/compilation-databases-for-clang-based-tools)
where he explains extraordinately these issues.

#### Fixed compilation database:

Specified using a `--` token after specifying the source and all compiler
options. After this token must follow any other flags or parameters needed to
compile properly the source code.

## Limitations/restrictions and assumptions:

This the list of restrictions and assumptions that the user must take into
account when using this compiler:

- Input languages:
    + C/C++ files *only* have been tested.

- ISA support:
    + Currently only working for AVX2 ISA architectures

- Types:
    + Pointers are not allowed at the moment, e.g.:

        ```
        (*S) = (*S) + A[i];
        ```
        This may be a common scenario when writing reductions in the code
        (when passing variables to the function by reference). You can simply
        avoid this issue by using a temporal variable.

    + Reductions are of the form:
        ```
        S = S + <expr> || S += <expr>
        ```
        in order to be detected properly.

- Declarations within scop:
    + Not permitted any kind or type of declaration within, e.g., `int var = 42;`
    + Assignments are permitted, e.g., `var = 42;`

- Statements:
    + Must be binary, i.e., all statements should be contained in the following
    grammar, in Backus-Naur Form:

    ```
        Syntax ::= <Statement>

        Statement ::=
            | <expr> = <S>

        S ::=
            | for(<expr> = <expr>; <expr> < <expr>; <expr>) {
                <S>; }
            | <S> binary_op <S>
            | f(<S>,<S>)
            | f(<S>)
            | <expr>

        binary_op ::= + | - | / | * | %

        expr ::= array | literal | var
    ```

- Loop related:
    + Only "for" loops are supported
    + Declaration of variables within the loop is allowed, e.g.:

            for (int i = 0; ...

        Also declaration outside the loop is allowed, e.g.:

            int i;
            ...
            for (i = 0; ...

        Nonetheless, some compilers may allow declarations in both sides,
        MACVETH does not, e.g.:

            int i;
            ...
            for (int i = 0; ...

    + There are no increments in the body of the loop of the region of
    interest, i.e., the loop condition is only incremented in the for
    statement.

## Testing

Refer to `tests/README`.

## Logo

The symbol within the ellipse is the 23rd letter of ancient
greek's alphabet Psi. It represents the cost function for our SIMD model.
This letter is widely used in science (e.g., Schrödinger equations).
Besides, 'psi' can be transliterated to 'ps', which is the suffix on Intel
Intrinsics for 'packed-single'. Same way, the shape of Psi greek letter has
three fleurons, as the Three Witches in the very first Act of the tragedy (or
is this too twisted?).

## Software development

- The version number of this compiler follows the Semantic Versioning
  Specification (https://semver.org/spec/v2.0.0.html)
- Continuous integration using GitHub actions.
- Code formatted using Clang-format (LLVM-style).

## License

List of [AUTHORS](AUTHORS).

Apache License.

Copyright (c) 2019-2022 the Colorado State University.
Copyright (c) 2020-2022 Universidade da Coruña.

[travis-badge]: https://travis-ci.com/markoshorro/MACVETH.svg?token=NvjC6fzdrgxL3SFrS5bJ&branch=develop
[travis-link]: https://travis-ci.org/markoshorro/MACVETH
[codecov-badge]: https://codecov.io/gh/markoshorro/MACVETH/branch/develop/graph/badge.svg
[codecov-link]: https://codecov.io/gh/markoshorro/MACVETH
[apache-badge]: https://img.shields.io/badge/license-Apache%202-blue
[apache-link]: https://opensource.org/licenses/Apache-2.0
[macveth-logo]: https://github.com/markoshorro/MACVETH/blob/develop/doc/report/img/MACVETHLOGO.svg
