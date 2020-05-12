#ifndef __bst_hpp__
#define __bst_hpp__

#include <memory>    // included for clarity, as they are
#include <iterator>  // already included in iterator.hpp

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

  // custom ctors
  explicit bst(k key, v value) : root{std::make_unique<node_type>(pair_type(key, value))} {}  
  explicit bst(k key, v value, cmp x) : root{std::make_unique<node_type>(pair_type(key, value))}, op{x} {}   

  //custom constructor that builds a tree from a vector of pairs
  explicit bst(std::vector<pair_type> vec) { build_from_vector(vec, 0, vec.size()-1); }
  
  ~bst() noexcept = default; // default dtor

  // copy ctor: performs a deep copy through copy ctor of Node
  
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
  
  std::pair<iterator, bool> insert(const pair_type& x); 
  std::pair<iterator, bool> insert(pair_type&& x);


  template< class... Types >
  std::pair<iterator,bool> emplace(Types&&... args) {
    return insert(pair_type{std::forward<Types>(args)...});
  }


  void clear() { root.reset(nullptr); }


  /*
    These functions return an iterator to the leftmost node.
    Starting from the root node they go left for as long as possible.
  */

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

  
  iterator find(const k& x); 
  const_iterator find(const k& x) const;

  
  void balance();

  
  void build_from_vector(const std::vector<pair_type> vec, std::size_t start, std::size_t end);

  
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

  
  void erase(const k& x);
  

  friend
  std::ostream& operator<<(std::ostream& os, const bst& x) {
    const_iterator it = x.begin();
    std::cout << "(key, value)\n";
    while (it != x.end()) {
      os << "(" << it->first << ", " << it->second << ") ";
      ++it;
    }
    std::cout << std::endl;
    return os;
  }

  
};

#include "bst.tpp"

#endif
