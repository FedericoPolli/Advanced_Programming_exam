// -*- mode: c++ -*-

#ifndef __bst_tpp__
#define __bst_tpp__

/*
  Function that inserts a node in the tree. If the node already exists it does nothing.
  The idea is that a new node can always be inserted as a leaf, therefore the function moves
  down, starting from the root node, by comparing the given key with the current node's key.
  Finally it inserts the node as either right or left child.
*/

template <typename k, typename v, typename cmp>
std::pair<typename bst<k,v,cmp>::iterator,bool> bst<k,v,cmp>::insert(const bst<k,v,cmp>::pair_type& x) {

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
  
  //now the iterator points to the parent of the node to insert
  //and the node has to be either a left or a right child.
  node_ptr->parent = it.get_ptr();  
  if (op(x.first, it->first))
    it.get_ptr()->left.reset(node_ptr);
  else
    it.get_ptr()->right.reset(node_ptr);
  return std::make_pair(iterator{node_ptr}, true);
}



//overloaded insert, the only differene being the use of std::move.

template <typename k, typename v, typename cmp>
std::pair<typename bst<k,v,cmp>::iterator,bool> bst<k,v,cmp>::insert(bst<k,v,cmp>::pair_type&& x)  {

  auto result=find(x.first);
  if (result != end())
    return std::make_pair(result, false);
    
  node_type* node_ptr{new node_type{std::move(x)}};

  if (root == nullptr) {
    root.reset(node_ptr);
    iterator it{root.get()};
    return std::make_pair(it, true);
  }
    
  iterator it{root.get()};
  while (!it.is_leaf()) {
    if (op(x.first, it->first) && it.has_left_child())  
      it.move_left();
    else if (op(it->first, x.first) && it.has_right_child()) 
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




/*
  Finds a given key in the tree, and returns an iterator to the associated node. 
  If the key is not found returns an iterator pointing to the end of the tree.
  The function simply traverses the tree and if the key is found it returns.
*/

template<typename k, typename v, typename cmp>
typename bst<k,v,cmp>::iterator bst<k,v,cmp>::find(const k& x) noexcept {
  if (root == nullptr)
    return iterator{nullptr};
  iterator it{root.get()};
  if (it->first == x)
      return it;
      
  while (!it.is_leaf()) {
    if (op(x, it->first) && it.has_left_child() )
      it.move_left();
    else if (op(it->first, x) && it.has_right_child() )
      it.move_right();
    else
      break;
    if (it->first == x)
      return it;
  }
  return end();
}


//Overloaded find which returns a const_iterator

template<typename k, typename v, typename cmp>
typename bst<k,v,cmp>::const_iterator bst<k,v,cmp>::find(const k& x) const noexcept {
  if (root == nullptr)
    return const_iterator{nullptr};
  iterator it{root.get()};
  if (it->first == x)
      return it;
  
  while (!it.is_leaf()) {
    if (op(x, it->first) && it.has_left_child() )
      it.move_left();
    else if (op(it->first, x) && it.has_right_child() )
      it.move_right();
    else
      break;
    if (it->first == x)
      return it;
  }
  return end();
}




/*
  Balances the tree. It copies the tree in a vector of pairs,
  then it clears the tree and rebuilds it from said vector in a balacned way.
*/

template <typename k, typename v, typename cmp>
void bst<k,v,cmp>::balance()  {
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




/*
  Recursively builds a balanced tree by finding the median element
  and then calls itself on the left and right halves.
*/

template <typename k, typename v, typename cmp>
void bst<k,v,cmp>::build_from_vector(const std::vector<std::pair<const k, v>> vec, std::size_t start, std::size_t end)  {
  //stopping condition
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




/*
  Erases the node with the given key. If the node is not found it returns, else
  it distinguishes two cases depending on whether the node has or has not any children.
  If the node is a leaf it simply sets the pointers to nullptr, otherwise it stores 
  the tree minus the node to erase in a vector, then clears the tree and rebuilds it.
*/

template <typename k, typename v, typename cmp>
void bst<k,v,cmp>::erase(const k& x) {
  
  auto res = find(x);
  if (res == end())
    return;

  node_type* node_ptr = res.get_ptr();

  //if there is only the root node in the tree set root to nullptr
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
    return;
  }

  else {
    iterator it=begin();
    std::vector<pair_type> vec;

    //fill vector with tree elements minus the element to delete
    while (it != end()) {
      if (it != res)
	vec.push_back(std::make_pair(it->first, it->second));
      it++;
    }
    clear();
    build_from_vector(vec, 0, vec.size()-1);
  }   
}


#endif
