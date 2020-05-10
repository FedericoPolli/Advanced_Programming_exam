#ifndef __bst_hpp__
#define __bst_hpp__

//#include <memory>
//#include <iterator>
#include <iostream>
#include <vector>
#include "node.hpp"
#include "iterator.hpp"

template <typename k, typename v, typename cmp = std::less<k>>
class bst {
  
  using pair_type=std::pair<const k, v>;
  using node_type=Node<pair_type>;
  using iterator=_iterator<node_type, pair_type>;
  using const_iterator=_iterator<node_type, const pair_type>;

  //variables
  cmp op;
  std::unique_ptr<node_type> root = nullptr;

public:
  
  bst() noexcept = default; //default constructor
  explicit bst(k key, v value) : root{std::make_unique<node_type>(pair_type(key, value))} {}    //custom ctor 1
  explicit bst(k key, v value, cmp x) : root{std::make_unique<node_type>(pair_type(key, value))}, op{x} {}   //custom ctor 2
  explicit bst(std::vector<pair_type> vec) { build_from_vector(vec, 0, vec.size()-1); }
  ~bst() noexcept = default; // default dtor

  
  //  copy ctor
  bst(const bst& b) : op{b.op} {root = std::make_unique<node_type>(*b.root);}

  // copy assignment
  bst& operator=(const bst& b) {
    root.reset();
    op=b.op;
    root=std::make_unique<node_type>(*b.root);
    return *this;
  }

  
  //move ctor
  bst(bst&& b) noexcept : op{std::move(b.op)}, root{b.root.release()} {}

  //move assignment
  bst& operator=(bst&& b) noexcept {
    op = std::move(b.op);
    root.reset(b.root.release());
    return *this;
  }

  
  //functions definition
  
  std::pair<iterator, bool> insert(const pair_type& x) {

    //if node is already in tree return
    auto result=find(x.first);
    if (result.get_ptr() != nullptr)
      return std::make_pair(result, false);
    
    node_type* node_ptr{new node_type{x}};

    // if tree is empty initialize node as root
    if (root == nullptr) {
      root.reset(node_ptr);
      iterator it{root.get()};
      return std::make_pair(it, true);
    }
    
    iterator it{root.get()};
    while (!it.is_leaf()) {
      if (op(x.first, it->first) && it.has_left_child())  //if key is less than current move left
	it.move_left();
      else if (op(it->first, x.first) && it.has_right_child())  //else move right
	it.move_right();
      else
	break;
    }
    
    node_ptr->parent = it.get_ptr();  
    if (op(x.first, it->first))
      it.get_ptr()->left.reset(node_ptr);
    else
      it.get_ptr()->right.reset(node_ptr);
    return std::make_pair(iterator{node_ptr}, true);
  }

  
  std::pair<iterator, bool> insert(pair_type&& x) {

    //if node is already in tree return
    auto result=find(x.first);
    if (result != end())
      return std::make_pair(result, false);
    
    node_type* node_ptr{new node_type{std::move(x)}};

    // if tree is empty initialize node as root
    if (root == nullptr) {
      root.reset(node_ptr);
      iterator it{root.get()};
      return std::make_pair(it, true);
    }
    
    iterator it{root.get()};
    while (!it.is_leaf()) {
      if (op(x.first, it->first) && it.has_left_child())  //if key is less than current move left
	it.move_left();
      else if (op(it->first, x.first) && it.has_right_child())  //else move right
	it.move_right();
      else
	break;
    }
    
    node_ptr->parent = it.get_ptr(); 
    if (op(x.first, it->first))
      it.get_ptr()->left.reset(node_ptr);
    else
      it.get_ptr()->right.reset(node_ptr);
    return std::make_pair(iterator{node_ptr}, true);
  }


  template< class... Types >
  std::pair<iterator,bool> emplace(Types&&... args) {
    return insert(pair_type{std::forward<Types>(args)...});
  }


  void clear() { root.reset(nullptr); }

  
  //go left starting from root
  iterator begin() {
    if (root == nullptr)
      return iterator{nullptr};
    iterator it{root.get()};
    while (it.has_left_child())
      it.move_left();
    return it;
  }
  
  
  const_iterator begin() const {
    if (root == nullptr)
      return const_iterator{nullptr};
    const_iterator it{root.get()};
    while (it.has_left_child())
      it.move_left();
    return it;
  }

  
  const_iterator cbegin() const {
    if (root == nullptr)
      return const_iterator{nullptr};
    const_iterator it{root.get()};
    while (it.has_left_child())
      it.move_left();
    return it;
  }


  iterator end() { return iterator{nullptr}; }
  const_iterator end() const { return const_iterator{nullptr}; }
  const_iterator cend() const { return const_iterator{nullptr}; }

  
  //search the tree from begin to end
  iterator find(const k& x) {
    if (root == nullptr)
      return iterator{nullptr};
    iterator it = begin();
    while(it != end()) {
      if (it->first == x)
	return it;
      ++it;
    }
    return end();
  }
  
  
  const_iterator find(const k& x) const {
    if (root == nullptr)
      return const_iterator{nullptr};
    const_iterator it = begin();
    while(it != end()) {
      if (it->first == x)
	return it;
      ++it;
    }
    std::cout << "not found\n";
    return end();
  }


  void balance() {
    //if tree is empty return
    if (root == nullptr)
      return;
    
    iterator it=begin();
    std::vector<pair_type> vec;

    //fill vector with tree elements
    while (it != end()) {
      vec.push_back(std::make_pair(it->first, it->second));
      it++;
    }

    //clear and then build from vector
    clear();
    build_from_vector(vec, 0, vec.size()-1);
  }


  void build_from_vector(const std::vector<pair_type> vec, std::size_t start, std::size_t end) {
    if (start > end)
      return;
    
    std::size_t t{(start+end)/2};
    insert(vec[t]);
    if (start != end) {
      if (t !=  0)
	build_from_vector(vec, start, t-1);
      build_from_vector(vec, t+1, end);
    }
  }
  
  
  v& operator[](const k& x) {
    auto it = find(x);
    if ( it == end() ) {
      auto p = insert(std::make_pair(x, v{}));
      return (*p.first).second;
    }
    else
      return (*it).second; 
  }

    
  v& operator[](k&& x) {
    auto it = find(x);
    if ( it == end() ) {
      auto p = insert(std::make_pair(std::move(x), v{}));
      return (*p.first).second;
    }
    else
      return (*it).second; 
  }


  friend
  std::ostream& operator<<(std::ostream& os, const bst& x) {
    const_iterator it = x.begin();
    std::cout << "(key, value)\n";
    while (it != x.end()) {
      os << "(" << it->first << "," << it->second << ") ";
      ++it;
    }
    std::cout << std::endl;
    return os;
  }

  
  void erase(const k& x) {
    auto res = find(x);
    //if no key do nothing
    if (res == end())
      return;

    node_type* node_ptr = res.get_ptr();

    if (res.is_leaf() && node_ptr == root.get()) {
      root.reset();
      return;
    }
    
    //if node is leaf simply delete it
    if (res.is_leaf() && node_ptr != root.get() ) {
      if (res.is_left_child()) 
	(*node_ptr).parent->left.reset();	
      else
	(*node_ptr).parent->right.reset();
      node_ptr = nullptr;
      return;
    }

    //else vectorize without the pair associated with key, clear and then rebuild
    else {
      iterator it=begin();
      std::vector<pair_type> vec;

      //fill vector with tree elements
      while (it != end()) {
	if (it != res)
	  vec.push_back(std::make_pair(it->first, it->second));
	it++;
      }
      clear();
      build_from_vector(vec, 0, vec.size()-1);
    }   
  }
  
};



#endif
