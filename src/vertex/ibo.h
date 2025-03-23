#pragma once
#include "../includes.h"

typedef struct {
    uint32_t renderer_id;
} index_buffer_object;

index_buffer_object create_ibo(const void* data, uint32_t size);
void delete_ibo(index_buffer_object* ibo);
void bind_ibo(const index_buffer_object* ibo);
void unbind_ibo();