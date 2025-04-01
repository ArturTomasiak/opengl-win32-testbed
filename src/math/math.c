#include "math.h"

// weird variable names as to not collide with existing defines
void orthographic_matrix(float* matrix, float left_f, float right_f, float bottom_f, float top_f, float near_f, float far_f) {
    matrix[0] = 2.0f / (right_f - left_f);
    matrix[5] = 2.0f / (top_f - bottom_f);
    matrix[10] = -2.0f / (far_f - near_f);
    matrix[12] = -(right_f + left_f) / (right_f - left_f);
    matrix[13] = -(top_f + bottom_f) / (top_f - bottom_f);
    matrix[14] = -(far_f + near_f) / (far_f - near_f);
    matrix[15] = 1.0f;
}

void translate_matrix(float* matrix, float x, float y, float z) {
    matrix[0] = 1.0f;
    matrix[5] = 1.0f;
    matrix[10] = 1.0f;
    matrix[15] = 1.0f;
    matrix[12] = x;
    matrix[13] = y;
    matrix[14] = z;
}

void multiply_f4x4(float* dest, const float* mat1, const float* mat2) {
    for (int col = 0; col < 4; col++) {
        for (int row = 0; row < 4; row++) {
            dest[col * 4 + row] =
                mat1[0 * 4 + row] * mat2[col * 4 + 0] +
                mat1[1 * 4 + row] * mat2[col * 4 + 1] +
                mat1[2 * 4 + row] * mat2[col * 4 + 2] +
                mat1[3 * 4 + row] * mat2[col * 4 + 3];
        }
    }
}