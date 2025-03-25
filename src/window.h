#pragma once
#include "includes.h"
#include "vertex/ibo.h"
#include "vertex/vao.h"
#include "shader/shader.h"
#include "texture/texture.h"
#include "renderer/renderer.h"

int32_t CALLBACK WinMain(HINSTANCE hinstance, HINSTANCE h_prev_instance, LPSTR lp_cmd_line, int32_t n_cmd_show);
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

uint32_t create_shader(const char* vertex_shader, const char* fragment_shader);
uint32_t compile_shader(uint32_t type, const char* source);


// static classes
// void unbind_all()
// void enable_vsync();
// void create_window();
// HGLRC create_temp_context();
// HGLRC create_context();