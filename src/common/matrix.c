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

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <string.h>
#include <math.h>

#include "matrix.h"

matrix_t *matrix_create(int rows, int cols)
{
    assert(rows > 0);
    assert(cols > 0);

    matrix_t *m = calloc(1, sizeof(matrix_t));
    m->nrows = rows;
    m->ncols = cols;
    m->data = calloc(rows * cols, sizeof(double));

    return m;
}

matrix_t *matrix_create_data(int rows, int cols, const double *data)
{
    assert(rows > 0);
    assert(cols > 0);

    matrix_t *m = calloc(1, sizeof(matrix_t));
    m->nrows = rows;
    m->ncols = cols;
    m->data = calloc(rows * cols, sizeof(double));

    int k = 0;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            MATRIX_EL(m, i, j) = data[k++];
        }
    }
    return m;
}

matrix_t *matrix_create_identity(int dim)
{
    assert(dim > 0);
    matrix_t *m = calloc(1, sizeof(matrix_t));
    m->nrows = dim;
    m->ncols = dim;
    m->data = calloc(dim * dim, sizeof(double));

    for (int i = 0; i < dim; i++) {
        MATRIX_EL(m, i, i) = 1;
    }
    return m;
}

//row counting starts at 0
void matrix_put(matrix_t *m, int row, int col, double v)
{
    assert(m);
    assert(m->nrows > row);
    assert(m->ncols > col);
    MATRIX_EL(m, row, col) = v;
}

double matrix_get(const matrix_t *m, int row, int col)
{
    assert(m);
    assert(m->nrows > row);
    assert(m->ncols > col);
    return MATRIX_EL(m, row, col);
}

matrix_t *matrix_copy(const matrix_t *_m)
{
    assert(_m);
    matrix_t *m = calloc(1, sizeof(matrix_t));
    m->nrows = _m->nrows;
    m->ncols = _m->ncols;
    m->data = calloc(m->nrows * m->ncols, sizeof(double));
    memcpy(m->data, _m->data, m->nrows * m->ncols * sizeof(double));
    return m;
}

int matrix_exact_equal(const matrix_t *a, const matrix_t *b)
{
    assert(a);
    assert(b);
    assert(a->ncols == b->ncols);
    assert(a->nrows == b->nrows);

    for (int i = 0; i < a->nrows; i++) {
        for (int j = 0; j < a->ncols; j++) {
            if(MATRIX_EL(a, i, j) != MATRIX_EL(b, i, j)) {
                return -1;
            }

        }
    }
    return 0;
}

int matrix_equal(const matrix_t *a, const matrix_t *b)
{
    assert(a);
    assert(b);
    assert(a->ncols == b->ncols);
    assert(a->nrows == b->nrows);

    for (int i = 0; i < a->nrows; i++) {
        for (int j = 0; j < a->ncols; j++) {
            if(fabs(MATRIX_EL(a, i, j) - MATRIX_EL(b, i, j)) > M_EPS) {
                return -1;
            }

        }
    }
    return 0;
}

matrix_t* matrix_transpose(const matrix_t *_m)
{
    matrix_t *m = matrix_create(_m->nrows, _m->ncols);
    for (int i = 0; i < m->nrows; i++) {
        for (int j = 0; j < m->ncols; j++) {
            MATRIX_EL(m, i, j) = MATRIX_EL(_m, j, i);
        }
    }
    return m;
}

void matrix_transpose_inplace(matrix_t *m)
{
    matrix_t *_m  = matrix_copy(m);
    m->nrows = _m->ncols;
    m->ncols = _m->nrows;
    for (int i = 0; i < m->nrows; i++) {
        for (int j = 0; j < m->ncols; j++) {
            MATRIX_EL(m, i, j) = MATRIX_EL(_m, j, i);
        }
    }
    matrix_destroy(_m);
}

matrix_t* matrix_add(const matrix_t *a, const matrix_t *b)
{
    assert(a);
    assert(b);
    assert(a->ncols == b->ncols);
    assert(a->nrows == b->nrows);

    matrix_t *m = matrix_create(a->nrows, a->ncols);
    for (int i = 0; i < m->nrows; i++) {
        for (int j = 0; j < m->ncols; j++) {
            MATRIX_EL(m, i, j) = MATRIX_EL(a, i, j) + MATRIX_EL(b, i, j);
        }
    }
    return m;

}

matrix_t* matrix_add_inplace(matrix_t *a, const matrix_t *b)
{
    assert(a);
    assert(b);
    assert(a->ncols == b->ncols);
    assert(a->nrows == b->nrows);

    matrix_t *m = a;
    for (int i = 0; i < m->nrows; i++) {
        for (int j = 0; j < m->ncols; j++) {
            MATRIX_EL(m, i, j) = MATRIX_EL(a, i, j) + MATRIX_EL(b, i, j);
        }
    }
    return m;

}

matrix_t* matrix_subtract(const matrix_t *a, const matrix_t *b)
{
    assert(a);
    assert(b);
    assert(a->ncols == b->ncols);
    assert(a->nrows == b->nrows);

    matrix_t *m = matrix_create(a->nrows, a->ncols);
    for (int i = 0; i < m->nrows; i++) {
        for (int j = 0; j < m->ncols; j++) {
            MATRIX_EL(m, i, j) = MATRIX_EL(a, i, j) - MATRIX_EL(b, i, j);
        }
    }
    return m;

}

matrix_t* matrix_subtract_inplace(matrix_t *a, const matrix_t *b)
{
    assert(a);
    assert(b);
    assert(a->ncols == b->ncols);
    assert(a->nrows == b->nrows);

    matrix_t *m = a;
    for (int i = 0; i < m->nrows; i++) {
        for (int j = 0; j < m->ncols; j++) {
            MATRIX_EL(m, i, j) = MATRIX_EL(a, i, j) - MATRIX_EL(b, i, j);
        }
    }
    return m;

}

matrix_t* matrix_mul(const matrix_t *a, const matrix_t *b)
{
    assert(a);
    assert(b);
    assert(a->ncols == b->nrows);

    matrix_t *m = matrix_create(a->nrows, b->ncols);
    for (int i = 0; i < m->nrows; i++) {
        for (int j = 0; j < m->ncols; j++) {
            double acc = 0.0;
            for (int k = 0; k < a->ncols; k++) {
                acc += MATRIX_EL(a, i, k) * MATRIX_EL(b, k, j);
            }
            MATRIX_EL(m, i, j) = acc;
        }
    }
    return m;
}

void matrix_mul_inplace(matrix_t *a, const matrix_t *_b)
{
    assert(a);
    assert(_b);
    assert(a->ncols == _b->nrows);

    matrix_t *m = matrix_copy(a);
    const matrix_t *b = _b;
    if(a == b) {
        b = m;
    }
    a->nrows = m->nrows;
    a->ncols = b->ncols;
    free(a->data);
    a->data = calloc(a->nrows * a->ncols, sizeof(double));
    for (int i = 0; i < m->nrows; i++) {
        for (int j = 0; j < m->ncols; j++) {
            double acc = 0.0;
            for (int k = 0; k < a->ncols; k++) {
                acc += MATRIX_EL(m, i, k) * MATRIX_EL(b, k, j);
            }
            MATRIX_EL(a, i, j) = acc;
        }
    }
    matrix_destroy(m);
}

void matrix_sub_col_switch(matrix_t *a, int c_i, int c_j, int row0, int row1)
{
    assert(a);
    assert(c_i >= 0);
    assert(c_j >= 0);
    assert(c_i < a->ncols);
    assert(c_j < a->ncols);
    assert(row0 >= 0);
    assert(row1 < a->nrows);
    assert(row0 <= row1);
    double tmp;
    for (int r = row0; r <= row1; r++) {
        tmp = MATRIX_EL(a, r, c_i);
        MATRIX_EL(a, r, c_i) = MATRIX_EL(a, r, c_j);
        MATRIX_EL(a, r, c_j) = tmp;
    }
}

void matrix_col_switch(matrix_t *a, int c_i, int c_j)
{
    matrix_sub_col_switch(a, c_i, c_j, 0, a->nrows-1);
}

void matrix_sub_row_switch(matrix_t *a, int r_i, int r_j, int col0, int col1)
{
    assert(a);
    assert(r_i >= 0);
    assert(r_j >= 0);
    assert(r_i < a->nrows);
    assert(r_j < a->nrows);
    assert(col0 >= 0);
    assert(col1 < a->ncols);
    assert(col0 <=  col1);
    double tmp;
    for (int c = col0; c <= col1; c++) {
        tmp = MATRIX_EL(a, r_i, c);
        MATRIX_EL(a, r_i, c) = MATRIX_EL(a, r_j, c);
        MATRIX_EL(a, r_j, c) = tmp;
    }
}

void matrix_row_switch(matrix_t *a, int r_i, int r_j)
{
    matrix_sub_row_switch(a, r_i, r_j, 0, a->ncols-1);
}

matrix_plu_t *matrix_PLU(const matrix_t *a)
{
    assert(a);
    assert(a->ncols == a->nrows);
    matrix_plu_t *m_plu = calloc(1, sizeof(matrix_plu_t));
    m_plu->P = matrix_create_identity(a->nrows);
    m_plu->L = matrix_create_identity(a->nrows);
    m_plu->U = matrix_copy(a);
    m_plu->det = 1;
    for (int i = 0; i < a->nrows-1; i++) {
        //partial pivoting
        int max_idx = i;
        double max_v = MATRIX_EL(m_plu->U, i, i);
        for (int j = i+1; j < a->nrows; j++) {
            if (MATRIX_EL(m_plu->U, j, i) > max_v) {
                max_idx = j;
                max_v = MATRIX_EL(m_plu->U, j, i);
            }
        }
        //switch row i and row max_idx;
        if (i != max_idx) {
            m_plu->det *= -1;
            matrix_row_switch(m_plu->U, i, max_idx);
            matrix_row_switch(m_plu->P, i, max_idx);
            if ( i > 0) {
                matrix_sub_row_switch(m_plu->L, i, max_idx, 0, i-1);
            }
        }

        for (int j = i+1; j < a->nrows; j++) {
            MATRIX_EL(m_plu->L, j, i) = MATRIX_EL(m_plu->U, j, i) / MATRIX_EL(m_plu->U, i, i);
            for (int k = j; j < a->ncols; j++) {
                MATRIX_EL(m_plu->U, j, k) =
                    MATRIX_EL(m_plu->U, j, k) - MATRIX_EL(m_plu->L, j, i) * MATRIX_EL(m_plu->U, i, i);
            }
        }
        m_plu->det *= MATRIX_EL(m_plu->U, i, i);
    }
    m_plu->det *= MATRIX_EL(m_plu->U, (a->nrows-1), (a->nrows-1));
    return m_plu;
}

matrix_t* matrix_PLU_solver(const matrix_plu_t *m_plu, const matrix_t *m)
{
    assert(m);
    assert(m_plu);
    assert(m->nrows == m_plu->P->nrows);

    matrix_t *L = m_plu->L;
    matrix_t *U = m_plu->U;
    // Not a full rank matrix
    if(m_plu->det == 0)
        return NULL;

    //permutation
    matrix_t *s = matrix_mul(m_plu->P, m);

    // LUx = s; Ly = s; Ux = y;
    matrix_t *y = matrix_copy(s);
    // forward substitution
    for (int i = 0; i < m->ncols; i++) {
        for (int j = 0; j < m->nrows; j++ ) {
            for (int k = 0; k < j; k++) {
                MATRIX_EL(y, j, i) =
                    MATRIX_EL(s, j, i) - MATRIX_EL(y, k, i) * MATRIX_EL(L, j, k);
            }
            MATRIX_EL(y, j, i) /=  MATRIX_EL(L, j, j);
        }
    }
    // backward substitution
    matrix_t *x = matrix_copy(y);
    for (int i = 0; i < m->ncols; i++) {
        for (int j = m->nrows-1; j >= 0; j-- ) {
            for (int k = m->nrows-1; k > j; k--) {
                MATRIX_EL(x, j, i) =
                    MATRIX_EL(y, j, i) - MATRIX_EL(x, k, i) * MATRIX_EL(U, j, k);
            }
            MATRIX_EL(x, j, i) /=  MATRIX_EL(U, j, j);
        }
    }
    matrix_destroy(s);
    matrix_destroy(y);
    return x;

}

static double matrix_plu_det(const matrix_t *a)
{
    matrix_plu_t *m = matrix_PLU(a);
    double det = m->det;
    matrix_destroy(m->P);
    matrix_destroy(m->U);
    matrix_destroy(m->L);
    free(m);
    return det;
}

double matrix_det(const matrix_t *a)
{
    assert(a);
    assert(a->nrows == a->ncols);
    switch(a->nrows) {

        case 0:
            assert(0);
            break;

        case 1:
            return a->data[0];

        case 2:
            return a->data[0] * a->data[3] - a->data[1] * a->data[2];

        case 3:
            return  a->data[0]*a->data[4]*a->data[8]
                - a->data[0]*a->data[5]*a->data[7]
                + a->data[1]*a->data[5]*a->data[6]
                - a->data[1]*a->data[3]*a->data[8]
                + a->data[2]*a->data[3]*a->data[7]
                - a->data[2]*a->data[4]*a->data[6];
        default:
            return matrix_plu_det(a);
    }
}

matrix_t* matrix_plu_inv(const matrix_plu_t *m_plu)
{

    matrix_t *m = matrix_create_identity(m_plu->P->nrows);
    matrix_t *x = matrix_PLU_solver(m_plu, m);
    matrix_destroy(m);
    return x;
}

matrix_t* matrix_inverse(const matrix_t *a)
{
    assert(a);
    assert(a->nrows == a->ncols);
    matrix_t *m;
    matrix_plu_t *m_plu;
    switch(a->nrows) {

        case 0:
            assert(0);
            break;

        case 1:
            if(!a->data[0])
                return NULL;
            m = matrix_create(1,1);
            m->data[0] = 1/m->data[0];
            return m;

        case 2:
            if(!(a->data[0] * a->data[3] - a->data[1] * a->data[2]))
                return NULL;
            m = matrix_create(2,2);
            return m;

        default:
            m_plu = matrix_PLU(a);
            m = matrix_plu_inv(m_plu);
            matrix_destroy(m_plu->P);
            matrix_destroy(m_plu->L);
            matrix_destroy(m_plu->U);
            free(m_plu);
            return m;
    }
}

void matrix_print(const matrix_t *m, char *fmt)
{
    assert(m);
    assert(fmt);
    for (int i = 0; i < m->nrows; i++) {
        for (int j = 0; j < m->ncols; j++) {
            printf(fmt, MATRIX_EL(m, i, j));
        }
        printf("\n");
    }
}

void matrix_destroy(matrix_t *m)
{
    if(!m)
        return;
    free(m->data);
    free(m);
}
