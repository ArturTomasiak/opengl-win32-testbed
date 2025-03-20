#include "vertex_buffer.h"

vertex_buffer create_vertex_buffer(const void* data, uint32_t size) {
    vertex_buffer vb;
    glGenBuffers(1, &vb.renderer_id);
    glBindBuffer(GL_ARRAY_BUFFER, vb.renderer_id);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    return vb;
}

void delete_vertex_buffer(vertex_buffer* vb) {
    glDeleteBuffers(1, &vb->renderer_id);
    vb->renderer_id = 0;
}

void bind_vertex_buffer(const vertex_buffer* vb) {
    glBindBuffer(GL_ARRAY_BUFFER, vb->renderer_id);
}

void unbind_vertex_buffer() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}