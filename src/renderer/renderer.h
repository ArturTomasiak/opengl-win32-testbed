#pragma once
#include "../includes.h"
#include "../vertex/vao.h"
#include "../vertex/ibo.h"
#include "../shader/shader.h"

void renderer_draw(const vertex_array_object* vao, const index_buffer_object* ibo);