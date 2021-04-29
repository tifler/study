#include <math.h>
#include "ptzmap.h"
#include "XDebug.h"

#if	0
/*
 * this equation is drived from an equation of finding the intersection
 * point of a plane and a line in the 3D cartesian coord-system.
 *
 * v1 (line)    : [0, 0, 0] + t[x0, y0, z0]
 * v2 (2d plane): [x1, y1, z1] = r,
 *                where [x1, y1, z1] is converted from [zoom, pan, tilt],
 *                r = ptz->zoom
 *
 * t[x0, y0, z0] dot [x1, y1, z1 ] = r
 * t(x0x1 + y0y1 + z0z1) = r
 * t = r / (x0x1 + y0y1 + z0z1)
 *
 * To get the intersection point, input t to the v1
 *
 * [x, y, z] = [x0 * t, y0 * t, z0 *t]
 */
void map_ptz_to_screen_3d(
		const sct_point_sp_t *ptz, const sct_point_sp_t *src,
		sct_size_2d_t size, sct_point_ca_t *dst)
{
	sct_point_ca_t tmp;
	sct_point_ca_t ptzca;

	/*
	 * convert points to cartesian coord to calculate intersection point.
	 */
	sct_spherical_to_cartesian(ptz, &ptzca);
	sct_spherical_to_cartesian(src, &tmp);

	double t = ptz->r / (tmp.x * ptzca.x + tmp.y * ptzca.y + tmp.z * ptzca.z);

	dst->x = t * tmp.x;
	dst->y = t * tmp.y;
	dst->z = t * tmp.z;
}
#endif	/*0*/

/*
 * this equation is drived from an equation of finding the intersection
 * point of a plane and a line in the 3D cartesian coord-system.
 *
 * v1 (line)    : [0, 0, 0] + t[x0, y0, z0]
 * v2 (2d plane): [0, 0, 1] = r (transformed to be parallel to Z-axis)
 *
 * t[x0, y0, z0] dot [0, 0, 1] = r
 * t * z0 = r
 * t = r / z0
 *
 * To get the intersection point, input t to the v1
 *
 * [x, y, z] = [ r * x0 / z0, r * y0 / z0, r * z0 / z0]
 * [x, y, z] = [ r * x0 / z0, r * y0 / z0, r]
 */
void map_ptz_to_screen_point(
		const sct_point_sp_t *ptz, const sct_point_sp_t *src,
		const sct_size_2d_t *size, const fov_t *fov, sct_point_2d_t *dst)
{
	sct_point_ca_t tmpca;
	sct_point_sp_t tmpsp;
	sct_point_2d_t tmp2d;

	/* Our objective is to project a point on a spherical surface to a 2D
	 * screen, so we can transform the point which is pointed by PTZ to be
	 * parallel to the Z-axis to simplify the problem, then we can calculate
	 * the points on a 2D screen. The transformation is simple, just substract
	 * ptz's angles from src's angles.
	 */
	tmpsp.r = src->r;
	tmpsp.theta = src->theta - ptz->theta;
	tmpsp.phi = src->phi - ptz->phi;
	sct_spherical_to_cartesian(&tmpsp, &tmpca);

	/* intersected point, tmp2d is on the 2D screen */
	tmp2d.x = src->r * tmpca.x / tmpca.z;
	tmp2d.y = src->r * tmpca.y / tmpca.z;
	//DBG("src->r=%f, x=%f, y=%f\n", src->r, tmp2d.x, tmp2d.y);
	// z = r;

	/* scaled final result */
	dst->x = (tmp2d.x / src->r) * (size->w / 2) / tan(fov->h / 2);
	dst->y = (tmp2d.y / src->r) * (size->h / 2) / tan(fov->h / 2);
	// w : h = y : y0
	dst->y = size->w * dst->y / size->h;
}

void map_screen_to_ptz_point(
		const sct_point_sp_t *ptz, const sct_point_2d_t *src,
		const sct_size_2d_t *size, const fov_t *fov, sct_point_sp_t *dst)
{
	sct_point_ca_t tmpca;

	tmpca.x = src->x;
	tmpca.y = src->y;
	tmpca.z = size->w / (2 * tan(fov->h / 2));
	sct_cartesian_to_spherical(&tmpca, dst);

	/*
	 * Now dst points a point with assuming PTZ's theta and phi are all zero.
	 * So we should rotate to actul direction.
	 */
	//DBG("dst (r=%f, theta=%f, phi=%f)\n", dst->r, dst->theta, dst->phi);
	dst->theta += ptz->theta;
	dst->phi += ptz->phi;

	/*
	 * dst is always on the PTZ's sphere surface, so the radius should be
	 * ptz->r
	 */
	dst->r = ptz->r;
}

/*
 * test the point's angles are in between ptz->center +/- fov angles
 */
int is_point_in_fov(
		const sct_point_sp_t *ptz, const fov_t *fov, const sct_point_sp_t *pt)
{
	return (((ptz->theta - fov->h / 2) <= pt->theta) &&
		((ptz->theta + fov->h / 2) >= pt->theta) &&
		((ptz->phi - fov->v / 2) <= pt->phi) &&
		((ptz->phi + fov->v / 2) >= pt->phi));
}

void map_screen_to_ptz_rect(
		const sct_point_sp_t *ptz, const rect_ca_t *src,
		const sct_size_2d_t *size, const fov_t *fov, rect_sp_t *dst)
{
	sct_point_2d_t pt;

	pt.x = src->lt.x;
	pt.y = src->lt.y;
	map_screen_to_ptz_point(ptz, &pt, size, fov, &dst->lt);

	pt.x = src->rt.x;
	pt.y = src->rt.y;
	map_screen_to_ptz_point(ptz, &pt, size, fov, &dst->rt);

	pt.x = src->lb.x;
	pt.y = src->lb.y;
	map_screen_to_ptz_point(ptz, &pt, size, fov, &dst->lb);

	pt.x = src->rb.x;
	pt.y = src->rb.y;
	map_screen_to_ptz_point(ptz, &pt, size, fov, &dst->rb);
}

void map_ptz_to_screen_rect(
		const sct_point_sp_t *ptz, const rect_sp_t *src,
		const sct_size_2d_t *size, const fov_t *fov, rect_ca_t *dst)
{
	sct_point_2d_t pt;

	map_ptz_to_screen_point(ptz, &src->lt, size, fov, &pt);
	dst->lt.x = pt.x;
	dst->lt.y = pt.y;

	map_ptz_to_screen_point(ptz, &src->rt, size, fov, &pt);
	dst->rt.x = pt.x;
	dst->rt.y = pt.y;

	map_ptz_to_screen_point(ptz, &src->lb, size, fov, &pt);
	dst->lb.x = pt.x;
	dst->lb.y = pt.y;

	map_ptz_to_screen_point(ptz, &src->rb, size, fov, &pt);
	dst->rb.x = pt.x;
	dst->rb.y = pt.y;
}

void get_cover_rect_ca(const rect_ca_t *src, rect_ca_t *dst)
{
	dst->lt.x = dst->lb.x = MIN(src->lt.x, src->lb.x);
	dst->lt.y = dst->rt.y = MIN(src->lt.y, src->rt.y);
	dst->rt.x = dst->rb.x = MAX(src->rt.x, src->rb.x);
	dst->lb.y = dst->rb.y = MAX(src->lb.y, src->rb.y);
}

/*
 * assume 1. two rect_ca_t has the same r.
 *        2. lt.phi and lb.phi are equal.
 *        3. lt.theta and rt.theta are equal.
 */
int do_intersect_rect_sp(const rect_sp_t *a, const rect_sp_t *b)
{
	return
		MAX(0, MAX(a->lt.phi, b->lt.phi) - MIN(a->rt.phi, b->rt.phi)) &&
		MAX(0, MAX(a->lb.theta, b->lb.theta) - MIN(a->lt.theta, b->lt.theta));
}

int get_intersect_rect_sp(const rect_sp_t *a, const rect_sp_t *b, rect_sp_t *r)
{
	if (!do_intersect_rect_sp(a, b))
		return 0;

	r->lt.phi = r->lb.phi = MAX(a->lt.phi, b->lt.phi);
	r->lt.theta = r->lb.theta = MAX(a->lt.theta, b->lt.theta);
	r->rt.phi = r->rb.phi = MIN(a->rt.phi, b->rt.phi);
	r->rt.theta = r->rb.theta = MIN(a->rt.theta, b->rt.theta);

	return 1;
}
