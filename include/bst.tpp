// -*- mode: c++ -*-

#ifndef __bst_tpp__
#define __bst_tpp__


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
    
  node_ptr->parent = it.get_ptr();  
  if (op(x.first, it->first))
    it.get_ptr()->left.reset(node_ptr);
  else
    it.get_ptr()->right.reset(node_ptr);
  return std::make_pair(iterator{node_ptr}, true);
}






template <typename k, typename v, typename cmp>
std::pair<typename bst<k,v,cmp>::iterator,bool> bst<k,v,cmp>::insert(bst<k,v,cmp>::pair_type&& x)  {

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


template<typename k, typename v, typename cmp>
typename bst<k,v,cmp>::iterator bst<k,v,cmp>::find(const k& x) {
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






template<typename k, typename v, typename cmp>
typename bst<k,v,cmp>::const_iterator bst<k,v,cmp>::find(const k& x) const {
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



template <typename k, typename v, typename cmp>
void bst<k,v,cmp>::build_from_vector(const std::vector<std::pair<const k, v>> vec, std::size_t start, std::size_t end)  {
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
  




template <typename k, typename v, typename cmp>
void bst<k,v,cmp>::erase(const k& x) {
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











#endif
