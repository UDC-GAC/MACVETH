# Reduction PASS
Clang/LLVM reduction pass is the original name of this repo. The former idea was
to implement a LLVM pass to perform a source-to-source translation of C/C++
codes.

---

## Installation
The `Makefile` in the root directory is self explicative, however you will have
to modify, at least, the `LLVM_SRC_PATH` and `LLVM_BUILD_PATH` variables. Then,
just use:

```
make
```
---

## Tests
There are some test in `test/` directory. Execute `make test` in root directory.

---

## Versions
* `clang version 9.0.1-svn374858-1~exp1~20191015044111.61 (branches/release_90)`
* `g++ (Ubuntu 8.3.0-6ubuntu1) 8.3.0`

