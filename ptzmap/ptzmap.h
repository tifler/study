#ifndef	__PTZMAP_H__
#define	__PTZMAP_H__

#include <math.h>
#include "sct.h"

#ifndef	MIN
#define	MIN(a, b)				((a) < (b) ? (a) : (b))
#endif	/*MIN*/

#ifndef	MAX
#define	MAX(a, b)				((a) > (b) ? (a) : (b))
#endif	/*MAX*/

#define RAD2DEG(rad)			((rad) * 180.0 / M_PI)
#define DEG2RAD(deg)			((deg) * M_PI / 180.0)

/*****************************************************************************/

typedef struct {
	double h;
	double v;
} fov_t;

typedef struct {
	double left;
	double right;
	double top;
	double bottom;
} rect_2d_t;

typedef struct {
	sct_point_ca_t lt;
	sct_point_ca_t rt;
	sct_point_ca_t lb;
	sct_point_ca_t rb;
} rect_ca_t;

typedef struct {
	sct_point_sp_t lt;
	sct_point_sp_t rt;
	sct_point_sp_t lb;
	sct_point_sp_t rb;
} rect_sp_t;

void map_ptz_to_screen_point(
		const sct_point_sp_t *ptz, const sct_point_sp_t *src,
		const sct_size_t *size, const fov_t *fov, int hflip, int vflip,
		sct_point_ca_t *dst);

void map_screen_to_ptz_point(
		const sct_point_sp_t *ptz, const sct_point_ca_t *src,
		const sct_size_t *size, const fov_t *fov, sct_point_sp_t *dst);

int is_point_in_fov(
		const sct_point_sp_t *ptz, const fov_t *fov, const sct_point_sp_t *pt);

void map_screen_to_ptz_rect(
		const sct_point_sp_t *ptz, const rect_ca_t *src,
		const sct_size_t *size, const fov_t *fov, rect_sp_t *dst);

void map_ptz_to_screen_rect(
		const sct_point_sp_t *ptz, const rect_sp_t *src,
		const sct_size_t *size, const fov_t *fov, int hflip, int vflip,
		rect_ca_t *dst);

void get_cover_rect_2d(const rect_ca_t *src, rect_2d_t *dst);
void get_cover_rect_ca(const rect_ca_t *src, rect_ca_t *dst);
int do_intersect_rect_sp(const rect_sp_t *a, const rect_sp_t *b);
int get_intersect_rect_sp(const rect_sp_t *a, const rect_sp_t *b, rect_sp_t *r);

void get_sp_fov_rect(
		const sct_point_sp_t *ptz, const fov_t *fov, rect_sp_t *rfov);

#endif	/*__PTZMAP_H__*/
