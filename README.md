# Advanced_Programming_exam

In this project I have implemented a binary search tree through the use of three different classes, **Node**, **_iterator** and **bst**.

**Structure:** 
* Include folder: contains the headers with the implementation of the three classes.
  * node.hpp contains the implementation of Node struct.
  * iterator.hpp contains the implementation of _iterator class.
  * bst.hpp contains the bst class and the declarations of all member functions, as well as the definition of the not too complicated ones.
  * bst.tpp contains the definition of the remaining functions.
* main.cpp is the source file used to test and verify the correctness of the above-mentioned functions and classes.
* Makefile produces the executables main.x and benchmark.x.
* Report.pdf is a general report about the bst implementation.
* Benchmark.cpp is used to test the efficiency of the implementation against std::map
