#include "link_game.h"

namespace game {

class link_game::mouse_button_processer {
public:
  mouse_button_processer(link_game& game);
  ~mouse_button_processer();

  mouse_button_processer(const mouse_button_processer&) = delete;
  mouse_button_processer& operator=(const mouse_button_processer&) = delete;

private:
  link_game& m_game;
  gl::window::callback_index_type m_mouse_button_callback_index;

  void mouse_button_callback(gl::window&, gl::mouse_button_token, gl::mouse_button_action, int);
  void active_pos(pos_t pos);
  bool is_linkable(pos_t begin, pos_t end) const;
};

}