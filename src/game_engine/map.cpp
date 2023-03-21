#include "map.h"

#include "unit.h"

namespace game_engine {

map::map(int count_x, int count_y):
  m_count_x(count_x), m_count_y(count_y), m_existing_unit_count(0),
  m_units(count_x * count_y, std::nullopt)
{}

std::optional<gl::vec4> map::get_color(int x, int y) const noexcept {
  auto pos = get_real_pos(x, y);
  return m_units[pos].has_value() ? std::make_optional(m_units[pos].value().get_color()) : std::nullopt;
}
std::optional<gl::vec4> map::get_color(pos_t pos) const noexcept {
  return get_color(pos.x, pos.y);
}

void map::set_color(int x, int y, gl::vec4 color) {
  auto pos = get_real_pos(x, y);
  if(m_units[pos].has_value()) {
    m_units[pos].value().set_color(color);
  } else {
    m_units[pos].emplace(color);
    ++m_existing_unit_count;
  }
  m_updated_data.insert({x, y});
}
void map::set_color(pos_t pos, gl::vec4 color) {
  set_color(pos.x, pos.y, color);
}

bool map::exsit(int x, int y) const noexcept {  return m_units[get_real_pos(x, y)].has_value(); }
bool map::exsit(pos_t pos) const noexcept { return exsit(pos.x, pos.y); }

void map::destory_unit(int x, int y) {
  auto pos = get_real_pos(x, y);
  if(m_units[pos].has_value()) {
    m_units[pos].reset();
    m_updated_data.insert( {x, y} );
    --m_existing_unit_count;
  }
}
void map::destory_unit(pos_t pos) {
  destory_unit(pos.x, pos.y);
}

gl::vertex_buffer_layout map::generate_vertex_buffer_layout() const {
  gl::vertex_buffer_layout layout;
  layout.push<gl::float_t>(2);
  layout.push<gl::float_t>(4);
  return layout;
}

}