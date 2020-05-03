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
  Node() noexcept {};
  explicit Node(const T& v) : value{v} {}
  explicit Node(T&& v) : value{std::move(v)} {}
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
