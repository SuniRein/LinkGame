#version 330 core

layout (location = 0) in vec4 pos;
layout (location = 1) in vec4 color;

out vec4 v_color;

uniform mat4 u_MVP;

void main() {
  gl_Position = u_MVP * pos;
  v_color = color;
}