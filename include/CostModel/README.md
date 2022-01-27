Random Packing Model format:
----------------------------
    CSV file containing the following columns:

        * ID:
        * SIGNATURE: refer to 
        * VERSION:

Signature format:
-----------------
    In extended REGEX:

        n([1-8])(_[0,1])+

        * \1: number of nnz elements
        * \2: contiguity. 1 if two elements a and b are contiguous, 0 otherwise.
