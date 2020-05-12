#include <iostream>
#include "bst.hpp"

int main() {
  try{
    using tree = bst<int, double>;
    using couple = std::pair<const int, double>;
    //testing
    tree b(1,19);
    b.insert({2,2});
    std::cout << b;
    std::vector<couple> v{ {2, 3.4}, {1, 6}, {7, 4.332}, {12, 1}, {3, 5}, {14, 1.22} };
    tree a{v};
    std::cout << a;





  }
  catch(const std::exception e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }
  catch(...) {
    std::cerr << "Unknown exception. Aborting... \n";
    return 2;
  }
}
