#pragma once
#include "../includes.h"

typedef struct {
    int32_t location;
    char* name;    
} shader_uniform_cache;

typedef struct {
    uint32_t renderer_id;
    uint32_t cache_length;
    shader_uniform_cache* cache;
} shader;

shader shader_create(const char* vertex_shader, const char* fragment_shader);
void shader_delete(shader* shader);
void shader_bind(const shader* shader);
void shader_unbind();
void shader_set_uniform4f(shader* shader, char* name, float v0, float v1, float v2, float v3);

// static classes
// char* file_content(const char* location);
// uint32_t shader_compile(uint32_t type, const char* source);
// int32_t get_uniform_location(shader* shader, char* name);