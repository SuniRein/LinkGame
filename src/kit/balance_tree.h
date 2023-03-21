#ifndef SUNIREIN_BALENCE_TREE_H_
#define SUNIREIN_BALENCE_TREE_H_

#include <ctime>
#include <random>
#include <tuple>
#include <optional>
#include <functional>
#include <stdexcept>

namespace SuniRein_kit {

/**
 * @brief 基本的平衡树，支持插入、删除、快速访问等操作
 * 
 * @tparam T 数据类型
 * @tparam Compare_Equal 等于运算符
 * @tparam Compare_Less 小于运算符
 */
template<typename T, 
         typename Compare_Equal = std::equal_to<T>,
         typename Compare_Less = std::less<T>>
class balance_tree {
public:
  using size_type = unsigned int;

  balance_tree() = default;
  ~balance_tree() { delete_deeply(m_root); }

  void push(const T& value);
  void push(T&& value);

  bool pop_one(const T& value);
  size_type pop_all(const T& value);

  void clear() { delete_deeply(m_root); m_root = nullptr; }

  bool exsit(const T& value) const;
  size_type count(const T& value) const;

  std::optional<T> get_nth(size_type rank) const;

  auto size() const { return get_size(m_root); }
  bool empty() const { return !m_root; }

private:
  struct node;
  using pointer = node*;

  pointer m_root = nullptr;

  std::pair<pointer, pointer> split_less_equal(pointer root, const T& value) const;
  std::pair<pointer, pointer> split_less(pointer root, const T& value) const;
  pointer merge(pointer l, pointer r) const;

  void update(pointer root) const;
  void delete_deeply(pointer root);

  static std::default_random_engine engine;
  pointer new_node(const T& value) const;
  pointer new_node(T&& value) const;

  auto get_size(pointer now) const { return now ? now->size : 0; }
  auto get_value(pointer now) const { return now ? std::make_optional<T>(now->value) : std::nullopt; }
};

template<typename T, typename Compare_Equal, typename Compare_Less>
struct balance_tree<T, Compare_Equal, Compare_Less>::node {
  T value;
  size_type size;
  size_type key;
  pointer lchild, rchild;
};

template<typename T, typename Compare_Equal, typename Compare_Less>
inline std::default_random_engine balance_tree<T, Compare_Equal, Compare_Less>::engine(::time(0));

template<typename T, typename Compare_Equal, typename Compare_Less>
auto balance_tree<T, Compare_Equal, Compare_Less>::new_node(const T& value) const -> pointer {
  auto ret = new node {
    value, 1, static_cast<size_type>(engine()), nullptr, nullptr
  };
  return ret;
}

template<typename T, typename Compare_Equal, typename Compare_Less>
auto balance_tree<T, Compare_Equal, Compare_Less>::new_node(T&& value) const -> pointer {
  auto ret = new node {
    std::move(value), 1, static_cast<size_type>(engine()), nullptr, nullptr
  };
  return ret;
}

template<typename T, typename Compare_Equal, typename Compare_Less>
inline auto balance_tree<T, Compare_Equal, Compare_Less>::split_less_equal(pointer root, const T& value) const -> std::pair<pointer, pointer>
{
  if(!root) { return {nullptr, nullptr}; }

  pointer l, r;
  if(root->value == value || root->value < value) {
    l = root;
    std::tie(l->rchild, r) = split_less_equal(l->rchild, value);
  } else {
    r = root;
    std::tie(l, r->lchild) = split_less_equal(r->lchild, value);
  }
  update(root);
  
  return {l, r};
}

template<typename T, typename Compare_Equal, typename Compare_Less>
inline auto balance_tree<T, Compare_Equal, Compare_Less>::split_less(pointer root, const T& value) const -> std::pair<pointer, pointer>
{
  if(!root) { return {nullptr, nullptr}; }

  pointer l, r;
  if(root->value < value) {
    l = root;
    std::tie(l->rchild, r) = split_less(l->rchild, value);
  } else {
    r = root;
    std::tie(l, r->lchild) = split_less(r->lchild, value);
  }
  update(root);
  
  return {l, r};
}

template<typename T, typename Compare_Equal, typename Compare_Less>
inline auto balance_tree<T, Compare_Equal, Compare_Less>::merge(pointer l, pointer r) const -> pointer {
  if(!l) return r;
  if(!r) return l;

  // 大根堆
  if(l->key > r->key) {
    l->rchild = merge(l->rchild, r);
    update(l);
    return l;
  } else {
    r->lchild = merge(l, r->lchild);
    update(r);
    return r;
  }
}

template<typename T, typename Compare_Equal, typename Compare_Less>
inline void balance_tree<T, Compare_Equal, Compare_Less>::update(pointer root) const {
  if(root) {
    root->size = get_size(root->lchild) + get_size(root->rchild) + 1;
  }
}

template<typename T, typename Compare_Equal, typename Compare_Less>
void balance_tree<T, Compare_Equal, Compare_Less>::delete_deeply(pointer root) {
  if(root) {
    delete_deeply(root->lchild);
    delete_deeply(root->rchild);
    delete root;
  }
}

template<typename T, typename Compare_Equal, typename Compare_Less>
inline void balance_tree<T, Compare_Equal, Compare_Less>::push(const T& value) {
  auto [l, r] = split_less_equal(m_root, value);
  m_root = merge(merge(l, new_node(value)), r);
}

template<typename T, typename Compare_Equal, typename Compare_Less>
inline void balance_tree<T, Compare_Equal, Compare_Less>::push(T&& value) {
  auto [l, r] = split_less_equal(m_root, value);
  m_root = merge(merge(l, new_node(std::move(value))), r);
}

template<typename T, typename Compare_Equal, typename Compare_Less>
inline bool balance_tree<T, Compare_Equal, Compare_Less>::pop_one(const T& value) {
  auto [l, r] = split_less_equal(m_root, value);
  auto [x, y] = split_less(l, value);

  auto temp = y;

  if(y) {
    y = merge(y->lchild, y->rchild);
  }

  m_root = merge(merge(x, y), r);
  
  if(temp) {
    delete temp;
    return true;
  } else {
    return false;
  }
}

template<typename T, typename Compare_Equal, typename Compare_Less>
inline auto balance_tree<T, Compare_Equal, Compare_Less>::pop_all(const T& value) -> size_type {
  auto [l, r] = split_less_equal(m_root, value);
  auto [x, y] = split_less(l, value);

  m_root = merge(x, r);
  
  auto ret = get_size(y);
  delete_deeply(y);
  return ret;
}

template<typename T, typename Compare_Equal, typename Compare_Less>
bool balance_tree<T, Compare_Equal, Compare_Less>::exsit(const T& value) const {
  return count(value);
}

template<typename T, typename Compare_Equal, typename Compare_Less>
auto balance_tree<T, Compare_Equal, Compare_Less>::count(const T& value) const -> size_type {
  auto [l, r] = split_less_equal(m_root, value);
  auto [x, y] = split_less(l, value);
  auto ret = get_size(y);
  m_root = merge(merge(x, y), r);
  return ret;
}

template<typename T, typename Compare_Equal, typename Compare_Less>
inline std::optional<T> balance_tree<T, Compare_Equal, Compare_Less>::get_nth(size_type rank) const {
  if(++rank > size()) { throw std::out_of_range("Balance Tree Get Out Of Range."); }

  auto now = m_root;
  while(now) {
    if(get_size(now->lchild) + 1 == rank) break;

    if(get_size(now->lchild) >= rank) {
      now = now->lchild;
    } else {
      rank -= get_size(now->lchild) + 1;
      now = now->rchild;
    }
  }
  return get_value(now);
}

}

#endif