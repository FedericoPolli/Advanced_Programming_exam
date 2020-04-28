#ifndef __iterator_hpp__
#define __iterator_hpp__

#include <memory>
#include <iterator>

template <typename node, typename T>
class _iterator{
  
  node* current=nullptr;
  
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
  bool operator==(const _iterator& rhs) const {return current==rhs.current}
  bool operator!=(const _iterator& rhs) const {return current!=rhs.current}

  //prefix increment
  _iterator& operator++() {
    if !(current) return *this;
  }

  //postfix increment
  _iterator operator++(int) {
    _iterator tmp{current};
    ++(*this);
    return tmp;
  }
}
#endif
