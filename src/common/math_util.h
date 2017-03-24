#ifndef _COMMON_MATHUTIL_H
#define _COMMON_MATHUTIL_H

#include <math.h>
#include <float.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef M_TWOPI
# define M_TWOPI       6.2831853071795862319959
#endif

#ifndef M_PI
# define M_PI 3.141592653589793238462643383279502884196
#endif

#define to_radians(x) ( (x) * (M_PI / 180.0 ))
#define to_degrees(x) ( (x) * (180.0 / M_PI ))

#define max(A, B) (A < B ? B : A)
#define min(A, B) (A < B ? A : B)

#define M_EPSILON 1E-8

    // random number between [0, 1)
    static inline float randf()
    {
        return ((float) rand()) / (RAND_MAX + 1.0);
    }

    // Map vin to [0, 2*PI)
    static inline double mod2pi_positive(double vin)
    {
        return vin - M_TWOPI * floor(vin / M_TWOPI);
    }

    // Map vin to [-PI, PI)
    static inline double mod2pi(double vin)
    {
        return mod2pi_positive(vin + M_PI) - M_PI;
    }

    // Map vin to [-PI, PI)
    static inline double angle_error(double va, double vb)
    {
        return mod2pi(mod2pi(va) - mod2pi(vb));
    }

    void rpy_to_quat(const double rpy[3], double quat[4]);
    void quat_to_rpy(const double quat[4], double rpy[3]);
    void rpy_to_T44(const double rpy_xyz[6], double T44[4][4]);
    void T44_to_rpy(const double T44[4][4], double rpy_xyz[6]);

#ifdef __cplusplus
}
#endif

#endif
