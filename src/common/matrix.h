/*
 * Copyright (C) <2017>  <Xipeng Wang>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _COMMON_MATRIX_H
#define _COMMON_MATRIX_H

#ifdef __cplusplus
extern "C" {
#endif

    struct matrix
    {
        unsigned int nrows, ncols;
        double *data;
    };

    typedef struct matrix matrix_t;

    struct matrix_plu
    {
        matrix_t *P;
        matrix_t *L;
        matrix_t *U;
        double det;
    };

    typedef struct matrix_plu matrix_plu_t;

#ifdef __cplusplus
}
#endif

#define MATRIX_EL(m, i, j) m->data[i * m->ncols + j]
#define M_EPS 10E-8

matrix_t *matrix_create(int rows, int cols);
matrix_t *matrix_create_data(int rows, int cols, const double *data);
matrix_t *matrix_create_identity(int dim);
matrix_t *matrix_copy(const matrix_t *_m);
int matrix_equal(const matrix_t *a, const matrix_t *b);
int matrix_exact_equal(const matrix_t *a, const matrix_t *b);
void matrix_put(matrix_t *m, int row, int col, double v);
double matrix_get(const matrix_t *m, int row, int col);
matrix_t* matrix_transpose(const matrix_t *_m);
void matrix_transpose_inplace(matrix_t *m);
matrix_t* matrix_add(const matrix_t *a, const matrix_t *b);
matrix_t* matrix_add_inplace(matrix_t *a, const matrix_t *b);
matrix_t* matrix_subtract(const matrix_t *a, const matrix_t *b);
matrix_t* matrix_subtract_inplace(matrix_t *a, const matrix_t *b);
matrix_t* matrix_mul(const matrix_t *a, const matrix_t *b);
void matrix_mul_inplace(matrix_t *a, const matrix_t *b);
void matrix_sub_col_switch(matrix_t *a, int c_i, int c_j, int row0, int row1);
void matrix_col_switch(matrix_t *a, int c_i, int c_j);
void matrix_sub_row_switch(matrix_t *a, int r_i, int r_j, int col0, int col1);
void matrix_row_switch(matrix_t *a, int r_i, int r_j);
matrix_plu_t* matrix_PLU(const matrix_t *a);
matrix_t* matrix_PLU_solver(const matrix_plu_t *m_plu, const matrix_t *m);
matrix_t* matrix_plu_inv(const matrix_plu_t *m_plu);
matrix_t* matrix_inverse(const matrix_t *m);
double matrix_det(const matrix_t *a);
void matrix_print(const matrix_t *m, char *fmt);
void matrix_destroy(matrix_t *m);

#endif
