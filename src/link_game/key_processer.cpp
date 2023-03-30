#include "key_processer.h"

namespace game {

link_game::key_processer::key_processer(link_game& game):
  m_game(game)
{
  m_key_callback_index = game.m_window->register_key_callback(
    [this] (gl::window& window, gl::key_token key, int scancode, gl::key_action action, int mods) {
      this->key_callback(window, key, scancode, action, mods);
    }
  );
}

link_game::key_processer::~key_processer() {
  m_game.m_window->unregister_key_callback(m_key_callback_index);
}

void link_game::key_processer::key_callback
  (gl::window&, gl::key_token key, int, gl::key_action action, int)
{
  if(key == gl::key_token::key_C && action == gl::key_action::press) {
    if(m_game.m_game_state == link_game::game_state_t::running) {
      m_game.m_game_state = link_game::game_state_t::waiting;
    }
  }
}

}
