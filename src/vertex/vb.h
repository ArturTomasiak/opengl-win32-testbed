#pragma once

#include "..\includes.h"

typedef struct {
    uint32_t renderer_id;
} vertex_buffer;

vertex_buffer vb_create(const void* data, uint32_t size);
void vb_delete(vertex_buffer* vb);
void vb_bind(const vertex_buffer* vb);
void vb_unbind();