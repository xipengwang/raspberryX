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

#include "math_util.h"

/**
 * @Ref
 * A tutorial on se (3) transformation parameterizations and on-manifold optimization
 */

void rpy_to_quat(const double rpy[3], double quat[4])
{
    double c_r = cos(rpy[0]/2);
    double c_p = cos(rpy[1]/2);
    double c_y = cos(rpy[2]/2);

    double s_r = sin(rpy[0]/2);
    double s_p = sin(rpy[1]/2);
    double s_y = sin(rpy[2]/2);

    quat[0] = c_r*c_p*c_y + s_r*s_p*s_y;
    quat[1] = s_r*c_p*c_y - c_r*s_p*s_y;
    quat[2] = c_r*s_p*c_y + s_r*c_p*s_y;
    quat[3] = c_r*c_p*s_y - s_r*s_p*c_y;
}

void rpy_to_quat_cov(const double rpy[3], double quat[4],
                     const double cov_rpy[6][6], double cov_quat[7][7])
{

    double J[7][6] = {0};
    double c_r = cos(rpy[0]/2);
    double c_p = cos(rpy[1]/2);
    double c_y = cos(rpy[2]/2);

    double s_r = sin(rpy[0]/2);
    double s_p = sin(rpy[1]/2);
    double s_y = sin(rpy[2]/2);

    double ccc = c_r*c_p*c_y;
    double sss = s_r*s_p*s_y;
    double ccs = c_r*c_p*s_y;
    double csc = c_r*s_p*c_y;
    double scc = s_r*c_p*c_y;
    double ssc = s_r*s_p*c_y;
    double scs = s_r*c_p*s_y;
    double css = c_r*s_p*s_y;

    J[0][0] = 1;
    J[1][1] = 1;
    J[2][2] = 1;

    J[3][3] = (ssc - ccs) / 2;
    J[3][4] = (scs - csc) / 2;
    J[3][5] = (css - scc) / 2;

    J[4][3] = -(csc + scs) / 2;
    J[4][4] = -(ssc + ccs) / 2;
    J[4][5] =  (ccc + sss) / 2;

    J[5][3] = (scc - css) / 2;
    J[5][4] = (ccc - sss) / 2;
    J[5][5] = (ccs - ssc) / 2;

    J[6][3] =  (ccc + sss) / 2;
    J[6][4] = -(css + scc) / 2;
    J[6][5] = -(csc + scs) / 2;

    //J * cov_rpy * J'
    double tmp[7][6] = {0};
    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 6; j++) {
            for (int k = 0; k < 6; k++) {
                tmp[i][j] += J[i][k] * cov_rpy[k][j];
            }
        }
    }
    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 7; j++) {
            for (int k = 0; k < 6; k++) {
                cov_quat[i][j] += tmp[i][k] * J[j][k]; //J'[k][j] = J[j][k]
            }
        }
    }
}

void quat_to_rpy(const double quat[4], double rpy[3])
{
    const double qr = quat[0];
    const double qx = quat[1];
    const double qy = quat[2];
    const double qz = quat[3];

    double disc = qr*qy - qx*qz;
    if (fabs(disc + 0.5) < M_EPSILON) {
        //near -0.5
        rpy[0] = 2*atan(qx/qr);
        rpy[1] = -M_PI/2;
        rpy[2] = 0;
    } else if(fabs(disc - 0.5) < M_EPSILON) {
        //near 0.5
        rpy[0] = -2*atan(qx/qr);
        rpy[1] = M_PI/2;
        rpy[2] = 0;
    } else {
        rpy[0] = atan(2*(qr*qz+qx*qy)/(1-2*(pow(qy,2) + pow(qz,2))));
        rpy[1] = asin(2*disc);
        rpy[2] = atan(2*(qr*qx+qy*qz)/(1-2*(pow(qx,2) + pow(qy,2))));
    }
}

void quat_to_rpy_cov(const double quat[4], double rpy[3],
                     const double cov_quat[7][7], double cov_rpy[6][6])
{
    assert(0);
    double J[6][7] = {0};

//TODO: Need to calculate Jacobian: rpy with respect to quat

    //J * cov_rpy * J'
    double tmp[6][7] = {0};
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 7; j++) {
            for (int k = 0; k < 7; k++) {
                tmp[i][j] += J[i][k] * cov_quat[k][j];
            }
        }
    }
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 6; j++) {
            for (int k = 0; k < 7; k++) {
                cov_rpy[i][j] += tmp[i][k] * J[j][k]; //J'[k][j] = J[j][k]
            }
        }
    }
}

void rpy_to_T44(const double rpy_xyz[6], double T44[4][4])
{

    T44[0][3] = rpy_xyz[3];
    T44[1][3] = rpy_xyz[4];
    T44[2][3] = rpy_xyz[5];
    T44[3][3] = 1;

    double c_r = cos(rpy_xyz[0]);
    double c_p = cos(rpy_xyz[1]);
    double c_y = cos(rpy_xyz[2]);

    double s_r = sin(rpy_xyz[0]);
    double s_p = sin(rpy_xyz[1]);
    double s_y = sin(rpy_xyz[2]);

    T44[0][0] = c_y*c_p;
    T44[0][1] = c_y*s_p*s_r - s_y*c_r;
    T44[0][2] = c_y*s_p*c_r + s_y*s_r;
    T44[1][0] = s_y*c_p;
    T44[1][1] = s_y*s_p*s_r + c_y*c_r;
    T44[1][2] = s_y*s_p*c_r - c_y*s_r;
    T44[2][0] = -s_p;
    T44[2][1] = c_p*s_r;
    T44[2][2] = c_p*c_r;
}

void rpy_to_T44_cov(const double rpy_xyz[6], double T44[4][4],
                    const double cov_rpy[6][6], double cov_T44[4][4])
{
    assert(0);
    double J[4][6] = {0};
    //TODO: Need to calculate Jacobian: T44 with respect to rpy

    //J * cov_rpy * J'
    double tmp[4][6] = {0};
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 6; j++) {
            for (int k = 0; k < 6; k++) {
                tmp[i][j] += J[i][k] * cov_rpy[k][j];
            }
        }
    }
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            for (int k = 0; k < 6; k++) {
                cov_T44[i][j] += tmp[i][k] * J[j][k]; //J'[k][j] = J[j][k]
            }
        }
    }
}

void T44_to_rpy(const double T44[4][4], double rpy_xyz[6])
{
    rpy_xyz[3] = T44[0][3];
    rpy_xyz[4] = T44[1][3];
    rpy_xyz[5] = T44[2][3];

    double p = atan2(T44[2][0], sqrt(T44[0][0]*T44[0][0] + T44[1][0]*T44[1][0]));
    double r, y;
    if (fabs(p - M_PI/2) < M_EPSILON) {
        // 90 degree
        r = 0;
        y = atan2(T44[1][2], T44[0][2]);
    } else if (fabs(p + M_PI/2) < M_EPSILON) {
        // -90 degree
        r = 0;
        y = atan2(-T44[1][2], -T44[0][2]);
    } else {
        y = atan2(T44[1][0], T44[0][0]);
        r = atan2(T44[2][1], T44[2][2]);
    }
    rpy_xyz[0] = r;
    rpy_xyz[1] = p;
    rpy_xyz[2] = y;
}
