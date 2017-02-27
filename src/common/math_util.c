#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <string.h>
#include <math.h>

#include "math_util.h"

void rpy_to_quat(double rpy[3], double quat[4])
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


void rpy_to_quat_cov(double rpy[3], double quat[4],
                     double cov_rpy[6][6], double cov_quat[7][7])
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
        for (int j = 0; i < 6; j++) {
            for (int k = 0; i < 6; k++) {
                tmp[i][j] += J[i][k] * cov_rpy[k][j];
            }
        }
    }
    for (int i = 0; i < 7; i++) {
        for (int j = 0; i < 7; j++) {
            for (int k = 0; i < 6; k++) {
                cov_quat[i][j] += tmp[i][k] * J[j][k]; //J'[k][j] = J[j][k]
            }
        }
    }
}
