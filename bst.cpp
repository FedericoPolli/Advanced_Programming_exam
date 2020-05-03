#ifndef __node_cpp__
#define __node_cpp__

#include <memory>    //std::unique_ptr
#include <utility>   //std::move

template <typename T>
struct Node{
  T value;
  std::unique_ptr<Node> left=nullptr;
  std::unique_ptr<Node> right=nullptr;
  Node* parent=nullptr;

  //constructors
  Node() noexcept = default;
  explicit Node(const T& v) : value{v} {}
  explicit Node(T&& v) : value{std::move(v)} {}
  explicit Node(const T& v, const Node* p) : value{v}, parent{p} {}
  explicit Node(const std::unique_ptr<Node>& p) : value{p->value}
  {
    this->parent=p->parent;
    if (p->left)
      left=std::make_unique<Node>(p->left, this);
    if (p->right)
      right=std::make_unique<Node>(p->right, this);
  }

  //destructor
  ~Node() noexcept = default;

  Node(Node&& n) noexcept = default;    //move ctor
  Node& operator=(Node&& n) noexcept = default;  //move assignment

  Node(const Node& n) : value{n.value} {} //copy ctor
  Node& operator=(const Node& n) {
    (*this) = std::move(n);
    return *this;
  }
};




#endif















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

















#ifndef __bst
#define __bst

#include <memory>
#include <iterator>
#include <iostream>

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
  ~bst() noexcept = default; // default dtor

  //copy ctor
  // bst(const bst& b) : root{std::make_unique<node_type>(b.root)} {}
  // bst& operator=(const bst& b) {
  //   root.reset();
  //   auto tmp = b;
  //   *this=std::move(tmp);
  //   return *this;
  // }
  
  //move ctor
  // bst(bst&& b) noexcept = default;
  // bst& operator=(bst&& b) noexcept {
  //   root = std::move(b.root);
  //   return *this;
  // }
  
  //functions definition
  
  std::pair<iterator, bool> insert(const pair_type& x) {

    //if node is already in tree return
    auto result=find(x.first);
    if (result.get_ptr() != nullptr)
      return std::make_pair(result, false);
    
    node_type node{x};
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
    node.parent = it.get_ptr();
    if (op(x.first, it->first))
      it.get_ptr()->left.reset(node_ptr);
    else
      it.get_ptr()->right.reset(node_ptr);
    return std::make_pair(it, true);
  }

  
  // std::pair<iterator, bool> insert(pair_type&& x) {
  //   auto result=find(x.first);
  //   if (result.get_ptr() != nullptr)
  //     return std::make_pair(result, false);
    
  //   node_type node{std::move(x)};
  //   node_type* node_ptr{&node};			
  //   if (root == nullptr) {
  //     root.reset(node_ptr);
  //     iterator it{root.get()};
  //     return std::make_pair(it, true);
  //   }

  //   iterator it{root.get()};
  //   while (!it.is_leaf()) {
  //     if (op(x.first, it->first))  //if key is less than current move left
  // 	it.move_left();             //else move right
  //     else
  // 	it.move_right();
  //   }
  //   node.parent = it.get_ptr();
  //   if (op(x.first, it->first))
  //     it.get_ptr()->left.reset(node_ptr);
  //   else
  //     it.get_ptr()->right.reset(node_ptr);
  //   return std::make_pair(it, true);
  // }


  template< class... Types >
  std::pair<iterator,bool> emplace(Types&&... args);


  void clear();

  
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


  void balance();

  
  v& operator[](const k& x);
  v& operator[](k&& x);


  friend
  std::ostream& operator<<(std::ostream& os, const bst& x) {
    const_iterator it = x.begin();
    std::cout << "(key, value)\n";
    while (it != x.end()) {
      os << "(" << it->first << "," << it->second << ") ";
      it++;
    }
    std::cout << std::endl;
    return os;
  }

  
  void erase(const k& x);
  
};



#endif

int main() {
  using couple = std::pair<const int, int>;
  using tree = bst<int, int>;
  tree b{};
  couple c1(3,3);
  b.insert({2,2});
  b.insert({4,1});
  b.insert(c1);
  std::cout << b;
  b.find(3);
}
  
