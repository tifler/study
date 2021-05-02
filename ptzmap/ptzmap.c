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
		sct_size_t size, sct_point_ca_t *dst)
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
#if	0
void map_ptz_to_screen_point(
		const sct_point_sp_t *ptz, const sct_point_sp_t *src,
		const sct_size_t *size, const fov_t *fov, int hflip, int vflip,
		sct_point_ca_t *dst)
{
	sct_point_ca_t tmpca;
	sct_point_sp_t tmpsp;
	sct_point_ca_t tmp;

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

	/* intersected point, tmp is on the 2D screen */
	tmp.x = src->r * tmpca.x / tmpca.z;
	tmp.y = src->r * tmpca.y / tmpca.z;
	DBG("src->r=%f, x=%f, y=%f\n", src->r, tmp.x, tmp.y);

	/* scaled final result */
	dst->x = (tmp.x / src->r) * (size->w / 2) / tan(fov->h / 2) + size->w / 2;
	if (hflip)
		dst->x = size->w - dst->x;

	dst->y = (tmp.y / src->r) * (size->h / 2) / tan(fov->h / 2);
	// w : h = y : y0
	dst->y = size->w * dst->y / size->h + size->h / 2;
	if (vflip)
		dst->y = size->h - dst->y;

	dst->z = src->r;
}
#else
void map_ptz_to_screen_point(
		const sct_point_sp_t *ptz, const sct_point_sp_t *src,
		const sct_size_t *size, const fov_t *fov, int hflip, int vflip,
		sct_point_ca_t *dst)
{
	sct_point_ca_t tmpca;
	sct_point_ca_t ptzca;
	sct_point_ca_t tmp;

	DBG("src (r=%f, phi=%f, theta=%f)\n",
			src->r, RAD2DEG(src->phi), RAD2DEG(src->theta));

	sct_spherical_to_cartesian(src, &tmpca);
	sct_spherical_to_cartesian(ptz, &ptzca);

	DBG("tmpca (x=%f, y=%f, z=%f)\n", tmpca.x, tmpca.y, tmpca.z);
	DBG("ptzca (x=%f, y=%f, z=%f)\n", ptzca.x, ptzca.y, ptzca.z);

	double t = ptz->r / (tmpca.x * ptzca.x + tmpca.y * ptzca.y + tmpca.z * ptzca.z);

	// tmp is the intersection point of the screen (r = ptz->r)
	// and the line (0,0,0) to src
	tmp.x = t * tmpca.x;
	tmp.y = t * tmpca.y;
	tmp.z = t * tmpca.z;

	DBG("tmp (x=%f, y=%f, z=%f)\n", tmp.x, tmp.y, tmp.z);
	DBG("size (w=%f, h=%f, d=%f\n", size->w, size->h, size->d);

#if	0
	// tmp is the intersection point of the screen (r = r_scaled)
	// and the line (0,0,0) to tmp;
	/*
	 * ([0,0,0] + t[tmp.x, tmp.y, tmp.z])dot(ptzca.x, ptzca.y, ptzca.z) = r_scaled
	 */
	t = r_scaled / (tmp.x * ptzca.x + tmp.y * ptzca.y + tmp.z * ptzca.z);

	dst->x = t * tmp.x;
	dst->y = t * tmp.y;
	dst->z = t * tmp.z;
#endif	/*0*/

	double r_scaled = (size->w / 2) / tan(fov->h / 2);

	DBG("r_scaled=%f\n", r_scaled);

	tmpca.x = r_scaled * tmp.x / src->r;
	tmpca.y = r_scaled * tmp.y / src->r;
	tmpca.z = r_scaled * tmp.z / src->r;

	// rotate to top view (orthogonal to z-axis)
	sct_roll(&tmpca, -ptz->phi, &tmp);
	DBG("roll (x=%f, y=%f, z=%f) -> (x=%f, y=%f, z=%f)\n",
			tmpca.x, tmpca.y, tmpca.z, tmp.x, tmp.y, tmp.z);
	sct_yaw(&tmp, -ptz->theta, &tmpca);
	DBG("yaw (x=%f, y=%f, z=%f) -> (x=%f, y=%f, z=%f)\n",
			tmp.x, tmp.y, tmp.z, tmpca.x, tmpca.y, tmpca.z);

	dst->x = tmpca.y;
	dst->y = tmpca.x;
	dst->z = tmpca.z;

#if	0
	/* scaled final result */
//	dst->x = (tmp.x / src->r) * (size->w / 2) / tan(fov->h / 2) + size->w / 2;
	dst->x = (tmp.x / ptz->r) * (size->w / 2) / tan(fov->h / 2);
	if (hflip)
		dst->x = size->w - dst->x;

	dst->y = (tmp.y / ptz->r) * (size->h / 2) / tan(fov->h / 2);
	// w : h = y : y0
//	dst->y = size->w * dst->y / size->h + size->h / 2;
	dst->y = size->w * dst->y / size->h;
	if (vflip)
		dst->y = size->h - dst->y;

	dst->z = tmp.z;
	DBG("to screen (r=%f, phi=%f, theta=%f) -> (x=%f, y=%f, z=%f) t=%f\n",
			tmpsp.r, RAD2DEG(tmpsp.phi), RAD2DEG(tmpsp.theta),
			dst->x, dst->y, dst->z, t);
#endif	/*0*/
}
#endif	/*0*/

#if	0
void map_screen_to_ptz_point(
		const sct_point_sp_t *ptz, const sct_point_ca_t *src,
		const sct_size_t *size, const fov_t *fov, sct_point_sp_t *dst)
{
	sct_point_ca_t tmpca;

	tmpca.x = src->x - size->w / 2;
	/* y is increased to down */
	//tmpca.y = size->h / 2 - src->y;
	tmpca.y = src->y - size->h / 2;
	// src->z is ignored and the z should be calculated
	tmpca.z = size->w / (2 * tan(fov->h / 2));
	sct_cartesian_to_spherical(&tmpca, dst);

	/*
	 * Now dst points a point with assuming PTZ's theta and phi are all zero.
	 * So we should rotate to actul direction.
	 */
#if	1
	DBG("+ dst (r=%f, phi=%f, theta=%f)\n",
			dst->r, RAD2DEG(dst->phi), RAD2DEG(dst->theta));
#endif	/*0*/
	dst->phi += ptz->phi;
	dst->theta += ptz->theta;

	/*
	 * dst is always on the PTZ's spherical surface, so the radius should be
	 * ptz->r
	 */
	dst->r = ptz->r;
	DBG("dst (x=%f, y=%f, z=%f) -> (r=%f, phi=%f, theta=%f)\n",
			tmpca.x, tmpca.y, tmpca.z,
			dst->r, RAD2DEG(dst->phi), RAD2DEG(dst->theta));
}
#else
void map_screen_to_ptz_point(
		const sct_point_sp_t *ptz, const sct_point_ca_t *src,
		const sct_size_t *size, const fov_t *fov, sct_point_sp_t *dst)
{
	sct_point_ca_t tmpca, tmp;

#if	0
	tmpca.x = src->x - size->w / 2;
	/* y is increased to down */
	//tmpca.y = size->h / 2 - src->y;
	tmpca.y = src->y - size->h / 2;
	tmpca.z = src->z;
#else
	double r_scaled = (size->w / 2) / tan(fov->h / 2);

	DBG("r=%f, r_scaled=%f\n", ptz->r, r_scaled);
	// by top view, the suface's x-axis should be mapped to y-axis,
	// the surface's y-axis should be mapped to x-axis,
	tmpca.x = src->y * ptz->r / r_scaled;
	tmpca.y = src->x * ptz->r / r_scaled;
	// suppose the screen is a part of the surface whose equation is z=ptz->r
	// later we rotate the point by ptz->phi, ptz->theta
	tmpca.z = ptz->r;
#endif	/*0*/

#if	1

	DBG("src (x=%f, y=%f, z=%f)\n", src->x, src->y, src->z);
	DBG("tmpca (x=%f, y=%f, z=%f)\n", tmpca.x, tmpca.y, tmpca.z);
	// rotate the point by ptz
	//
	sct_yaw(&tmpca, ptz->theta, &tmp);
	DBG("yaw (x=%f, y=%f, z=%f) -> (x=%f, y=%f, z=%f)\n",
			tmpca.x, tmpca.y, tmpca.z, tmp.x, tmp.y, tmp.z);
	sct_cartesian_to_spherical(&tmp, dst);
#if	0
	DBG("after yaw (x=%f, y=%f, z=%f) -> (r=%f, phi=%f, theta=%f)\n",
			tmp.x, tmp.y, tmp.z,
			dst->r, RAD2DEG(dst->phi), RAD2DEG(dst->theta));
#endif	/*0*/
	sct_roll(&tmp, ptz->phi, &tmpca);
	DBG("roll (x=%f, y=%f, z=%f) -> (x=%f, y=%f, z=%f)\n",
			tmp.x, tmp.y, tmp.z, tmpca.x, tmpca.y, tmpca.z);

	sct_cartesian_to_spherical(&tmpca, dst);
#if	0
	DBG("after roll (x=%f, y=%f, z=%f) -> (r=%f, phi=%f, theta=%f)\n",
			tmpca.x, tmpca.y, tmpca.z,
			dst->r, RAD2DEG(dst->phi), RAD2DEG(dst->theta));
#endif	/*0*/
#endif	/*0*/

#if	0
	sct_cartesian_to_spherical(&tmpca, dst);
	DBG("src (x=%f, y=%f, z=%f) -> (r=%f, phi=%f, theta=%f)\n",
			tmpca.x, tmpca.y, tmpca.z,
			dst->r, RAD2DEG(dst->phi), RAD2DEG(dst->theta));

	dst->phi += ptz->phi;
	dst->theta += ptz->theta;
#endif	/*0*/

	/*
	 * dst is always on the PTZ's spherical surface, so the radius should be
	 * ptz->r
	 */
	// move the point onto the ptz's spherical surface.
//	dst->r = ptz->r;
	dst->r = 1.0;
	DBG("ptz->r=%f, dst->r=%f\n", ptz->r, dst->r);

	DBG("final (x=%f, y=%f, z=%f) -> (r=%f, phi=%f, theta=%f)\n",
			tmpca.x, tmpca.y, tmpca.z,
			dst->r, RAD2DEG(dst->phi), RAD2DEG(dst->theta));
}
#endif	/*0*/

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
		const sct_size_t *size, const fov_t *fov, rect_sp_t *dst)
{
	map_screen_to_ptz_point(ptz, &src->lt, size, fov, &dst->lt);
	map_screen_to_ptz_point(ptz, &src->rt, size, fov, &dst->rt);
	map_screen_to_ptz_point(ptz, &src->lb, size, fov, &dst->lb);
	map_screen_to_ptz_point(ptz, &src->rb, size, fov, &dst->rb);
}

void map_ptz_to_screen_rect(
		const sct_point_sp_t *ptz, const rect_sp_t *src,
		const sct_size_t *size, const fov_t *fov, int hflip, int vflip,
		rect_ca_t *dst)
{
	map_ptz_to_screen_point(ptz, &src->lt, size, fov, hflip, vflip, &dst->lt);
	map_ptz_to_screen_point(ptz, &src->rt, size, fov, hflip, vflip, &dst->rt);
	map_ptz_to_screen_point(ptz, &src->lb, size, fov, hflip, vflip, &dst->lb);
	map_ptz_to_screen_point(ptz, &src->rb, size, fov, hflip, vflip, &dst->rb);
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

void get_sp_fov_rect(
		const sct_point_sp_t *ptz, const fov_t *fov, rect_sp_t *rfov)
{
	rfov->lt.phi = rfov->lb.phi = ptz->phi - fov->h / 2;
	rfov->lt.theta = rfov->rt.theta = ptz->theta - fov->v / 2;
	rfov->rt.phi = rfov->rb.phi = ptz->phi + fov->h / 2;
	rfov->lb.theta = rfov->rb.theta = ptz->theta + fov->v / 2;
	rfov->lt.r = rfov->rt.r = rfov->lb.r = rfov->rb.r = ptz->r;
}

