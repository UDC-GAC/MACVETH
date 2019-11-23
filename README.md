# MACVETH
This compiler attemps to vectorize properly loops by performing a source-to-source translation from standard C/C++ to C/Intrinsics. It uses Clang for AST parsing and, therefore, finding regions of interest (ROI). 

MACVETH stands for Multi-Array C-compiler for VEctorization and Translation for High-performance computing. Besides, Macbeth, tradegy written by William Shakespeare, represents the detrimental effects of the narcisism and vanity when looking for the power for its own benefits. In contraposition, MACVETH is composed of many components seeking for a common purpose: optimize vectorization. Last but not least, the first and last letter of the acronym stand for the name of the main author (Marcos Horro).

---

## Installation
Building system used for this project is `CMake`. There is no additional configuration needed so:

```
cmake {ROOT_PROJECT}
make
```

---

## Tests
There are some test in `test/` directory. Execute `make test` in root directory.

---

## Versions
* `clang version 9.0.1-svn374858-1~exp1~20191015044111.61 (branches/release_90)`
* `g++ (Ubuntu 8.3.0-6ubuntu1) 8.3.0`
* `cmake >=3.13`

