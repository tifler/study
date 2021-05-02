#ifndef __SIMPLE_COORD_TRANSFORM_H__
#define __SIMPLE_COORD_TRANSFORM_H__

#include <stdint.h>

#if (INTPTR_MAX == INT64_MAX)
typedef double sct_float_t;
#else
typedef float sct_float_t;
#endif	/*INTPTR_MAX == INT64_MAX*/

#if	0
#define	__SPHERICAL_COMPENSATE(base, max)		\
	do { while ((base) < 0.0F) (base) += (max); } while (0)
#else
#define	__SPHERICAL_COMPENSATE(...)			do { } while (0)
#endif	/*0*/

#define	COMPENSATE_PHI(phi)			__SPHERICAL_COMPENSATE((phi), 2 * M_PI)
#define	COMPENSATE_THETA(theta)		__SPHERICAL_COMPENSATE((theta), M_PI)

typedef struct {
	double w;
	double h;
	double d;
} sct_size_t;

typedef struct {
	double x;
	double y;
	double z;
} sct_point_ca_t;

/* See the https://en.wikipedia.org/wiki/Spherical_coordinate_system
 *
 * Used mathmatics notation not physics.
 *
 * r : r, radial distance
 * t : theta, polar angle
 * p : phi, azimuthal angle
 */
typedef struct {
	double r;
	double phi;
	double theta;
} sct_point_sp_t;

void sct_cartesian_to_spherical(const sct_point_ca_t *ca, sct_point_sp_t *sp);
void sct_spherical_to_cartesian(const sct_point_sp_t *sp, sct_point_ca_t *ca);
void sct_roll(const sct_point_ca_t *s, double roll, sct_point_ca_t *d);
void sct_pitch(const sct_point_ca_t *s, double pitch, sct_point_ca_t *d);
void sct_yaw(const sct_point_ca_t *s, double yaw, sct_point_ca_t *d);

#endif	/*__SIMPLE_COORD_TRANSFORM_H__*/
