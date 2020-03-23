![MACVETH](https://github.com/markoshorro/MACVETH/blob/develop/doc/report/img/MACVETHLOGO.svg)

# MACVETH
## Multi-dimensional Array C-compiler VEctorization and Translation for HPC applications

Copyright (c) 2019-2020 the Colorado State Univesity.

Contact:
    Marcos Horro Varela <marcos.horro@udc.es>
    Louis-Noël Pouchet <pouchet@colostate.edu>

MACVETH is a source-to-source compiler from C/C++ codes to SIMD. It is platform
or ISA and architecture dependent.

MACVETH stands for Multi-Array C-compiler for VEctorization and Translation for
High-performance computing. Besides, Macbeth, tragedy written by William
Shakespeare, represents the detrimental effects of human's narcissism and vanity
when looking for the power for its own benefits. In contraposition, MACVETH is
composed of many intermediate representations seeking for a common purpose:
optmize vectorization.
Last but not least, the first and last letter of the acronym stand for the name
of the main author (Marcos Horro).

## Main dependencies:
- Clang >=10.0.0
- CMake >=3.13
- GNU GCC >=8.3.0
- Doxygen >=1.8.13

## Getting started:

Simple guide to get started with the compiler.

### Compilation of the project:

Recommended steps for a clean building of the project:

$> mkdir build
$> CXX=[C++ compiler] CC=[C compiler] cmake -G "Unix Makefiles" ..
$> make

This will create an executable in the same folder called 'macveth'

### Executing:

User may execute the compiler just typing:

`$> ./macveth <input_file.c>`

This will generate a macro-fashion SIMD code (if possible) onto a file named
macveth_output.c/cpp.

For displaying all the available options, type:

`$> ./macveth --help`

## CLI Options:

In this section we will comment in detail all the options available:

    --debug: Print debug comments, from TAC creation to the backend generator
    --debug-file=<file>: Print debug output to file

    --input-cdag=<file>: Input file with placements for the CDAG created as IR

    --macro-free: The backend will generate intrinsics and will not use the
                  Vector API for generating SIMD code. This flag is needed right
                  now because the VectorAPI is not implemented yet.

    --march=<arch>: Target architecture (not implemented yet)
    --misa=<isa>: Target ISA (only AVX2 right now)
    --fma: FMA support in the architecture

    -o=<output>: Output file, otherwise macveth_output.c
    -p=<path>: Build path

## Region-of-interest in our code:

The compiler recognizes the region-of-interest (ROI from now on) by the
delimitation of the pragmas:

    #pragma macveth [options]
    // code
    #pragma endmacveth

Available options are:

    - nounroll: avoid unrolling the code within. This may be useful if we have a
      irregular code and we just want to vectorize it

Pragmas to be implemented (soon):

    - unroll_factor [val1] [[val2] [...]]: explicitly tell the compiler the unroll
      factor of each dimension of the nested loop/s
    - hw_alignment [array_name1] [val1] [[array_name2] [val2] [...]]: hint
      regarding the alignment factor for an array, e.g. A 64 means that we can
      can assume that A[0] % 64 = 0. Per array option.
    - tc {>|>=|=|<|<=} [val]: hint regarding the trip count of the loop, i.e.
      the expected number of iterations. Per loop option.

* Limitations/restrictions and assumptions:
-------------------------------------------

This the list of restrictions and assumptions that the user must take into
account when using this compiler:

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
          in order to be detected properly
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
          statement. Besides, steps must be uniary, i.e.
        + One #pragma per set of loop nests, e.g.:

            This is correct:

            #pragma macveth
            for (int i = 0; i < N; ++i) {
            }
            #pragma endmacveth
            #pragma macveth
            for (int i = 0; i < N; ++i) {
                for (int j = 0; j < M; ++j) {
                    ...
                }
            }
            #pragma endmacveth

            This is not:

            #pragma macveth
            for (int i = 0; i < N; ++i) {
            }
            for (int i = 0; i < N; ++i) {
                for (int j = 0; j < M; ++j) {
                    ...
                }
            }
            #pragma endmacveth

## Versioning

The version number of this compiler follows the Semantic Versioning
Specification (https://semver.org/spec/v2.0.0.html)

## License

(C) Copyright 2019-2020 the Colorado State University.

No warranties. Under development, code disclosed under request.
