#include "link_game.h"

namespace game {

class link_game::key_processer {
public:
  key_processer() = delete;
  key_processer(link_game& game);
  ~key_processer();

  key_processer(const key_processer&) = delete;
  key_processer& operator=(const key_processer&) = delete;

private:
  link_game& m_game;
  gl::window::callback_index_type m_key_callback_index;

  void key_callback(gl::window&, gl::key_token, int, gl::key_action, int);
};

}