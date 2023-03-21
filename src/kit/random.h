#ifndef SUNIREIN_RANDOM_H_
#define SUNIREIN_RANDOM_H_

#include <random>
#include <ctime>

namespace SuniRein_kit {

/**
 * @brief 返回一个随机数，为 [min, max] 间的整数
 * 
 * @tparam int_t 一个整数类型
 * @return int_t 
 */
template<typename int_t>
int_t random(int_t min, int_t max) {
  static_assert(std::is_integral_v<int_t>);
  static std::default_random_engine engine(::time(0));
  static std::uniform_int_distribution<int_t> distribution;
  typename std::uniform_int_distribution<int_t>::param_type param(min, max);
  return distribution(engine, param);
}

/**
 * @brief 返回给定容器的随机一个元素
 * 
 * @tparam container_t 容器类型 
 * @param container 所给容器，要求非空，且支持随机访问
 * @return container_t::value_type 
 */
template<typename container_t>
auto random_element(const container_t& container) -> typename container_t::value_type {
  auto size = container.size();
  assert(size > 0);
  auto pos = random<delctype(size)>(0, size - 1);
  return container[pos];
}

/**
 * @brief 返回给定范围的随机一个元素
 * 
 * @param begin 范围的起始指针
 * @param count 范围内的元素数目，要求为正
 */
template<typename T>
T random_element(const T* const begin, std::size_t count) {
  assert(count > 0);
  auto pos = random<std::size_t>(0, count - 1);
  return *(begin + pos);
}

}

#endif