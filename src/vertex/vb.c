#include "vb.h"

vertex_buffer vb_create(const void* data, uint32_t size) {
    vertex_buffer vb;
    glGenBuffers(1, &vb.renderer_id);
    glBindBuffer(GL_ARRAY_BUFFER, vb.renderer_id);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    return vb;
}

void vb_delete(vertex_buffer* vb) {
    glDeleteBuffers(1, &vb->renderer_id);
    vb->renderer_id = 0;
}

void vb_bind(const vertex_buffer* vb) {
    glBindBuffer(GL_ARRAY_BUFFER, vb->renderer_id);
}

void vb_unbind() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}