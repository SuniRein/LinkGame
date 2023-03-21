#ifndef LINK_GAME_UNIT_H_
#define LINK_GAME_UNIT_H_

#include "openGL_cpp.h"

namespace game_engine {

namespace gl = openGL_cpp;

class unit {
public:
  unit(gl::vec4 color);

  unit(const unit&) = default;
  unit& operator=(const unit&) = default;

  auto get_color() const noexcept { return m_color; }
  void set_color(gl::vec4 color) noexcept { m_color = color; }

private:
  gl::vec4 m_color;
};

}

#endif