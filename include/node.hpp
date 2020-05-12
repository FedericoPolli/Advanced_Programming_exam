#ifndef __node_hpp__
#define __node_hpp__

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
  Node(const T& v, Node* p=nullptr) : value{v}, parent{p} {}
  Node(T&& v, Node* p=nullptr) : value{std::move(v)}, parent{p} {}


  //destructor
  ~Node() noexcept = default;

  //copy constructor
  Node(const Node& n, Node* p = nullptr) : value{n.value}, parent{p}
  {
    if (n.left)
      left=std::make_unique<Node>(*n.left, this);
    if (n.right)
      right=std::make_unique<Node>(*n.right, this);
  }

  
};




#endif
