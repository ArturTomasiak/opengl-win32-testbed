#include "texture.h"
#include <png.h>

texture texture_create(const char* file_path) {
    texture texture = {0};
    texture.file_path = file_path;
    FILE* file_pointer = fopen(file_path, "rb");
    if (!file_pointer) {
        win32_err(err_file_not_found);
        return texture;
    }
    png_byte header[8];
    fread(header, 1, 8, file_pointer);
    if (png_sig_cmp(header, 0, 8)) {
        win32_err(err_file_not_png);
        return texture;
    }
    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    png_infop info = png_create_info_struct(png);
    if (!png || !info) {
        warning(__LINE__, __FILE__, "png.h create struct functions failed");
        fclose(file_pointer);
        return texture;
    }
    if (setjmp(png_jmpbuf(png))) {
        png_destroy_read_struct(&png, &info, NULL);
        fclose(file_pointer);
        return texture;
    }
    png_init_io(png, file_pointer);
    png_set_sig_bytes(png, 8);
    png_read_info(png, info);
    int width = png_get_image_width(png, info);
    int height = png_get_image_height(png, info);
    png_byte color_type = png_get_color_type(png, info);
    png_byte bit_depth = png_get_bit_depth(png, info);
    if (bit_depth == 16) png_set_strip_16(png);
    if (color_type == PNG_COLOR_TYPE_PALETTE) png_set_palette_to_rgb(png);
    if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8) png_set_expand_gray_1_2_4_to_8(png);
    if (png_get_valid(png, info, PNG_INFO_tRNS)) png_set_tRNS_to_alpha(png);
    if (color_type == PNG_COLOR_TYPE_RGB || color_type == PNG_COLOR_TYPE_GRAY)
        png_set_filler(png, 0xFF, PNG_FILLER_AFTER);
    if (color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
        png_set_gray_to_rgb(png);
    png_read_update_info(png, info);
    int row_bytes = png_get_rowbytes(png, info);
    png_bytep buffer = (png_bytep)malloc(row_bytes * height);
    png_bytep* row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * height);
    for (int y = 0; y < height; y++) {
        row_pointers[y] = buffer + (height - 1 - y) * row_bytes;
    }
    png_read_image(png, row_pointers);
    fclose(file_pointer);
    png_destroy_read_struct(&png, &info, NULL);
    free(row_pointers);
    texture.width = width;
    texture.height = height;
    texture.bits_per_pixel = 32;
    texture.local_buffer = buffer;
    glGenTextures(1, &texture.renderer_id);
    glBindTexture(GL_TEXTURE_2D, texture.renderer_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, texture.width, texture.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
    glBindTexture(GL_TEXTURE_2D, 0);
    if (buffer)
        free(buffer);
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