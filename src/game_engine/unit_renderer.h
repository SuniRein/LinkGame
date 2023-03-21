#ifndef GAME_ENGINE_UNIT_RENDERER_H_
#define GAME_ENGINE_UNIT_RENDERER_H_

#include "openGL_cpp.h"

namespace game_engine {

namespace gl = openGL_cpp;

class unit_renderer {
public:
  unit_renderer() = delete;
  unit_renderer(gl::uint_t unit_count, const gl::vertex_buffer_layout& layout);

  unit_renderer(const unit_renderer&) = delete;
  unit_renderer& operator=(const unit_renderer&) = delete;

  ~unit_renderer();

  void draw(gl::uint_t unit_count) const;
  void update(gl::uint_t unit_pos, const void* const data);

  template<typename T, typename... Args>
  void set_uniform(const std::string& name, const T& value, Args... args) {
    m_shader.set_uniform(name, value, args...);
  }
  
private:
  gl::vertex_array m_VAO;
  gl::dynamic_vertex_buffer m_VBO;
  gl::index_buffer m_IBO;
  gl::program m_shader;

  static constexpr int VERTEX_PER_UNIT = 4;
  static constexpr int INDEX_PER_UNIT = 6;
};

}

#endif