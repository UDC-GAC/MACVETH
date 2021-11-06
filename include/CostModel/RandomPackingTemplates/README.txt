Random Packing Templates
------------------------
.mrt extension stands for Macveth Random Templates. The naming of each file is
described as follows:
    <arch>_<isa>_<data_type>_n<nnz>_<contiguity_form>_<version>

Where:
    <arch>            ::= architecture, e.g. cascadelake, skylake, zen3, etc.
    <isa>             ::= SIMD ISA supported, e.g. sse, avx, avx2, avx512f
    <data_type>       ::= float | double | int
    <contiguity_form> ::= (_[01]){nnz - 1}

Their syntax is quite simple:

        line ::= <expr>
        <expr> ::= <reg> = <inst> | <inst>
        <inst> ::= signature(<args>)
        <args> ::= <reg> | <mem> | *args
