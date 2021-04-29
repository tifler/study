#ifndef __SIMPLE_COORD_TRANSFORM_H__
#define __SIMPLE_COORD_TRANSFORM_H__

#include <stdint.h>

#if (INTPTR_MAX == INT64_MAX)
typedef double sct_float_t;
#else
typedef float sct_float_t;
#endif	/*INTPTR_MAX == INT64_MAX*/

typedef struct {
	double w;
	double h;
} sct_size_2d_t;

typedef struct {
	double x;
	double y;
} sct_point_2d_t;

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
	double theta;
	double phi;
} sct_point_sp_t;

void sct_cartesian_to_spherical(const sct_point_ca_t *ca, sct_point_sp_t *sp);
void sct_spherical_to_cartesian(const sct_point_sp_t *sp, sct_point_ca_t *ca);

#endif	/*__SIMPLE_COORD_TRANSFORM_H__*/
