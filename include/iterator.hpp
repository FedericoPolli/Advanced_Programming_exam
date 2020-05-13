#ifndef __iterator_hpp__
#define __iterator_hpp__

#include <memory>     //unique_ptr::get
#include <iterator>

template <typename node, typename T, typename cmp>
class _iterator{
  
  node* current = nullptr;
  cmp op;
  
public:
  
  using value_type = T;
  using reference = value_type&;
  using pointer = value_type*;
  using iterator_category = std::forward_iterator_tag;
  using difference_type = std::ptrdiff_t;
  
  //ctor
  _iterator() noexcept : current{nullptr} {}
  explicit _iterator(node* p) noexcept : current{p} {}

  //dtor
  ~_iterator() noexcept = default;

  reference operator*() const noexcept {return current->value;}
  pointer operator->() const noexcept {return &(*(*this));}
  bool operator==(const _iterator& rhs) const {return current==rhs.current;}
  bool operator!=(const _iterator& rhs) const {return current!=rhs.current;}

  //copy ctor
  _iterator(const _iterator& it) : current{it.current}, op{it.op} {}

  //copy assignment
  _iterator& operator=(const _iterator& it) {
    current = it.current;
    return *this;
  }
  
  /*
    prefix increment: 3 cases.
    1) If node has a right child, move right and then left for as long as possible.
    2) If node has parent, compare current key with parent key, and if current key is greater, move up. 
       Repeat until parent key is greater, then move up once more and return.
  2.5) The only time the loop doesn't stop is when we start in the rightmost node,
       in which case, we have already traversed the whole tree .
    3) If node is root and there are no right children we have traversed the whole tree
  */

  _iterator& operator++() noexcept {
    
    if (current == nullptr) return *this;

    // Case 1
    if (current->right != nullptr) {
      (*this).move_right();
      while(current->left != nullptr) {
	(*this).move_left();
      }
      return *this;
    }
    
    else {

      // Case 2
      if (current->parent != nullptr) {
	while (op(current->parent->value.first, current->value.first)) {
	  (*this).move_up();

	  // Case 2.5
	  if (current->parent == nullptr) {
	    current = nullptr;
	    return *this;
	  }	    
	}
	
	(*this).move_up();
	return *this;
      }

      // Case 3
      else {
	current = nullptr;
	return *this;
      }

    } 
  }

  
  //postfix increment
  _iterator operator++(int) noexcept {
    _iterator tmp{current};
    ++(*this);
    return tmp;
  }

  
  void move_up() noexcept { current = current->parent; }
  void move_left() noexcept { current = current->left.get(); }
  void move_right() noexcept { current = current->right.get(); }

  
  bool has_left_child() const noexcept {
    if (current != nullptr && current->left.get() != nullptr)
      return true;
    return false;
  }

  
  bool has_right_child() const noexcept {
    if (current != nullptr && current->right.get() != nullptr)
      return true;
    return false;
  }


  node* get_ptr() const noexcept {return current;}

  
  bool is_leaf() const noexcept { return !( ((*this).has_left_child()) || ((*this).has_right_child()) ); }


  //These functions check whether the current node is respectively a left or right child of another node
  
  bool is_left_child () const noexcept {
    _iterator it = *this;
    it.move_up();
    it.move_left();
    if (it == *this)
      return true;
    else
      return false;
  }

  
  bool is_right_child () const noexcept {
    _iterator it = *this;
    it.move_up();
    it.move_right();
    if (it == *this)
      return true;
    else
      return false;
  }
  
};
#endif
