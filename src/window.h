#pragma once
#include "includes.h"
#include "vertex/vertex_buffer.h"
#include "vertex/index_buffer.h"
#include "shaders/shaders.h"

int32_t CALLBACK WinMain(HINSTANCE hinstance, HINSTANCE h_prev_instance, LPSTR lp_cmd_line, int32_t n_cmd_show);
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

uint32_t create_shader(const char* vertex_shader, const char* fragment_shader);
uint32_t compile_shader(uint32_t type, const char* source);

void enable_vsync();
void create_window();
HGLRC create_temp_context();
HGLRC create_context();