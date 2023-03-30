#include "link_game.h"

#include <string>

namespace game {

class link_game::command_processer {
public:
  command_processer() = default;
  ~command_processer() = default;

  void run_command(link_game& game, const std::string& command);

  command_processer(const command_processer&) = delete;
  command_processer& operator=(const command_processer&) = delete;

  const char* command_output() const { return m_output.c_str(); }

private:
  std::string m_output;

  void success(const std::string& information);
  void error(const std::string& format);
};

}