#include <math.h>
#include "sct.h"
#include "XDebug.h"

void sct_cartesian_to_spherical(const sct_point_ca_t *ca, sct_point_sp_t *sp)
{
	sp->r = sqrt(pow(ca->x, 2) + pow(ca->y, 2) + pow(ca->z, 2));
	sp->theta = acos(ca->z / sp->r);
	COMPENSATE_THETA(sp->theta);
	/* XXX we have to use atan2 not atan */
	sp->phi = atan2(ca->y, ca->x);
	COMPENSATE_PHI(sp->phi);
}

void sct_spherical_to_cartesian(const sct_point_sp_t *sp, sct_point_ca_t *ca)
{
	ca->x = sp->r * cos(sp->phi) * sin(sp->theta);
	ca->y = sp->r * sin(sp->phi) * sin(sp->theta);
	ca->z = sp->r * cos(sp->theta);
}

// ceter: z-axis
void sct_roll(const sct_point_ca_t *s, double roll, sct_point_ca_t *d)
{
	/*
	 * cos -sin 0 0    x
	 * sin cos  0 0    y
	 * 0   0    1 0    z
	 * 0   0    0 1    1
	 */
	d->x = s->x * cos(roll) - s->y * sin(roll);
	d->y = s->x * sin(roll) + s->y * cos(roll);
	d->z = s->z;
}

// center: x-axis
void sct_pitch(const sct_point_ca_t *s, double pitch, sct_point_ca_t *d)
{
	/*
	 * 1 0   0    0
	 * 0 cos -sin 0
	 * 0 sin cos  0
	 * 0 0   0    1
	 */
	d->x = s->x;
	d->y = s->y * cos(pitch) - s->z * sin(pitch);
	d->z = s->y * sin(pitch) + s->z * cos(pitch);
}

// center: y-axis
void sct_yaw(const sct_point_ca_t *s, double yaw, sct_point_ca_t *d)
{
	/*
	 *  cos 0 sin 0
	 *  0   1 0   0
	 * -sin 0 cos 0
	 *  0   0   0 1
	 */
	d->x = s->x * cos(yaw) + s->z * sin(yaw);
	d->y = s->y;
	d->z = s->z * cos(yaw) - s->x * sin(yaw);
}
