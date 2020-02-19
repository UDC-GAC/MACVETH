
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*                                    MACVETH
*    Multi-dimensional Array C-compiler VEctorization and Translation for
*                              HPC applications
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

Contact:
    Marcos Horro Varela <marcos.horro@udc.es>
    Louis-Noël Pouchet <pouchet@colostate.edu>

MACVETH is a source-to-source compiler from C/C++ codes to SIMD. It is platform
dependent.

* Main dependencies:
--------------------
- Clang 10.0

* Getting started:
------------------

Simple guide to get started with the compiler.

** Compilation of the project:
------------------------------

Recommended steps for a clean building of the project:

$> mkdir build
$> CXX=[C++ compiler] CC=[C compiler] cmake -G "Unix Makefiles" ..
$> make

This will create an executable in the same folder called 'macveth'

** Executing:
------------

User may execute the compiler just typing:

$> ./macveth <input_file.c>

For displaying all the available options, type:

$> ./macveth --help

* CLI Options:
--------------

In this section we will comment in detail all the options available:

    --debug: Print debug comments, from TAC creation to the backend generator
    --debug-file=<file>: Print debug output to file
    --fma: FMA support in the architecture
    --input-cdag=<file>: Input file with placements for the CDAG
    --macro-free: The backend will generate intrinsics and will not use the
                  Vector API for generating SIMD code
    --march=<arch>: Target architecture
    --misa=<isa>: Target ISA
    -o=<output>: Output file, otherwise macveth_output.c
    -p=<path>: Build path

* Region-of-interest in our code:
----------------------------------

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


* License:
----------

Under development, code disclosed under request.

