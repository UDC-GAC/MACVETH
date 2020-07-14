![MACVETH](https://github.com/markoshorro/MACVETH/blob/develop/doc/report/img/MACVETHLOGO.svg)

# MACVETH - Multi-dimensional Array C-compiler for VEctorizing Tensors in HPC 

[![Build Status][travis-badge]][travis-link]
[![codecov][codecov-badge]][codecov-link]

Copyright (c) 2019-2020 the Colorado State University.

Marcos Horro Varela (marcos.horro@udc.es)

Dr. Louis-Noël Pouchet (pouchet@colostate.edu)

MACVETH is a source-to-source compiler from C/C++ codes to SIMD. It is platform
or ISA and architecture dependent.

MACVETH stands for Multi-Array C-compiler for VEctorizating Tensors in HPC
apps. Besides, Macbeth, tragedy written by William Shakespeare, represents the 
detrimental effects of human's narcissism and vanity when looking for the power
for its own benefits. In contraposition, MACVETH is composed of many 
intermediate representations seeking for a common purpose: optmize vectorization.
Last but not least, the first and last letter of the acronym stand for the name
of the main author (Marcos Horro).

Regarding the logo, the image represents nodes in the CDAG structure that it
uses. Same way, it also creates the shape of a crown, as Macbeth's, having
three fleurons, as the Three Witches in the very first Act of the tragedy.

## Main dependencies:

* Clang     >= 10.0.0
* CMake     >= 3.12
* GNU/GCC   >= 8.3.0
* Doxygen   >= 1.8.13

## Getting started:

Simple guide to get started with the compiler.

### Compilation of the project:

Recommended steps for a clean building of the project:

``` 
$> mkdir -p build && cd build
$> CXX=[C++ compiler] CC=[C compiler] cmake -G "Unix Makefiles" ..
$> make -j4 && sudo ln -sf $PWD/macveth /usr/local/bin/macveth
```

This will create an executable in the same folder called 'macveth', and also
will create a symbolic link to its executable. 

### Executing:

User may execute the compiler just typing:

`$> ./macveth <input_file.c>` 

This will generate a macro-fashion SIMD code (if possible) onto a file named
macveth_output.c/cpp.

For displaying all the available options, type:

`$> ./macveth --help` 

## Documentation

MACVETH is documented using Doxygen. Configuration file can be found in `doc/`
directory. Besides, in this folder there are additional resources for
understanding the intermediate representations created and used in this compiler.


## CLI Options:

In this section we will comment in detail all the options available:

    --debug: Print debug comments, from TAC creation to the backend generator
    --debug-file=<file>: Print debug output to file

    --input-cdag=<file>: Input file with placements for the CDAG created as IR

    --macro-code: TODO yet, generate code using a macro fashion code, not ad-hoc 
        intrinsics code.

    --march=<arch>: Target architecture (not implemented yet)
    --misa=<isa>: Target ISA (only AVX2 right now)
    --fma: FMA support in the architecture
    --nofma: do not generate FMA instructions even if they are available in the architecture

    -o=<output>: Output file, otherwise macveth_output.c
    -p=<path>: Build path

## Region-of-interest in our code:

The compiler recognizes the region-of-interest (ROI from now on) by the
delimitation of the pragmas:

    #pragma macveth [options]
    // code
    #pragma endmacveth

Available options for each scop are:

    - nounroll: avoid unrolling the code within. This may be useful if we have 
    a irregular code and we just want to vectorize it.

    - unroll_factor [val1] [[val2] [...]]: explicitly tell the compiler the 
    unroll factor of each dimension of the nested loop/s.

    - unrollandjam: perform unroll-and-jam in the loops within the scop.

    - nosimd: do not generate SIMD code; useful for only unrolling code.

Pragmas to be implemented (soon):

    - hw_alignment [array_name1] [val1] [[array_name2] [val2] [...]]: hint
    regarding the alignment factor for an array, e.g. A 64 means that we can
    assume that A[0] % 64 = 0. Per array option.

    - tc {>|>=|=|<|<=} [val]: hint regarding the trip count of the loop, i.e.
      the expected number of iterations. Per loop option.

    - li val1 val2 [val21 val22 [...]]: loop interchange, interchanges the val1
        with val2, and so on.

## Limitations/restrictions and assumptions:

This the list of restrictions and assumptions that the user must take into
account when using this compiler:

    - Languages:
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

            S = S + <expr>
          
          in order to be detected properly (FIXME)

    - Declarations within scop:
        + Not permitted any kind or type of declaration within, e.g. int var = 42;
        + Assignments are permitted, e.g. var = 42;

    - Statements:
        + Must be binary, i.e. all statements should be contained in the following 
        grammar, in a Backus-Naur Form fashion:
            
            Syntax ::= <Statement>

            Statement ::= (non-terminal)
                | <expr> = <S>

            S ::= (non-terminal)
                | for(<expr> = <expr>; <expr> < <expr>; <expr>) { 
                    <S>; }
                | <S> op <S>
                | f(<S>,<S>)
                | f(<S>)
                | <expr>

            op ::= (terminal)
                | +,-,/,*,%

            expr ::= (terminal)
                | array
                | literal
                | var

        "op" may be a function (with two arguments) or a binary operation. This 
        limitation imposes that either functions and operations must be binary.

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

## Versioning

The version number of this compiler follows the Semantic Versioning
Specification (https://semver.org/spec/v2.0.0.html)

## License

(C) Copyright 2019-2020 the Colorado State University.

No warranties. Under development, code disclosed under request.

[travis-badge]:    https://travis-ci.com/markoshorro/MACVETH.svg?token=NvjC6fzdrgxL3SFrS5bJ&branch=develop
[travis-link]:     https://travis-ci.org/markoshorro/MACVETH
[codecov-badge]:   https://codecov.io/gh/markoshorro/MACVETH/branch/develop/graph/badge.svg
[codecov-link]:    https://codecov.io/gh/markoshorro/MACVETH
