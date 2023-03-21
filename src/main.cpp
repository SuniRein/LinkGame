#include "openGL_cpp.h"

#include "link_game/link_game.h"

#include <string>

namespace gl = openGL_cpp;

constexpr auto WINDOW_WIDTH = 800;
constexpr auto WINDOW_HEIGHT = 600;

int main(int argc, char* argv[]) {
  int unit_count_x = 40;
  int unit_count_y = 30;
  if(argc == 3) {
    unit_count_x = std::stoi(argv[1]);
    unit_count_y = std::stoi(argv[2]);
  }

  gl::init_glfw(3.3);

  gl::window window(WINDOW_WIDTH, WINDOW_HEIGHT, "LINK GAME");
  window.use();

  gl::load_glad();

  game::link_game link_game(unit_count_x, unit_count_y, window);

  while(!window.should_close()) {
    gl::clear(gl::buffer_bit::color_buffer_bit);

    link_game.on_update();
    link_game.on_render();

    window.poll_events();
    window.swap_buffer();
  }

  return 0;
}
