#ifndef LINK_GAME_MAP_H_
#define LINK_GAME_MAP_H_

#include "openGL_cpp.h"

#include "unit.h" // TO DO

#include <vector>
#include <set>
#include <optional>

namespace game_engine {

namespace gl = openGL_cpp;

class map {
public:
  struct pos_t {
    int x, y;

    constexpr pos_t(): x(0), y(0) {}
    constexpr pos_t(int x_, int y_): x(x_), y(y_) {}
    constexpr pos_t(gl::vec2 pos): x(pos.x), y(pos.y) {}

    pos_t& operator+=(const pos_t& rhs) noexcept {  x += rhs.x; y += rhs.y; return *this; }
    pos_t& operator-=(const pos_t& rhs) noexcept {  x -= rhs.x; y -= rhs.y; return *this; }

    gl::vec2 to_gl_vec2() const noexcept {  return gl::vec2(x, y);  }
  };

  map(int count_x, int count_y);
  ~map() = default;

  auto get_count_x() const noexcept { return m_count_x; }
  auto get_count_y() const noexcept { return m_count_y; }
  auto get_size() const noexcept { return m_count_x * m_count_y; }
  auto get_existing_unit_count() const noexcept { return m_existing_unit_count; }

  std::optional<gl::vec4> get_color(int x, int y) const noexcept;
  std::optional<gl::vec4> get_color(pos_t pos) const noexcept;

  void set_color(int x, int y, gl::vec4 color);
  void set_color(pos_t pos, gl::vec4 color);

  bool exsit(int x, int y) const noexcept;
  bool exsit(pos_t pos) const noexcept;

  const auto& get_updated_data() const noexcept {  return m_updated_data; }
  void clear_updated_data() noexcept { m_updated_data.clear(); }

  void destory_unit(int x, int y);
  void destory_unit(pos_t pos);

  gl::vertex_buffer_layout generate_vertex_buffer_layout() const;

  glm::mat4 projection() const noexcept { return glm::ortho<float_t>(0, m_count_x, 0, m_count_y, -1.0f, 1.0f);  }
  
private:
  int m_count_x, m_count_y;
  int m_existing_unit_count;

  std::vector<std::optional<unit>> m_units;

  std::set<pos_t> m_updated_data;

  auto get_real_pos(int x, int y) const noexcept {  return x * get_count_y() + y; }
  auto get_real_pos(pos_t pos) const noexcept { return get_real_pos(pos.x, pos.y);  }
};

inline map::pos_t operator+(const map::pos_t& lhs, const map::pos_t& rhs) noexcept {  return {lhs.x+rhs.x, lhs.y+rhs.y}; }
inline map::pos_t operator-(const map::pos_t& lhs, const map::pos_t& rhs) noexcept {  return {lhs.x-rhs.x, lhs.y-rhs.y}; }

inline bool operator<(const map::pos_t& lhs, const map::pos_t& rhs) noexcept {
  return (lhs.x == rhs.x) ? (lhs.y < rhs.y) : (lhs.x < rhs.x);  
}
inline bool operator==(const map::pos_t& lhs, const map::pos_t& rhs) noexcept { 
  return (lhs.x == rhs.x) && (lhs.y == rhs.y);  
}

}

#endif