#include "link_game.h"

#include "map_generator.h"
#include "mouse_button_processer.h"
#include "key_processer.h"
#include "command_processer.h"

#include "../game_engine/map.h"
#include "../game_engine/unit_renderer.h"

#include <tuple>

namespace game {

namespace {
  auto generate_vertices(gl::vec2 pos, gl::vec4 color, gl::float_t width) {
    constexpr auto VERTEX_PER_UNIT = 4;
    std::unique_ptr<link_game::vertex[]> vertices( new link_game::vertex[VERTEX_PER_UNIT] );

    vertices[0].pos = {pos.x, pos.y};
    vertices[1].pos = {pos.x, pos.y + width};
    vertices[2].pos = {pos.x + width, pos.y + width};
    vertices[3].pos = {pos.x + width, pos.y};

    for(int i = 0; i != VERTEX_PER_UNIT; ++i) {
      vertices[i].color = color;
    }

    return vertices;
  }
}

link_game::link_game(int unit_count_x, int unit_count_y, gl::window& window) {
  init(unit_count_x, unit_count_y, window);
}

link_game::~link_game() {}

std::unique_ptr<link_game::map_generator> link_game::generator = std::make_unique<link_game::map_generator>();

void link_game::init(int unit_count_x, int unit_count_y, gl::window& window) {
  m_game_state = game_state_t::running;

  m_map = std::make_unique<game_engine::map>(unit_count_x, unit_count_y);
  (*generator)(*this);

  m_actived_pos = std::nullopt;
  m_window = &window;

  m_renderer = std::make_unique<game_engine::unit_renderer>(m_map->get_size() + 1, m_map->generate_vertex_buffer_layout());
  m_clicker = std::make_unique<mouse_button_processer>(*this);
  m_key_processer = nullptr;;
  m_command_processer = nullptr;
}

void link_game::release() {
  m_game_state = game_state_t::terminated;

  m_map.release();
  m_actived_pos.reset();
  m_window = nullptr;

  m_clicker.release();
  m_key_processer.release();
  m_command_processer.release();
}

void link_game::on_update() {
  if(m_game_state == game_state_t::running) {
    for(auto pos : m_map->get_updated_data()) {
      constexpr gl::vec4 BLANK_COLOR = {0.0f, 0.0f, 0.0f, 0.0f};

      auto color = m_map->get_color(pos);
      auto vertices = generate_vertices(pos.to_gl_vec2(), color.value_or(BLANK_COLOR), 1.0f);
      m_renderer->update(calculate_pos_in_buffer(pos), vertices.get());
    }
    m_map->clear_updated_data();

    if(m_actived_pos.has_value()) {
      auto vertices = generate_vertices(m_actived_pos.value().to_gl_vec2(), active_color, 1.0f);
      m_renderer->update(m_map->get_size(), vertices.get());
    }
  }
}

void link_game::on_render() {
  if(m_game_state == game_state_t::running) {
    m_renderer->set_uniform("u_MVP", m_map->projection());
    m_renderer->draw(m_map->get_size() + m_actived_pos.has_value());
  }
}

}