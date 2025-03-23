#include "ibo.h"

index_buffer_object create_ibo(const void* data, uint32_t size) {
    index_buffer_object ibo;
    glGenBuffers(1, &ibo.renderer_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo.renderer_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(uint32_t), data, GL_STATIC_DRAW);
    return ibo;
}

void delete_ibo(index_buffer_object* ibo) {
    glDeleteBuffers(1, &ibo->renderer_id);
    ibo->renderer_id = 0;
}

void bind_ibo(const index_buffer_object* ibo) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo->renderer_id);
}

void unbind_ibo() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}