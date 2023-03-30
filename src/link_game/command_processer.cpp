#include "command_processer.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <sstream>
#include <iostream>

namespace game {

void link_game::command_processer::run_command(link_game& game, const std::string& command) {
  static std::stringstream stream;
  stream.clear();
  stream << command;

  if(std::string word; stream >> word) {
    if(word == "init") {
      if(int x, y; stream >> x >> y) {
        ImGui_ImplGlfw_RestoreCallbacks(game.m_window->gl_window());

        game.init(x, y, *game.m_window);
        game.on_update();
        game.on_render();
        game.m_game_state = link_game::game_state_t::waiting;
        
        ImGui_ImplGlfw_InstallCallbacks(game.m_window->gl_window());

        success("Successfully init.");
      } else {
        error("init <x_count> <y_count>");
      }
    }
    
    else if(word == "quit") {
      game.m_window->set_should_close(true);
      game.release();
    }

    else if(word == "continue") {
      game.m_game_state = link_game::game_state_t::running;
    }
    
    else {
      error("");
    }
  }
}

void link_game::command_processer::success(const std::string& information) {
  m_output = information;
}

void link_game::command_processer::error(const std::string& format) {
  static std::stringstream stream;
  stream.clear();
  stream << "Wrong command format!!!\n"
         << "The right format is: " << format;
  m_output = stream.rdbuf()->str();
}

}