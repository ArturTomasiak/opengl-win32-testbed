#version 330 core

layout(location=0) in vec4 position;
layout(location=1) in vec2 texture_coords;
out vec2 v_texture_coords;
uniform float u_aspect_ratio;
void main() {
    gl_Position = vec4(position.x * u_aspect_ratio, position.y, 0.0, 1.0);
    v_texture_coords = texture_coords;
}