#ifndef SUNIREIN_RANDOM_POOL_H_
#define SUNIREIN_RANDOM_POOL_H_

#include "balance_tree.h"
#include "random.h"

namespace SuniRein_kit {

template<typename T,
         typename Compare_Equal = std::equal_to<T>,
         typename Compare_Less = std::less<T>>
class random_pool {
public:
  random_pool() = default;

  void push(const T& value) { m_data.push(value); }
  void push(T&& value) { m_data.push(std::move(value)); }

  void erase(const T& value) { m_data.pop_all(value); }

  void clear() { m_data.clear(); }

  auto size() const { return m_data.size(); }
  auto empty() const { return m_data.empty(); }

  auto operator() () {
    auto pos = random<decltype(m_data.size())>(0, m_data.size() - 1);
    auto ret = m_data.get_nth(pos);
    m_data.pop_one(ret.value());
    return ret;
  }

private:
  balance_tree<T, Compare_Equal, Compare_Less> m_data;
};

}

#endif