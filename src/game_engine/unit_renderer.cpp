#include "unit_renderer.h"

#include <memory>
#include <filesystem>

namespace game_engine {

namespace {
  auto generate_indices(int unit_count) {
    constexpr auto INDEX_PER_UNIT = 6;
    constexpr auto VERTEX_PER_UNIT = 4;

    int index_count = unit_count * INDEX_PER_UNIT;

    std::unique_ptr<gl::uint_t[]> indices( new gl::uint_t[index_count] );
    for(int i = 0, offset = 0; i != index_count; i += INDEX_PER_UNIT, offset += VERTEX_PER_UNIT) {
      indices[i + 0] = offset + 0;
      indices[i + 1] = offset + 1;
      indices[i + 2] = offset + 2;
      indices[i + 3] = offset + 2;
      indices[i + 4] = offset + 3;
      indices[i + 5] = offset + 0;
    }
    return indices;
  }

  std::filesystem::path shader_path("../res/shaders");
}

unit_renderer::unit_renderer(gl::uint_t unit_count, const gl::vertex_buffer_layout& layout):
  m_VAO(),
  m_VBO(unit_count * VERTEX_PER_UNIT, layout.get_stride()),
  m_IBO(generate_indices(unit_count).get(), unit_count * INDEX_PER_UNIT),
  m_shader(gl::vertex_shader(shader_path / "vshader.vs"), gl::fragment_shader(shader_path / "fshader.fs"))
{
  m_VAO.add_buffer(m_VBO, layout);
  m_VAO.add_buffer(m_IBO);

  gl::enable_texture_alpha();
}

unit_renderer::~unit_renderer() {}

void unit_renderer::draw(gl::uint_t unit_count) const {
  gl::draw(m_shader, m_VAO, unit_count * INDEX_PER_UNIT);
}

void unit_renderer::update(gl::uint_t unit_pos, const void* const data) {
  auto real_pos = unit_pos * VERTEX_PER_UNIT;
  m_VBO.insert(real_pos, data, VERTEX_PER_UNIT);
}

}