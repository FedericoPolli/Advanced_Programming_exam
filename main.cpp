#include <iostream>
#include <string>
#include "bst.hpp"

int main() {
  try{
    using tree = bst<int, double>;
    using couple = std::pair<const int, double>;
    
    //testing

    bst<int, double, std::greater<int>> r_tree(2,2);  //custom ctor with different order
    r_tree.emplace(3,3);
    r_tree.insert({1,1});
    std::cout << "\nReverse tree:\n";
    std::cout << r_tree;
    
    std::vector<couple> v{ {2, 3.4}, {1, 6}, {7, 4.332}, {12, 1}, {3, 5}, {14, 1.22}, {0, -2}, {10, 0.003}, {5, 2}, {8, -0.55}, {4, 3.4} };
    tree A{v};
    std::cout << "\nTree A initialised from vector, not balanced:\n";
    std::cout << A;
    A.balance();
    std::cout << "\nA after balancing:\n";
    std::cout << A;
    tree B=A;      //copy assignment
    std::cout << "\nB is a copy of A:\n";
    std::cout << "B: " << B;
    A.clear();
    std::cout << "\nA after clearing:\n";
    std::cout << A;
    std::cout << "\nB after clearing A:\n";
    std::cout << B;
    tree C{std::move(B)};    //move ctor
    std::cout << "\nC after moving B:\n";
    std::cout << C;
    std::cout << "\nB after moving:\n";
    std::cout << B;
    
    C.insert({17, 2});
    C.insert(std::make_pair(25, 6.3));
    C.emplace(32, 2.3);
    C.emplace(21, -12.34);
    C[23] = 3.2;
    std::cout << "\nC after adding nodes:\n" << C;
    C.erase(1);
    std::cout << "\nErasing a leaf node ( in this case (1,6) ) doesn't rebalance the tree\n";
    std::cout << C;
    C.erase(5);
    std::cout << "\nErasing a node with children ( in this case (5,2) ) rebalances the tree \n";
    std::cout << C;
    
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
