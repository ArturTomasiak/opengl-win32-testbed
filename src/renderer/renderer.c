#include "renderer.h"

void renderer_draw(const vertex_array_object* vao, const index_buffer_object* ibo) {
    vao_bind(vao);
    ibo_bind(ibo);
    glDrawElements(GL_TRIANGLES, ibo->count, GL_UNSIGNED_INT, 0);
}