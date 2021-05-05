#include <math.h>
#include "ptzmap.h"
#include "XDebug.h"

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
		const sct_size_t *size, const fov_t *fov, int hflip, int vflip,
		sct_point_ca_t *dst)
{
	sct_point_ca_t ptzca, srcca, tmp;

	sct_spherical_to_cartesian(src, &srcca);
	sct_spherical_to_cartesian(ptz, &ptzca);

	double t = ptz->r / (srcca.x * ptzca.x + srcca.y * ptzca.y + srcca.z * ptzca.z);

	// tmp is the intersection point of the screen (r = ptz->r)
	// and the line (0,0,0) to src
	tmp.x = t * srcca.x;
	tmp.y = t * srcca.y;
	tmp.z = t * srcca.z;
//	DBG("src (x=%f, y=%f, z=%f, ptz->r=%f)\n", tmp.x, tmp.y, tmp.z, ptz->r);

	// rotate to top view (orthogonal to z-axis)
	sct_roll(&tmp, -ptz->phi, &srcca);
	sct_yaw(&srcca, -ptz->theta, &tmp);

//	DBG("top (x=%f, y=%f, z=%f, ptz->r=%f)\n", tmp.x, tmp.y, tmp.z, ptz->r);

	/*
	 * tan(fov->h / 2) = (w_fov / 2) / tmp.z
	 */
	double w_fov = 2 * tmp.z * tan(fov->h / 2);

//	DBG("w_fov = %f, ptz->r = %f, fov->h = %f\n", w_fov, ptz->r, fov->h);

	dst->x = (tmp.y + w_fov / 2) * size->w / w_fov;
	dst->y = (tmp.x + w_fov / 2) * size->h / w_fov;

//	DBG("swap-scale (x=%f, y=%f)\n", dst->x, dst->y);

	if (hflip)
		dst->x = size->w - dst->x;

	if (vflip)
		dst->y = size->h - dst->y;
		//dst->y = size->h / 2 - dst->y;

//	DBG("flip(h:%d, v:%d) (x=%f, y=%f)\n", hflip, vflip, dst->x, dst->y);
}

void map_screen_to_ptz_point(
		const sct_point_sp_t *ptz, const sct_point_ca_t *src,
		const sct_size_t *size, const fov_t *fov, sct_point_sp_t *dst)
{
	sct_point_ca_t tmpca, tmp;

	double r_scaled = (size->w / 2) / tan(fov->h / 2);

	/*
	 * We're going to put the screen to be orthogonal to z-axis.
	 * To do that pan -ptz->phi and tilt -ptz->theta.
	 * Through the above operation, the suface's x-axis is mapped to y-axis
	 * and the surface's y-axis is mapped to x-axis,
	 */
	tmpca.x = (src->y - size->h / 2) * ptz->r / r_scaled;
	tmpca.y = (src->x - size->w / 2) * ptz->r / r_scaled;

	// suppose the screen is a part of the surface whose equation is z=ptz->r
	// later we rotate the point by ptz->phi, ptz->theta
	tmpca.z = ptz->r;

	DBG("src (x=%f, y=%f, z=%f)\n", src->x, src->y, src->z);
	DBG("tmpca (x=%f, y=%f, z=%f)\n", tmpca.x, tmpca.y, tmpca.z);

	// rotate the point by ptz
	sct_yaw(&tmpca, ptz->theta, &tmp);
	sct_roll(&tmp, ptz->phi, &tmpca);

	sct_cartesian_to_spherical(&tmpca, dst);
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
#if	0
	map_ptz_to_screen_point(ptz, &src->lt, size, fov, hflip, vflip, &dst->lt);
	map_ptz_to_screen_point(ptz, &src->lb, size, fov, hflip, vflip, &dst->lb);
	map_ptz_to_screen_point(ptz, &src->rt, size, fov, hflip, vflip, &dst->rt);
	map_ptz_to_screen_point(ptz, &src->rb, size, fov, hflip, vflip, &dst->rb);
#else
	if (hflip && vflip) {
		map_ptz_to_screen_point(ptz, &src->lt, size, fov, hflip, vflip, &dst->rb);
		map_ptz_to_screen_point(ptz, &src->lb, size, fov, hflip, vflip, &dst->rt);
		map_ptz_to_screen_point(ptz, &src->rt, size, fov, hflip, vflip, &dst->lb);
		map_ptz_to_screen_point(ptz, &src->rb, size, fov, hflip, vflip, &dst->lt);
	} else if (hflip) {
		map_ptz_to_screen_point(ptz, &src->lt, size, fov, hflip, vflip, &dst->rt);
		map_ptz_to_screen_point(ptz, &src->lb, size, fov, hflip, vflip, &dst->rb);
		map_ptz_to_screen_point(ptz, &src->rt, size, fov, hflip, vflip, &dst->lt);
		map_ptz_to_screen_point(ptz, &src->rb, size, fov, hflip, vflip, &dst->lb);
	} else if (vflip) {
		map_ptz_to_screen_point(ptz, &src->lt, size, fov, hflip, vflip, &dst->lb);
		map_ptz_to_screen_point(ptz, &src->lb, size, fov, hflip, vflip, &dst->lt);
		map_ptz_to_screen_point(ptz, &src->rt, size, fov, hflip, vflip, &dst->rb);
		map_ptz_to_screen_point(ptz, &src->rb, size, fov, hflip, vflip, &dst->rt);
	} else {
		map_ptz_to_screen_point(ptz, &src->lt, size, fov, hflip, vflip, &dst->lt);
		map_ptz_to_screen_point(ptz, &src->lb, size, fov, hflip, vflip, &dst->lb);
		map_ptz_to_screen_point(ptz, &src->rt, size, fov, hflip, vflip, &dst->rt);
		map_ptz_to_screen_point(ptz, &src->rb, size, fov, hflip, vflip, &dst->rb);
	}
#endif	/*0*/
}

void get_cover_rect_2d(const rect_ca_t *src, rect_2d_t *dst)
{
	dst->left = MIN(src->lt.x, src->lb.x);
	dst->top = MIN(src->lt.y, src->rt.y);
	dst->right = MAX(src->rt.x, src->rb.x);
	dst->bottom = MAX(src->lb.y, src->rb.y);
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

int do_union_rect_2d(const rect_2d_t *a, const rect_2d_t *b)
{
	return
		MAX(0, MIN(a->right, b->right) - MAX(a->left, b->left)) &&
		MAX(0, MIN(a->bottom, b->bottom) - MAX(a->top, b->top));
}

int get_union_rect_2d(const rect_2d_t *a, const rect_2d_t *b, rect_2d_t *r)
{
	if (!do_union_rect_2d(a, b)) {
		r->left = r->right = r->top = r->bottom = 0.0;
		return 0;
	}

	r->left = MAX(a->left, b->left);
	r->top = MAX(a->top, b->top);
	r->right = MIN(a->right, b->right);
	r->bottom = MIN(a->bottom, b->bottom);

	return 1;
}
