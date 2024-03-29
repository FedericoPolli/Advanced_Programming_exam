#include <iostream>
#include <random>
#include <chrono>
#include <algorithm>
#include <vector>
#include "bst.hpp"
#include <map>

int main(int argc, char* argv[]) {
  
  if(argc!=2){
    std::cout << "\nTree size not given...\n";
    return 1;
  }

  std::size_t size = strtoul(argv[1], nullptr, 10);

  std::vector<int> v_int;
  std::vector<double> v_d;
  std::vector<std::pair<const int, double>> v;

  //initialise vectors with elements
  for (std::size_t i=0; i<size; i++) {
    v_int.push_back(i);
    v_d.push_back(i/double(size));
  }

  //shuffle 
  std::random_shuffle(v_int.begin(), v_int.end());
  std::random_shuffle(v_d.begin(), v_d.end());

  for (std::size_t i=0; i<size; i++) 
      v.push_back( {v_int[i], v_d[i]} );
  
  //initialise tree from vector (faster than inserting elements one by one)
  auto bt=std::chrono::steady_clock::now();
  bst<int, double> tree{v};
  tree.balance();
  auto b_elapsed = std::chrono::steady_clock::now()-bt;
  
  //initialise map
  auto bt_map=std::chrono::high_resolution_clock::now();
  std::map<int, double> std_map;
  for (std::size_t i=0; i<size; i++)
    std_map.insert({v_int[i], v_d[i]});
  auto b_elapsed_map = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now()-bt_map);

  
  std::cout << "Tree time to build: " << b_elapsed.count()/double(1000000000)  << " seconds\n";
  std::cout << "Map time to build: " << b_elapsed_map.count()/double(1000000)  << " seconds\n";
  

  //perform finds on bst tree
  auto t=std::chrono::high_resolution_clock::now();
  for (std::size_t i=0; i<size; i++)
    tree.find(i);
  auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now()-t);


  //perform finds on std::map
  auto t_map=std::chrono::high_resolution_clock::now();
  for (std::size_t i=0; i<size; i++)
    std_map.find(i);
  auto elapsed_map = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now()-t_map);
    

  std::cout << "Tree time to find nodes: " << elapsed.count()/double(1000)  << " milliseconds\n";
  std::cout << "Map time to find nodes: " << elapsed_map.count()/double(1000)  << " milliseconds\n";

}
