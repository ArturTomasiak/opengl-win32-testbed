#pragma once

#define demidebug

#include <GL/glew.h>
#include <GL/wglew.h>
#include <GL/gl.h>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#if defined(min)
#undef min
#endif
#if defined(max)
#undef max
#endif

#include <stdint.h>

// error.c
enum err_type {
    err_allocation_failed,
    err_file_not_found,
    err_glew_initialization,
    err_shader_compilation,
    err_opengl_context
};

void win32_err(enum err_type err);
#ifdef demidebug
#include <stdio.h>
void fatal(uint32_t line, char* file, char* message);
void warning(uint32_t line, char* file, char* message);
void info(uint32_t line, char* file, char* message);
void check_gl_errors();
#endif