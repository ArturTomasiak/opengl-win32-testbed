#pragma once
#include "../includes.h"

void orthographic_matrix(float* matrix, float left_f, float right_f, float bottom_f, float top_f, float near_f, float far_f);
void translate_matrix(float* matrix, float x, float y, float z);
void multiply_f4x4(float* dest, const float* mat1, const float* mat2);