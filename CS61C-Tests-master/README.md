Extra Tests for CS61C
=====================

Copy (not recommended) or symlink (recommended) the testing files in each
directory to your project or homework folder, then follow the instructions below
to run the tests:


Project 2-1
-----------

Replace the provided Makefile with the Makefile in this repository. Extra tests
can be run with the following command:

```
make extradisasmtest
```

Tests are in `mipscode/test-disasm.s`, and the output to compare to is in
`mipscode/test-disasm.cmp`.

Project 2-2
-----------

Extra tests are in `mipscode/tests.s`. It should print "All tests passed",
otherwise it will print the number of the failed test. These tests are
incremental, which means every test requires all previous tests to pass.

To run, make sure you have the updated `Makefile` in your proj2 folder:

```
make runtest
```
