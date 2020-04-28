#ifndef __node_cpp__
#define __node_cpp__

#include <memory>
#include <utility>

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

  Node(const Node& n) : value{n.value} {}
  Node& operator=(const Node& n) {
    (*this) = std::move(n);
    return *this;
  }
};




#endif

int main() {
  using coppia=std::pair<int, int>;
  Node<coppia> n{};
  Node<coppia> m{coppia(1,2)};
  n=m;
  return 0;
}
