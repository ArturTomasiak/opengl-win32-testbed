#pragma once
#include "../includes.h"

// static char* file_content(const char* location);
// static uint32_t compile_shader(uint32_t type, const char* source);
uint32_t create_shader(const char* vertex_shader, const char* fragment_shader);