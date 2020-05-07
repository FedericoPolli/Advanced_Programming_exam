#ifndef __iterator_hpp__
#define __iterator_hpp__

#include <memory>     //unique_ptr::get
#include <iterator>

template <typename node, typename T>
class _iterator{
  
  node* current = nullptr;
  
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

  //prefix increment
  _iterator& operator++() noexcept {
    
    if (current == nullptr) return *this;

    //if have right child, move right
    //and then left for as long as possible
    if (current->right != nullptr) {
      (*this).move_right();
      while(current->left != nullptr) {
	(*this).move_left();
      }
      return *this;
    }    
    else {
      
      //if there is a parent move up until key is greater
      if (current->parent != nullptr) {
	while (current->value.first > current->parent->value.first) {
	  (*this).move_up();

	  //if top node exit
	  if (current->parent == nullptr) {
	    current = nullptr;
	    return *this;
	  }	    
	}	
	(*this).move_up();
	return *this;
      }
      
      else {
	//we are in the top node, exit
	current=nullptr;
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

  bool has_left_child() {
    if (current != nullptr && current->left.get() != nullptr)
      return true;
    return false;
  }

  bool has_right_child() {
    if (current != nullptr && current->right.get() != nullptr)
      return true;
    return false;
  }

  bool is_leaf() { return !( ((*this).has_left_child()) || ((*this).has_right_child()) ); }


  node* get_ptr() noexcept {return current;}
  
};
#endif
