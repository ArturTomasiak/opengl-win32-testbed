#pragma once
#include "../includes.h"

typedef struct {
    uint32_t renderer_id;
} index_buffer_object;

index_buffer_object ibo_create(const void* data, uint32_t size);
void ibo_delete(index_buffer_object* ibo);
void ibo_bind(const index_buffer_object* ibo);
void ibo_unbind();