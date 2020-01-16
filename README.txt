
MACVETH - Multi-dimensional Array C-compiler VEctorization and Transformation
for HPC applications

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

User may execute the compiler just

$> ./macveth <input_file.c>

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
        - Currently only working for AVX2 ISA architectures
    - Loop related:
        + Only for loops are supported
        + Initialization of variables within the loop is allowed
        + There are no increments in the body of the loop of the region of
          interest
    - Statement related:
    - Known issues:



