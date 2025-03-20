#pragma once

#include "..\includes.h"

typedef struct {
    uint32_t renderer_id;
} vertex_buffer;

vertex_buffer create_vertex_buffer(const void* data, uint32_t size);
void delete_vertex_buffer(vertex_buffer* vb);
void bind_vertex_buffer(const vertex_buffer* vb);
void unbind_vertex_buffer();