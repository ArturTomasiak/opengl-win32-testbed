#pragma once
#include "../includes.h"

typedef struct {
    uint32_t renderer_id;
} index_buffer;

index_buffer create_index_buffer(const void* data, uint32_t size);
void delete_index_buffer(index_buffer* ib);
void bind_index_buffer(const index_buffer* ib);
void unbind_index_buffer();