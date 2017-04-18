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

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "common/print_util.h"
#include "common/time_util.h"
#include "common/matrix.h"

#define BREAK printf("\n")

int main(int argc, char **args)
{
    print_marker("XRobot","Hello World!");
    print_marker("XRobot","Test for matrix operations!");

    double data[] = {1.2, 1.4, 1.6, 1.8, 1.9, 2.0};

    matrix_t *m = matrix_create_data(2, 3, data);
    /* matrix_print(m, "%.2f, "); */
    /* BREAK; */

    matrix_transpose_inplace(m);
    matrix_print(m, "%.2f, ");
    BREAK;

    matrix_t *m1 = matrix_create_identity(3);
    /* matrix_print(m1, "%.2f, "); */
    /* BREAK; */


    /* matrix_add_inplace(m1, m1); */
    /* matrix_print(m1, "%.2f, "); */
    /* BREAK; */

    /* matrix_mul_inplace(m1, m1); */
    /* matrix_print(m1, "%.2f, "); */
    /* BREAK; */

    matrix_row_switch(m1, 0, 2);
    /* matrix_print(m1, "%.2f, "); */
    /* BREAK; */

    matrix_col_switch(m1, 1, 2);
    matrix_print(m1, "%.2f, ");
    BREAK;

    matrix_t *m_inv = matrix_inverse(m1);
    matrix_print(m_inv, "%.2f, ");
    BREAK;

    /* matrix_t *m5 = matrix_mul(m1, m_inv); */
    /* matrix_print(m5, "%.2f, "); */
    /* BREAK; */

    matrix_plu_t *m_plu = matrix_PLU(m1);

    /* matrix_print(m_plu->P, "%.2f, "); */
    /* BREAK; */
    /* matrix_print(m_plu->L, "%.2f, "); */
    /* BREAK; */
    /* matrix_print(m_plu->U, "%.2f, "); */
    /* BREAK; */
    /* matrix_t *m3 = matrix_mul(m_plu->L, m_plu->U); */
    /* matrix_t *m4 = matrix_mul(m_plu->P, m1); */
    /* if(!matrix_equal(m3,m4)) */
    /*     printf("Yeah! :) %f \n", m_plu->det); */
    /* else */
    /*     puts(":("); */

    //PLU solver test
    matrix_t *x = matrix_PLU_solver(m_plu, m);
    matrix_print(x, "%.2f, ");
    BREAK;

    matrix_destroy(x);
    matrix_destroy(m_plu->P);
    matrix_destroy(m_plu->L);
    matrix_destroy(m_plu->U);
    free(m_plu);
    matrix_destroy(m);
    matrix_destroy(m1);
    matrix_destroy(m_inv);
    return 0;
}
