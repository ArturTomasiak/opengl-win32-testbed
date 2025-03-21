#pragma once
#include "../includes.h"
#include "vertex_buffer.h"

typedef struct {
    uint32_t count;
    uint32_t type;
    _Bool normalized;
} layout_element;

typedef struct {
    layout_element* elements;
    uint32_t stride;
} vertex_buffer_layout;

void add_buffer(const vertex_buffer vb, const vertex_buffer_layout layout);