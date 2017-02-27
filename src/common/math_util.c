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
        for (int j = 0; j < 7; j++) {
            for (int k = 0; k < 7; k++) {
                cov_rpy[i][j] += tmp[i][k] * J[j][k]; //J'[k][j] = J[j][k]
            }
        }
    }
}
