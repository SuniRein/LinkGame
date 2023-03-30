#ifndef GAME_LINK_GAME_H_
#define GAME_LINK_GAME_H_

#include <memory>
#include <optional>

#include "openGL_cpp.h"

#include "game_engine/map.h"

namespace game_engine {
  class unit_renderer;
}

namespace game {

namespace gl = openGL_cpp;

class link_game {
public:
  struct vertex {
    gl::vec2 pos;
    gl::vec4 color;
  };

  link_game() = delete;
  link_game(int unit_count_x, int unit_count_y, gl::window& window);

  ~link_game();

  void init(int unit_count_x, int unit_count_y, gl::window& window);
  void release();

  void on_update();
  void on_render();
  void on_ImGui_render();

  int get_count_x() const {  return m_map->get_count_x();  }
  int get_count_y() const {  return m_map->get_count_y();  }
  int get_exsitring_unit_count() const { return m_map->get_existing_unit_count(); }
  auto get_game_state() const noexcept { return m_game_state; }

  enum class game_state_t {
    running, waiting, terminated,
  };

private:
  using pos_t = game_engine::map::pos_t;

  game_state_t m_game_state = game_state_t::terminated;

  std::unique_ptr<game_engine::map> m_map = nullptr;
  gl::window* m_window;

  std::optional<pos_t> m_actived_pos = std::nullopt;
  static constexpr gl::vec4 active_color = {0.2f, 0.3f, 0.3f, 0.3f};

  auto calculate_pos_in_buffer(pos_t pos) noexcept {  return pos.x * m_map->get_count_y() + pos.y;  }

                                std::unique_ptr<game_engine::unit_renderer> m_renderer;
  class map_generator;          static std::unique_ptr<map_generator> generator;
  class mouse_button_processer; std::unique_ptr<mouse_button_processer> m_clicker;
  class key_processer;          std::unique_ptr<key_processer> m_key_processer;
  class command_processer;      static std::unique_ptr<command_processer> terminal;
};

}

#endif