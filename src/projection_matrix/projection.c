#include "projection.h"

// weird near and far names due to mingw having near and far already defined
float* orthographic_matrix(float left, float right, float bottom, float top, float near_f, float far_f) {
    float* matrix = malloc(16 * sizeof(float));
    for (int i = 0; i < 16; i++) matrix[i] = 0.0f;
    matrix[0] = 2.0f / (right - left);
    matrix[5] = 2.0f / (top - bottom);
    matrix[10] = -2.0f / (far_f - near_f);
    matrix[12] = -(right + left) / (right - left);
    matrix[13] = -(top + bottom) / (top - bottom);
    matrix[14] = -(far_f + near_f) / (far_f - near_f);
    matrix[15] = 1.0f;
    return matrix;
}