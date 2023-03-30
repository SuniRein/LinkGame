#include "openGL_cpp.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

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

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();

  ImGui::StyleColorsDark();

  game::link_game link_game(unit_count_x, unit_count_y, window);

  // 需要放到所有注册 gl::window 回调函数操作的后面
  ImGui_ImplGlfw_InitForOpenGL(window.gl_window(), true);
  ImGui_ImplOpenGL3_Init("#version 130");

  while(!window.should_close()) {
    gl::clear(gl::buffer_bit::color_buffer_bit);    
    
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    link_game.on_update();
    link_game.on_render();
    link_game.on_ImGui_render();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    window.poll_events();
    window.swap_buffer();
  }

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  return 0;
}
