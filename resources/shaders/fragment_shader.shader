#version 330 core

layout(location=0) out vec4 color;
uniform vec4 u_color;
uniform sampler2D u_texture;
in vec2 v_texture_coords;
void main() {
    vec4 texture_color = texture(u_texture, v_texture_coords);
    color = texture_color; 
}
