template <typename T, class... Types>
std::unique_ptr<T> make_unique(Types&&... args) {
  T* x = new T{std::forward<Types>(args)...};
return std::unique_ptr<T> {x};
}
