#pragma once

#include "../includes.h"
#include "../renderer/renderer.h"

typedef struct {
    uint32_t renderer_id;
    const char* file_path;
    uint8_t* local_buffer;
    int32_t width, height, bits_per_pixel; 
} texture;

texture texture_create(const char* file_path);
void texture_delete(texture* texture);
void texture_bind(uint32_t slot, const texture* texture);
void texture_unbind();
