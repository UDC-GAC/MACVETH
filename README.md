<p align="center">
  <img src="https://github.com/markoshorro/MACVETH/blob/develop/doc/report/img/MACVETHLOGO.svg">
</p>

# MACVETH - Multi-dimensional Array C-compiler for VEctorizing Tensors in HPC

[![Build Status][travis-badge]][travis-link]
[![codecov](https://codecov.io/gh/markoshorro/MACVETH/branch/master/graph/badge.svg?token=BRvGSUAVby)](https://codecov.io/gh/markoshorro/MACVETH)
[![License: MIT][mit-badge]][mit-link]

Main authors (refer to [License](#license) section for further details):

- Marcos Horro Varela (marcos.horro@udc.es)
- Dr. Louis-Noël Pouchet (pouchet@colostate.edu)

MACVETH is a source-to-source compiler for C/C++ codes to compilable
SIMD-fashion codes. It is platform or ISA and architecture dependent.

MACVETH stands for Multi-Array C-compiler for VEctorizating Tensors in HPC
apps. Besides, Macbeth, tragedy written by William Shakespeare, represents the
detrimental effects of human's narcissism and vanity when looking for the power
for its own benefits. In contraposition, MACVETH is composed of many
intermediate representations seeking for a common purpose: optimize vectorization.
Last but not least, the first and last letter of the acronym stand for the name
of the main author (Marcos Horro).

Regarding the logo, the symbol within the ellipse is the 23rd letter of ancient
greek's alphabet Psi. It represents the cost function for our SIMD model.
This letter is widely used in science (e.g. Schrödinger equations).
Besides, 'psi' can be transliterated to 'ps', which is the suffix on Intel
Intrinsics for 'packed-single'. Same way, the shape of Psi greek letter has
three fleurons, as the Three Witches in the very first Act of the tragedy (or
is this too twisted?).

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
position (e.g. shared environment without superuser privileges) ensure that you
install, at least, `clang` and `clang-tools-extra`.

Be sure to have LLVM/Clang's libraries in `LIBRARY_PATH` (Linux)

## Getting started:

Simple guide to get started with the compiler.

### Compilation of the project:

Recommended steps for a clean building of the project (Linux):

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

### Executing:

User may execute the compiler just typing:

`$> macveth <input_file.c>`

This will generate a macro-fashion SIMD code (if possible) onto a file named
macveth_output.c/cpp.

For displaying all the available options, type:

`$> macveth --help`

If the program has includes, they can be specified using `--extra-arg-before`
Clang's option in order to [not break the parsing](#compilation-database), e.g. we have `bar.cpp` such as:

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
e.g.

`$> macveth --extra-arg-before='-I/path/to/mylib' bar.cpp`

It can be also done using `--` after `macveth` command, as we later explain in
[Fixed Compilation Database](#fixed-compilation-database), e.g.

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

    --debug-mv: Print debug comments, from TAC creation to the backend generator
    --debug-file=<file>: Print debug output to file

    --func=<string>: target function to vectorize. Useful for large documents with many functions
    in order to avoid the overhead of analyzing all LOCs in a program.

    --input-cdag=<file>: Input file with placements for the CDAG created as IR

    --march=<arch>: Target architecture (not implemented yet)
    --misa=<isa>: Target ISA (only AVX2 right now)
    --fma: FMA support in the architecture
    --nofma: do not generate FMA instructions even if they are available in the architecture
    --no-headers: do not print headers in output file
    --no-svml: disable SVML Intel library (math functions, basically)

    --simd-cost-model=[conservative|aggressive|unlimited]: different algorithms selected
    by the compiler to decide whether to vectorize or not a concrete region.

    --simd-info=<file>: report with all the SIMD information retrieved
    --simd-info-missed=<file>: report with all the SIMD opportunities missed due to SIMD cost model or other issues

    -o=<output>: Output file, otherwise macveth_output.c
    -p=<path>: Build path

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

Pragmas to be implemented (soon):

    - hw_alignment [array_name1] [val1] [[array_name2] [val2] [...]]: hint
    regarding the alignment factor for an array, e.g. A 64 means that we can
    assume that A[0] % 64 = 0. Per array option.

    - tc {>|>=|=|<|<=} [val]: hint regarding the trip count of the loop, i.e.
      the expected number of iterations. Per loop option.

    - li val1 val2 [val21 val22 [...]]: loop interchange, interchanges the val1
        with val2, and so on.

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

#### JSON compilation database:

## SIMD Cost model

We have described in [§CLI Options](#cli-options) the `--simd-cost-model`
option. In all compilers using modules such as auto-vectorizers there is a step
where vectorization cost is computed in order to evaluate the benefit.

In MACVETH, cost model is constructed by using instructions characteristics of each
architecture and ISA. For x86-64 architectures, Intel and AMD basically,
vectorization is done by replacing code for Intel Intrinsics functions.
ASM instruction's latency, throughput and port usage are obtained using tables
available at [uops.info](http://uops.info/table.html) [1]. Nonetheless, this
table does not provide a direct 1:1 translation onto Intel Intrinsics. For
this purpose, we use the `XED_iform`. This format is just a representation of
instructions and their operands in a _unequivocally form_ (discussion
[here](https://github.com/andreas-abel/XED-to-XML#instruction-variants-vs-xed-iforms)).
As this ''form'' is available in uops.info, we can pair each ASM
instruction to its Intel Intrinsic equivalent using [public web
data](https://software.intel.com/sites/landingpage/IntrinsicsGuide/). Notice
that not all Intel Intrinsics are translated onto a unique ASM instruction,
e.g. `vps = _mm_set_ps(v3,v2,v1,v0)`, where each `vX` is a `float` and `vps` a
`__m128` register, in this case code generated by compiler differs
significantly depending on the address values of the
operands, thus if operands are contiguous in memory compiler may generate a
unique ASM instruction such as `movaps xmm, addr[v0]`, or four different data
movements (`movss`) if those operands are sparsed in memory. Hence, in our
model, we have considered always this last worst case, by modeling those `set`
intrinsics with the same number of data movements as operands.

Basically, any cost model should satisfy the following inequation:

<!-- $$
\sum{V} \lt \sum{S} \Rightarrow vectorize
$$ -->

<div align="center"><img src="https://render.githubusercontent.com/render/math?math=%5Csum%7BV%7D%20%5Clt%20%5Csum%7BS%7D%20%5CRightarrow%20vectorize"></div>

Where <!-- $V$ --> <img src="https://render.githubusercontent.com/render/math?math=V"> is the set of vector instructions and <!-- $S$ --> <img src="https://render.githubusercontent.com/render/math?math=S"> the equivalent set of
sequential or scalar instructions. In our approach, our model is agnostic
regarding instructions ''already in the pipeline'', so we do not consider
throughput or port usage in first place. Comparison is done at VectorIR level:
for a set of packable nodes it is generated the vector operation and,
therefore, its SIMD equivalent (architecture dependent); cost of these SIMD
instructions (<!-- $I$ --> <img src="https://render.githubusercontent.com/render/math?math=I">) should be lower than the scalar cost of the nodes (<!-- $N$ --> <img src="https://render.githubusercontent.com/render/math?math=N">).

<!-- $$
\sum{I} \lt \sum{N} /\exists f:{I,N} \mapsto \mathbb{Z}^{+}
$$ -->

<div align="center"><img src="https://render.githubusercontent.com/render/math?math=%5Csum%7BI%7D%20%5Clt%20%5Csum%7BN%7D%20%2F%5Cexists%20f%3A%7BI%2CN%7D%20%5Cmapsto%20%5Cmathbb%7BZ%7D%5E%7B%2B%7D"></div>

Where <!-- $f$ --> <img src="https://render.githubusercontent.com/render/math?math=f"> is a function that given either a set of SIMD instructions (<!-- $I$ --> <img src="https://render.githubusercontent.com/render/math?math=I">) or a
set of nodes (<!-- $N$ --> <img src="https://render.githubusercontent.com/render/math?math=N">) returns a cost integer positive value, according to the
latency table for that architecture and ISA.

Complexity and increasing intelligence of compilers and their auto-vectorizers
make impossible to create an accurate SIMD cost model. Nonetheless, providing
accurate latency information improves the quality of SIMD-zed code.

## Limitations/restrictions and assumptions:

This the list of restrictions and assumptions that the user must take into
account when using this compiler:

    - Input languages:
        + C/C++ files *only* have been tested.

    - ISA support:
        + Currently only working for AVX2 ISA architectures

    - Types:
        + Pointers are not allowed at the moment, e.g.:

            (*S) = (*S) + A[i];

            This may be a common scenario when writing reductions in the code
            (when passing variables to the function by reference). You can simply
            avoid this issue by using a temporal variable.

        + Reductions should look like:

            S = S + <expr> || S += <expr>

            in order to be detected properly

    - Declarations within scop:
        + Not permitted any kind or type of declaration within, e.g. int var = 42;
        + Assignments are permitted, e.g. var = 42;

    - Statements:
        + Must be binary, i.e. all statements should be contained in the following
        grammar, in Backus-Naur Form:

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
        interest, i.e. the loop condition is only incremented in the for
        statement.

## Testing

Refer to `tests/README`.

## Software development

- The version number of this compiler follows the Semantic Versioning
  Specification (https://semver.org/spec/v2.0.0.html)
- Continuous integration using TravisCI
- Code format using Clang-format (LLVM-style, of course)

## License

MIT License.

Copyright (c) 2019-2020 the Colorado State University.
Copyright (c) 2020 Universidade da Coruña.

Under development, code disclosed only under request.

[travis-badge]: https://travis-ci.com/markoshorro/MACVETH.svg?token=NvjC6fzdrgxL3SFrS5bJ&branch=develop
[travis-link]: https://travis-ci.org/markoshorro/MACVETH
[codecov-badge]: https://codecov.io/gh/markoshorro/MACVETH/branch/develop/graph/badge.svg
[codecov-link]: https://codecov.io/gh/markoshorro/MACVETH
[mit-badge]: https://img.shields.io/badge/License-MIT-yellow.svg
[mit-link]: https://opensource.org/licenses/MIT
[macveth-logo]: https://github.com/markoshorro/MACVETH/blob/develop/doc/report/img/MACVETHLOGO.svg
