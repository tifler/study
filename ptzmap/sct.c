#include <math.h>
#include "sct.h"

void sct_cartesian_to_spherical(const sct_point_ca_t *ca, sct_point_sp_t *sp)
{
	sp->r = sqrt(pow(ca->x, 2) + pow(ca->y, 2) + pow(ca->z, 2));
	sp->theta = acos(ca->z / sp->r);
	sp->phi = (ca->y == 0.0F) ? 0.0F : atan(ca->y / ca->x);
}

void sct_spherical_to_cartesian(const sct_point_sp_t *sp, sct_point_ca_t *ca)
{
	ca->x = sp->r * cos(sp->phi) * sin(sp->theta);
	ca->y = sp->r * sin(sp->phi) * sin(sp->theta);
	ca->z = sp->r * cos(sp->theta);
}

