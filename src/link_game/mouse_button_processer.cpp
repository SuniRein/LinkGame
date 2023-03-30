#include "mouse_button_processer.h"

#define array_size(array) (sizeof(array) / sizeof(array[0]))

#include <stack>

namespace game {

link_game::mouse_button_processer::mouse_button_processer(link_game &game):
  m_game(game)
{
  m_mouse_button_callback_index = game.m_window->register_mouse_button_callback( 
    [this] (gl::window& win, gl::mouse_button_token button, gl::mouse_button_action action, int mods) {
      this->mouse_button_callback(win, button, action, mods);
    }
  );
}

link_game::mouse_button_processer::~mouse_button_processer() {
  m_game.m_window->unregister_mouse_button_callback(m_mouse_button_callback_index);
}

void link_game::mouse_button_processer::mouse_button_callback
  (gl::window& window, gl::mouse_button_token button, gl::mouse_button_action action, int) 
{
  if(m_game.m_game_state == link_game::game_state_t::running) {
    if(button == gl::mouse_button_token::button_left && action == gl::mouse_button_action::press) {
      auto [x, y] = window.get_cursor_pos(); 
      auto [width, height] = window.get_size();
      pos_t unit_pos = {
        static_cast<int>(x) / (width / m_game.m_map->get_count_x()),
        static_cast<int>(height - y) / (height / m_game.m_map->get_count_y())
      };
      active_pos(unit_pos);
    }
  }
}

// 当且仅当场上已存在一个激活位置，且两个位置可以消去时执行消除
// 否则激活位置改为光标位置
void link_game::mouse_button_processer::active_pos(pos_t pos) {
  if(m_game.m_actived_pos.has_value() && is_linkable(m_game.m_actived_pos.value(), pos)) {
    m_game.m_map->destory_unit(m_game.m_actived_pos.value());
    m_game.m_map->destory_unit(pos);
    m_game.m_actived_pos.reset();
  } else {
    m_game.m_actived_pos = pos;
  }
}

bool link_game::mouse_button_processer::is_linkable(pos_t begin, pos_t end) const {
  // 位置相等或不存在或颜色不相等
  if(begin == end ||
     !m_game.m_map->exsit(begin) || !m_game.m_map->exsit(end) ||
     m_game.m_map->get_color(begin) != m_game.m_map->get_color(end)) 
  {
    return false;
  }

  auto count_x = m_game.m_map->get_count_x();
  auto count_y = m_game.m_map->get_count_y();

  static pos_t directions[] = {
    { 1,  0}, // RIGHT
    {-1,  0}, // LEFT
    { 0, -1}, // UP
    { 0,  1}, // DOWN
  };

  static int corner_last[100][100][array_size(directions)]; // TO DO
  memset(corner_last, 0, sizeof(corner_last));
  for(int i = 0; i != count_x; ++i) {
    for(int j = 0; j != count_y; ++j) {
      if(!m_game.m_map->exsit(i, j)) {
        for(int k = 0; k != array_size(directions); ++k) {
          corner_last[i][j][k] = 3;
        }
      }
    }
  }

  struct node {
    pos_t pos;
    pos_t last_direction;
    int corner;
  };
  std::stack<node> search;
  search.push( {begin, {0, 0}, -1} ); // 起点的方向与四个方向都不同，从起点走出去时必导致 corner 递增，所以设为 -1

  while(!search.empty()) {
    auto now = search.top();
    search.pop();

    for(auto i = 0; i != array_size(directions); ++i) {
      node next = {
        now.pos + directions[i],
        directions[i],
        now.corner + (now.last_direction != directions[i])
      };

      if(next.corner <= 2) {
        if(next.pos == end) {
          return true;
        } else if(corner_last[next.pos.x][next.pos.y][i] > next.corner) {
          corner_last[next.pos.x][next.pos.y][i] = next.corner;
          search.push(next);
        }
      }
    }
  }

  return false;
}

}