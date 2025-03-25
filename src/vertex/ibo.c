#include "ibo.h"

index_buffer_object ibo_create(const void* data, uint32_t size) {
    index_buffer_object ibo;
    ibo.count = size;
    glGenBuffers(1, &ibo.renderer_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo.renderer_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * sizeof(uint32_t), data, GL_STATIC_DRAW);
    return ibo;
}

void ibo_delete(index_buffer_object* ibo) {
    glDeleteBuffers(1, &ibo->renderer_id);
    ibo->renderer_id = 0;
}

void ibo_bind(const index_buffer_object* ibo) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo->renderer_id);
}

void ibo_unbind() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}