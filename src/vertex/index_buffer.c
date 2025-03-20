#include "index_buffer.h"

index_buffer create_index_buffer(const void* data, uint32_t size) {
    index_buffer ib;
    glGenBuffers(1, &ib.renderer_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib.renderer_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(uint32_t), data, GL_STATIC_DRAW);
    return ib;
}

void delete_index_buffer(index_buffer* ib) {
    glDeleteBuffers(1, &ib->renderer_id);
    ib->renderer_id = 0;
}

void bind_index_buffer(const index_buffer* ib) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib->renderer_id);
}

void unbind_index_buffer() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}