#include "texture.h"
#include "../dependencies/stb_image.h"

texture texture_create(const char* file_path) {
    texture texture = {0};
    texture.file_path = file_path;
    stbi_set_flip_vertically_on_load(1);
    texture.local_buffer = stbi_load(file_path, &texture.width, &texture.height, &texture.bits_per_pixel, 4);
    glGenTextures(1, &texture.renderer_id);
    glBindTexture(GL_TEXTURE_2D, texture.renderer_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, texture.width, texture.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture.local_buffer);
    glBindTexture(GL_TEXTURE_2D, 0);
    if (texture.local_buffer)
        stbi_image_free(texture.local_buffer);
    return texture;
}

void texture_delete(texture* texture) {
    glDeleteTextures(1, &texture->renderer_id);
    texture->renderer_id = 0;
}

void texture_bind(uint32_t slot, const texture* texture) {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, texture->renderer_id);
}

void texture_unbind() { 
    glBindTexture(GL_TEXTURE_2D, 0); 
}