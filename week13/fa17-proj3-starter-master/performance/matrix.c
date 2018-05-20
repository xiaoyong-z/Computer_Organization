#include "matrix.h"


int get_rows(matrix *mat) {
    return mat->dim.rows;
}

int get_cols(matrix *mat) {
    return mat->dim.cols;
}

void matrix_power(matrix *mat, int power, matrix *dst) {
    assert(mat != dst && same_size(mat, dst) && mat->dim.rows == mat->dim.cols);
    if (power == 1) {
        copy(mat, dst);
        return;
    }
    if (power == 2) {
        matrix_multiply(mat, mat, dst);
        return;
    }

//    matrix *intermediate;
//    eye(&intermediate, dst->dim);
//    copy(intermediate, dst);
//    for (int i = 0; i < pow; i++) {
//        matrix_multiply(intermediate, mat, dst);
//        copy(dst, intermediate);
//    }
//    free_matrix(intermediate);
    matrix *intermediate;
    matrix **inte = &intermediate;
    allocate_matrix(inte, mat->dim.rows, mat->dim.cols);
    copy(mat, intermediate);
    int temp = power;
    int i, j, k;
    while (temp > 2) {
        k = 1, i = 0, j = 0;
        while (k < temp) {
            k *= 2;
            i++;
        }
        temp = temp - (2 ^ k);
        while (j < i) {
            matrix_multiply(intermediate, intermediate, dst);
            copy(dst, intermediate);
            j++;
        }
    }
    for (; j < temp; j++) {
        matrix_multiply(intermediate, mat, dst);
        copy(dst, intermediate);
    }
    free_matrix(intermediate);
}

void set_loc(matrix *mat, int row, int col, float val) {
    assert(row < mat->dim.rows && col < mat->dim.cols && row >= 0 && col >= 0);
    mat->data[row][col] = val;
}

int same_size(matrix *mat1, matrix *mat2) {
    return mat1 && mat2 && mat1->dim.rows == mat2->dim.rows && mat1->dim.cols == mat2->dim.cols;
}

float get_loc(matrix *mat, int row, int col) {
    return mat->data[row][col];
}

/*
    called like:
    ```c
    matrix mat;
    allocate_matrix(&mat, 1, 2);
    ```
*/
int allocate_matrix(matrix **mat, int rows, int cols) {
    *mat = malloc(sizeof(matrix));
    (*mat)->dim.rows = rows;
    (*mat)->dim.cols = cols;
    (*mat)->data = malloc(sizeof(float *) * rows);
    for (int i = 0; i < rows; i += 1) {
        (*mat)->data[i] = calloc(cols, sizeof(float));
    }
    return 0;
}


int allocate_matrix_s(matrix **mat, shape s) {
    return allocate_matrix(mat, s.rows, s.cols);
}

void free_matrix(matrix *mat) {
    for (int i = 0; i < mat->dim.rows; i += 1) {
        free(mat->data[i]);
    }
    free(mat->data);
    free(mat);
}


int eye(matrix **mat, shape s) {
    assert(allocate_matrix_s(mat, s) == 0);
    // Make the result an identity matrix
    int i;
#pragma omp parallel for simd
    for (i = 0; i < s.rows / 4 * 4; i += 4) {
        (*mat)->data[i][i] = 1;
        (*mat)->data[i + 1][i + 1] = 1;
        (*mat)->data[i + 2][i + 2] = 1;
        (*mat)->data[i + 3][i + 3] = 1;
    }
#pragma omp parallel for simd
    for (i = s.rows / 4 * 4; i < s.rows; i++) {
        (*mat)->data[i][i] = 1;
    }
    return 0;
}


void dot_product(matrix *vec1, matrix *vec2, float *result) {
    assert(same_size(vec1, vec2) && vec1->dim.cols == 1);
    float global_sum = 0.0;
    int i;
#pragma omp parallel for simd reduction(+: global_sum)
    for (i = 0; i < vec1->dim.rows; i++) {
        global_sum += vec1->data[i][0] * vec2->data[i][0];
    }
    *result = global_sum;
}

void outer_product(matrix *vec1, matrix *vec2, matrix *dst) {
    assert(vec1->dim.cols == 1 && vec2->dim.cols == 1 && vec1->dim.rows == dst->dim.rows &&
           vec2->dim.rows == dst->dim.cols);
    int i, j;
#pragma omp parallel for simd private(j)
    for (i = 0; i < vec1->dim.rows; i++) {
        for (j = 0; j + 3 < vec2->dim.rows; j += 4) {
            dst->data[i][j] = vec1->data[i][0] * vec2->data[j][0];
            dst->data[i][j + 1] = vec1->data[i][0] * vec2->data[j + 1][0];
            dst->data[i][j + 2] = vec1->data[i][0] * vec2->data[j + 2][0];
            dst->data[i][j + 3] = vec1->data[i][0] * vec2->data[j + 3][0];
        }

        for (; j < vec2->dim.rows; j++) {
            dst->data[i][j] = vec1->data[i][0] * vec2->data[j][0];
        }
    }
}

void matrix_scale(matrix *mat, float scalar, matrix *dst) {
    assert(same_size(mat, dst));

    __m256 ymm0, ymm1, ymm2, ymm3, ymm4, ymm5, ymm6, ymm7,
            ymm8, ymm9, ymm10, ymm11, ymm12, ymm13, ymm14, ymm15;
    int i, j;
    for (i = 0; i < mat->dim.rows; i++) {
        for (j = 0; j + 64 < mat->dim.cols; j += 64) {
            ymm0 = __builtin_ia32_loadups256(&mat->data[i][j]);
            ymm1 = __builtin_ia32_loadups256(&mat->data[i][j + 8]);
            ymm2 = __builtin_ia32_loadups256(&mat->data[i][j + 16]);
            ymm3 = __builtin_ia32_loadups256(&mat->data[i][j + 24]);
            ymm4 = __builtin_ia32_loadups256(&mat->data[i][j + 32]);
            ymm5 = __builtin_ia32_loadups256(&mat->data[i][j + 40]);
            ymm6 = __builtin_ia32_loadups256(&mat->data[i][j + 48]);
            ymm7 = __builtin_ia32_loadups256(&mat->data[i][j + 56]);

            ymm8 = _mm256_set1_ps(scalar);
            ymm9 = _mm256_set1_ps(scalar);
            ymm10 = _mm256_set1_ps(scalar);
            ymm11 = _mm256_set1_ps(scalar);
            ymm12 = _mm256_set1_ps(scalar);
            ymm13 = _mm256_set1_ps(scalar);
            ymm14 = _mm256_set1_ps(scalar);
            ymm15 = _mm256_set1_ps(scalar);

            ymm0 = __builtin_ia32_mulps256(ymm0, ymm8);
            ymm1 = __builtin_ia32_mulps256(ymm1, ymm9);
            ymm2 = __builtin_ia32_mulps256(ymm2, ymm10);
            ymm3 = __builtin_ia32_mulps256(ymm3, ymm11);
            ymm4 = __builtin_ia32_mulps256(ymm4, ymm12);
            ymm5 = __builtin_ia32_mulps256(ymm5, ymm13);
            ymm6 = __builtin_ia32_mulps256(ymm6, ymm14);
            ymm7 = __builtin_ia32_mulps256(ymm7, ymm15);

            __builtin_ia32_storeups256(&dst->data[i][j], ymm0);
            __builtin_ia32_storeups256(&dst->data[i][j + 8], ymm1);
            __builtin_ia32_storeups256(&dst->data[i][j + 16], ymm2);
            __builtin_ia32_storeups256(&dst->data[i][j + 24], ymm3);
            __builtin_ia32_storeups256(&dst->data[i][j + 32], ymm4);
            __builtin_ia32_storeups256(&dst->data[i][j + 40], ymm5);
            __builtin_ia32_storeups256(&dst->data[i][j + 48], ymm6);
            __builtin_ia32_storeups256(&dst->data[i][j + 56], ymm7);
        }
        for (; j + 32 < mat->dim.cols; j += 32) {
            ymm8 = _mm256_set1_ps(scalar);
            ymm9 = _mm256_set1_ps(scalar);
            ymm10 = _mm256_set1_ps(scalar);
            ymm11 = _mm256_set1_ps(scalar);

            ymm0 = __builtin_ia32_loadups256(&mat->data[i][j]);
            ymm1 = __builtin_ia32_loadups256(&mat->data[i][j + 8]);
            ymm2 = __builtin_ia32_loadups256(&mat->data[i][j + 16]);
            ymm3 = __builtin_ia32_loadups256(&mat->data[i][j + 24]);

            ymm0 = __builtin_ia32_mulps256(ymm0, ymm8);
            ymm1 = __builtin_ia32_mulps256(ymm1, ymm9);
            ymm2 = __builtin_ia32_mulps256(ymm2, ymm10);
            ymm3 = __builtin_ia32_mulps256(ymm3, ymm11);

            __builtin_ia32_storeups256(&dst->data[i][j], ymm0);
            __builtin_ia32_storeups256(&dst->data[i][j + 8], ymm1);
            __builtin_ia32_storeups256(&dst->data[i][j + 16], ymm2);
            __builtin_ia32_storeups256(&dst->data[i][j + 24], ymm3);
        }
        for (; j < mat->dim.cols; j++) {
            dst->data[i][j] = scalar * mat->data[i][j];
        }
    }
}

void matrix_multiply_elementwise(matrix *mat1, matrix *mat2, matrix *dst) {
    assert(same_size(mat1, mat2) && same_size(mat1, dst));
    __m256 ymm0, ymm1, ymm2, ymm3, ymm4, ymm5, ymm6, ymm7,
            ymm8, ymm9, ymm10, ymm11, ymm12, ymm13, ymm14, ymm15;
    int i, j;
    for (i = 0; i < dst->dim.rows; i++) {
        for (j = 0; j + 64 < dst->dim.cols; j += 64) {
            ymm0 = __builtin_ia32_loadups256(&mat1->data[i][j]);
            ymm1 = __builtin_ia32_loadups256(&mat1->data[i][j + 8]);
            ymm2 = __builtin_ia32_loadups256(&mat1->data[i][j + 16]);
            ymm3 = __builtin_ia32_loadups256(&mat1->data[i][j + 24]);
            ymm4 = __builtin_ia32_loadups256(&mat1->data[i][j + 32]);
            ymm5 = __builtin_ia32_loadups256(&mat1->data[i][j + 40]);
            ymm6 = __builtin_ia32_loadups256(&mat1->data[i][j + 48]);
            ymm7 = __builtin_ia32_loadups256(&mat1->data[i][j + 56]);


            ymm8 = __builtin_ia32_loadups256(&mat2->data[i][j]);
            ymm9 = __builtin_ia32_loadups256(&mat2->data[i][j + 8]);
            ymm10 = __builtin_ia32_loadups256(&mat2->data[i][j + 16]);
            ymm11 = __builtin_ia32_loadups256(&mat2->data[i][j + 24]);
            ymm12 = __builtin_ia32_loadups256(&mat2->data[i][j + 32]);
            ymm13 = __builtin_ia32_loadups256(&mat2->data[i][j + 40]);
            ymm14 = __builtin_ia32_loadups256(&mat2->data[i][j + 48]);
            ymm15 = __builtin_ia32_loadups256(&mat2->data[i][j + 56]);

            ymm0 = __builtin_ia32_mulps256(ymm0, ymm8);
            ymm1 = __builtin_ia32_mulps256(ymm1, ymm9);
            ymm2 = __builtin_ia32_mulps256(ymm2, ymm10);
            ymm3 = __builtin_ia32_mulps256(ymm3, ymm11);
            ymm4 = __builtin_ia32_mulps256(ymm4, ymm12);
            ymm5 = __builtin_ia32_mulps256(ymm5, ymm13);
            ymm6 = __builtin_ia32_mulps256(ymm6, ymm14);
            ymm7 = __builtin_ia32_mulps256(ymm7, ymm15);

            __builtin_ia32_storeups256(&dst->data[i][j], ymm0);
            __builtin_ia32_storeups256(&dst->data[i][j + 8], ymm1);
            __builtin_ia32_storeups256(&dst->data[i][j + 16], ymm2);
            __builtin_ia32_storeups256(&dst->data[i][j + 24], ymm3);
            __builtin_ia32_storeups256(&dst->data[i][j + 32], ymm4);
            __builtin_ia32_storeups256(&dst->data[i][j + 40], ymm5);
            __builtin_ia32_storeups256(&dst->data[i][j + 48], ymm6);
            __builtin_ia32_storeups256(&dst->data[i][j + 56], ymm7);
        }
        for (; j + 32 < dst->dim.cols; j += 32) {
            ymm8 = __builtin_ia32_loadups256(&mat2->data[i][j]);
            ymm9 = __builtin_ia32_loadups256(&mat2->data[i][j + 8]);
            ymm10 = __builtin_ia32_loadups256(&mat2->data[i][j + 16]);
            ymm11 = __builtin_ia32_loadups256(&mat2->data[i][j + 24]);

            ymm0 = __builtin_ia32_loadups256(&mat1->data[i][j]);
            ymm1 = __builtin_ia32_loadups256(&mat1->data[i][j + 8]);
            ymm2 = __builtin_ia32_loadups256(&mat1->data[i][j + 16]);
            ymm3 = __builtin_ia32_loadups256(&mat1->data[i][j + 24]);

            ymm0 = __builtin_ia32_mulps256(ymm0, ymm8);
            ymm1 = __builtin_ia32_mulps256(ymm1, ymm9);
            ymm2 = __builtin_ia32_mulps256(ymm2, ymm10);
            ymm3 = __builtin_ia32_mulps256(ymm3, ymm11);

            __builtin_ia32_storeups256(&dst->data[i][j], ymm0);
            __builtin_ia32_storeups256(&dst->data[i][j + 4], ymm1);
            __builtin_ia32_storeups256(&dst->data[i][j + 8], ymm2);
            __builtin_ia32_storeups256(&dst->data[i][j + 12], ymm3);
        }
        for (; j < dst->dim.cols; j++) {
            dst->data[i][j] = mat1->data[i][j] * mat2->data[i][j];
        }
    }
}

void matrix_add(matrix *mat1, matrix *mat2, matrix *dst) {
    assert(same_size(mat1, mat2) && same_size(mat1, dst));
    __m256 ymm0, ymm1, ymm2, ymm3, ymm4, ymm5, ymm6, ymm7,
            ymm8, ymm9, ymm10, ymm11, ymm12, ymm13, ymm14, ymm15;
    int i, j;
    for (i = 0; i < dst->dim.rows; i++) {
        for (j = 0; j + 64 < dst->dim.cols; j += 64) {
            ymm0 = __builtin_ia32_loadups256(&mat1->data[i][j]);
            ymm1 = __builtin_ia32_loadups256(&mat1->data[i][j + 8]);
            ymm2 = __builtin_ia32_loadups256(&mat1->data[i][j + 16]);
            ymm3 = __builtin_ia32_loadups256(&mat1->data[i][j + 24]);
            ymm4 = __builtin_ia32_loadups256(&mat1->data[i][j + 32]);
            ymm5 = __builtin_ia32_loadups256(&mat1->data[i][j + 40]);
            ymm6 = __builtin_ia32_loadups256(&mat1->data[i][j + 48]);
            ymm7 = __builtin_ia32_loadups256(&mat1->data[i][j + 56]);


            ymm8 = __builtin_ia32_loadups256(&mat2->data[i][j]);
            ymm9 = __builtin_ia32_loadups256(&mat2->data[i][j + 8]);
            ymm10 = __builtin_ia32_loadups256(&mat2->data[i][j + 16]);
            ymm11 = __builtin_ia32_loadups256(&mat2->data[i][j + 24]);
            ymm12 = __builtin_ia32_loadups256(&mat2->data[i][j + 32]);
            ymm13 = __builtin_ia32_loadups256(&mat2->data[i][j + 40]);
            ymm14 = __builtin_ia32_loadups256(&mat2->data[i][j + 48]);
            ymm15 = __builtin_ia32_loadups256(&mat2->data[i][j + 56]);

            ymm0 = __builtin_ia32_addps256(ymm0, ymm8);
            ymm1 = __builtin_ia32_addps256(ymm1, ymm9);
            ymm2 = __builtin_ia32_addps256(ymm2, ymm10);
            ymm3 = __builtin_ia32_addps256(ymm3, ymm11);
            ymm4 = __builtin_ia32_addps256(ymm4, ymm12);
            ymm5 = __builtin_ia32_addps256(ymm5, ymm13);
            ymm6 = __builtin_ia32_addps256(ymm6, ymm14);
            ymm7 = __builtin_ia32_addps256(ymm7, ymm15);

            __builtin_ia32_storeups256(&dst->data[i][j], ymm0);
            __builtin_ia32_storeups256(&dst->data[i][j + 8], ymm1);
            __builtin_ia32_storeups256(&dst->data[i][j + 16], ymm2);
            __builtin_ia32_storeups256(&dst->data[i][j + 24], ymm3);
            __builtin_ia32_storeups256(&dst->data[i][j + 32], ymm4);
            __builtin_ia32_storeups256(&dst->data[i][j + 40], ymm5);
            __builtin_ia32_storeups256(&dst->data[i][j + 48], ymm6);
            __builtin_ia32_storeups256(&dst->data[i][j + 56], ymm7);
        }
        for (; j + 32 < dst->dim.cols; j += 32) {
            ymm8 = __builtin_ia32_loadups256(&mat2->data[i][j]);
            ymm9 = __builtin_ia32_loadups256(&mat2->data[i][j + 8]);
            ymm10 = __builtin_ia32_loadups256(&mat2->data[i][j + 16]);
            ymm11 = __builtin_ia32_loadups256(&mat2->data[i][j + 24]);

            ymm0 = __builtin_ia32_loadups256(&mat1->data[i][j]);
            ymm1 = __builtin_ia32_loadups256(&mat1->data[i][j + 8]);
            ymm2 = __builtin_ia32_loadups256(&mat1->data[i][j + 16]);
            ymm3 = __builtin_ia32_loadups256(&mat1->data[i][j + 24]);

            ymm0 = __builtin_ia32_addps256(ymm0, ymm8);
            ymm1 = __builtin_ia32_addps256(ymm1, ymm9);
            ymm2 = __builtin_ia32_addps256(ymm2, ymm10);
            ymm3 = __builtin_ia32_addps256(ymm3, ymm11);

            __builtin_ia32_storeups256(&dst->data[i][j], ymm0);
            __builtin_ia32_storeups256(&dst->data[i][j + 8], ymm1);
            __builtin_ia32_storeups256(&dst->data[i][j + 16], ymm2);
            __builtin_ia32_storeups256(&dst->data[i][j + 24], ymm3);
        }
        for (; j < dst->dim.cols; j++) {
            dst->data[i][j] = mat1->data[i][j] + mat2->data[i][j];
        }
    }
}

void matrix_transpose(matrix *m, matrix *dst) {
    assert(m->dim.rows == dst->dim.cols && m->dim.cols == dst->dim.rows);
    int i, j;
#pragma omp parallel for simd private(j)
    for (i = 0; i < dst->dim.rows; i++) {
        for (j = 0; j + 3 < dst->dim.cols; j += 4) {
            dst->data[i][j] = m->data[j][i];
            dst->data[i][j + 1] = m->data[j + 1][i];
            dst->data[i][j + 2] = m->data[j + 2][i];
            dst->data[i][j + 3] = m->data[j + 3][i];
        }

        for (; j < dst->dim.cols; j++) {
            dst->data[i][j] = m->data[j][i];
        }
    }
}

void copy(matrix *src, matrix *dst) {
    assert(same_size(src, dst));
    int i, j;
#pragma omp parallel for simd private(j)
    for (i = 0; i < src->dim.rows; i++) {
        for (j = 0; j + 3 < src->dim.cols; j += 4) {
            dst->data[i][j] = src->data[i][j];
            dst->data[i][j + 1] = src->data[i][j + 1];
            dst->data[i][j + 2] = src->data[i][j + 2];
            dst->data[i][j + 3] = src->data[i][j + 3];
        }
        for (j = 0; j < src->dim.cols; j++) {
            dst->data[i][j] = src->data[i][j];
        }
    }
}


void get_matrix_as_array(float *arr, matrix *mat) {
    int i, j, k = 0;
    for (i = 0; i < mat->dim.rows; i++) {
        for (j = 0; j < mat->dim.cols; j++) {
            arr[k] = mat->data[i][j];
            k++;
        }
    }
}

void get_matrix_as_array2(float *arr, matrix *mat) {
    int i, j, k = 0;
    for (j = 0; j < mat->dim.cols; j++) {
        for (i = 0; i < mat->dim.rows; i++) {
            arr[k] = mat->data[i][j];
            k++;
        }
    }
}

matrix *arr_to_matrix(float *arr, int rows, int cols) {
    matrix *m;
    allocate_matrix(&m, rows, cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            set_loc(m, i, j, arr[i * cols + j]);
        }
    }
    return m;
}

void apply_func(matrix *mat, matrix *dst, float (*f)(float)) {
    assert(same_size(mat, dst));
    int i, j;
#pragma omp parallel for simd private(j)
    for (i = 0; i < mat->dim.rows; i++) {
        for (j = 0; j < mat->dim.cols; j++) {
            dst->data[i][j] = f(mat->data[i][j]);
        }
    }
}




void matrix_multiply(matrix *mat1, matrix *mat2, matrix *dst) {
    // version1:
    assert(mat1->dim.cols == mat2->dim.rows && dst->dim.rows == mat1->dim.rows && dst->dim.cols == mat2->dim.cols);
    __m256 ymm0, ymm1, ymm2, ymm3, ymm4, ymm5, ymm6, ymm7,
            ymm8, ymm9, ymm10, ymm11, ymm12, ymm13, ymm14, ymm15;
    int length = (mat2->dim.rows) * (mat2->dim.cols);
//    for (int i = 0; i < mat2 -> dim.rows; i++ ) {
//        for (int j = 0; j < mat2->dim.cols; j++) {
//            printf("%f ", mat2->data[i][j]);
//        }
//        printf("\n");
//    }
    float *arraytemp = (float *) malloc(length * sizeof(float));

    if (arraytemp == NULL)
        printf("\nfalse\n");
    get_matrix_as_array2(arraytemp, mat2);
    int i = 0, j, k, z = 0;
//    while (i < length)
//        printf("arraytemp[%d] = %f\n", i, arraytemp[i++]);

//    float scratchpad[8];
    float scratchpad[8] __attribute__((aligned(16)));
    for (i = 0; i < mat1->dim.rows; i++) {
        for (j = 0; j < mat2->dim.cols; j++) {

            float sum = 0.0;
            for (k = 0; k + 64 < mat1->dim.cols; k += 64, z += 64) {
                ymm0 = __builtin_ia32_loadups256(&mat1->data[i][k]);
                ymm1 = __builtin_ia32_loadups256(&mat1->data[i][k + 8]);
                ymm2 = __builtin_ia32_loadups256(&mat1->data[i][k + 16]);
                ymm3 = __builtin_ia32_loadups256(&mat1->data[i][k + 24]);
                ymm4 = __builtin_ia32_loadups256(&mat1->data[i][k + 32]);
                ymm5 = __builtin_ia32_loadups256(&mat1->data[i][k + 40]);
                ymm6 = __builtin_ia32_loadups256(&mat1->data[i][k + 48]);
                ymm7 = __builtin_ia32_loadups256(&mat1->data[i][k + 56]);

                ymm8 = __builtin_ia32_loadups256(&arraytemp[z]);
                ymm9 = __builtin_ia32_loadups256(&arraytemp[z + 8]);
                ymm10 = __builtin_ia32_loadups256(&arraytemp[z + 16]);
                ymm11 = __builtin_ia32_loadups256(&arraytemp[z + 24]);
                ymm12 = __builtin_ia32_loadups256(&arraytemp[z + 32]);
                ymm13 = __builtin_ia32_loadups256(&arraytemp[z + 40]);
                ymm14 = __builtin_ia32_loadups256(&arraytemp[z + 48]);
                ymm15 = __builtin_ia32_loadups256(&arraytemp[z + 56]);

                ymm0 = __builtin_ia32_mulps256(ymm0, ymm8);
                ymm1 = __builtin_ia32_mulps256(ymm1, ymm9);
                ymm2 = __builtin_ia32_mulps256(ymm2, ymm10);
                ymm3 = __builtin_ia32_mulps256(ymm3, ymm11);
                ymm4 = __builtin_ia32_mulps256(ymm4, ymm12);
                ymm5 = __builtin_ia32_mulps256(ymm5, ymm13);
                ymm6 = __builtin_ia32_mulps256(ymm6, ymm14);
                ymm7 = __builtin_ia32_mulps256(ymm7, ymm15);

                ymm0 = __builtin_ia32_addps256(ymm0, ymm1);
                ymm2 = __builtin_ia32_addps256(ymm2, ymm3);
                ymm4 = __builtin_ia32_addps256(ymm4, ymm5);
                ymm6 = __builtin_ia32_addps256(ymm6, ymm7);
                ymm0 = __builtin_ia32_addps256(ymm0, ymm2);
                ymm4 = __builtin_ia32_addps256(ymm4, ymm6);
                ymm0 = __builtin_ia32_addps256(ymm0, ymm4);


                __builtin_ia32_storeups256(scratchpad, ymm0);
                for (int m = 0; m < 8; m++)
                    sum += scratchpad[m];
            }
            for (; k + 32 < mat1->dim.cols; k += 32, z += 32) {
                ymm8 = __builtin_ia32_loadups256(&arraytemp[z]);
                ymm9 = __builtin_ia32_loadups256(&arraytemp[z + 8]);
                ymm10 = __builtin_ia32_loadups256(&arraytemp[z + 16]);
                ymm11 = __builtin_ia32_loadups256(&arraytemp[z + 24]);

                ymm0 = __builtin_ia32_loadups256(&mat1->data[i][k]);
                ymm1 = __builtin_ia32_loadups256(&mat1->data[i][k + 8]);
                ymm2 = __builtin_ia32_loadups256(&mat1->data[i][k + 16]);
                ymm3 = __builtin_ia32_loadups256(&mat1->data[i][k + 24]);

                ymm0 = __builtin_ia32_mulps256(ymm0, ymm8);
                ymm1 = __builtin_ia32_mulps256(ymm1, ymm9);
                ymm2 = __builtin_ia32_mulps256(ymm2, ymm10);
                ymm3 = __builtin_ia32_mulps256(ymm3, ymm11);

                ymm0 = __builtin_ia32_addps256(ymm0, ymm1);
                ymm2 = __builtin_ia32_addps256(ymm2, ymm3);
                ymm0 = __builtin_ia32_addps256(ymm0, ymm2);

                __builtin_ia32_storeups256(scratchpad, ymm0);
                for (int m = 0; m < 8; m++)
                    sum += scratchpad[m];
            }
            for (; k < mat1->dim.cols; k++, z++) {
                sum += mat1->data[i][k] * arraytemp[z];
            }
            if (z == length)
                z = 0;
//            printf("sum = %f\n", sum);
            dst->data[i][j] = sum;
        }
    }
    free(arraytemp);

    // version2:
//    assert (mat1->dim.cols == mat2->dim.rows && dst->dim.rows == mat1->dim.rows && dst->dim.cols == mat2->dim.cols);
//            float global_sum = 0.0;
//            int i, j, k;
//#pragma omp parallel for simd private(global_sum, j, k)
//            for (i = 0; i < mat1->dim.rows; i++)
//                for (j = 0; j < mat2->dim.cols; j++) {
//                    global_sum = 0;
//                    for (k = 0; k < mat1->dim.cols; k++)
//                        global_sum += mat1->data[i][k] * mat2->data[k][j];
//#pragma omp cirtical
//                    dst->data[i][j] += global_sum;
//                }
        }




