#ifndef	__PTZMAP_H__
#define	__PTZMAP_H__

#include "sct.h"

#ifndef	MIN
#define	MIN(a, b)				((a) < (b) ? (a) : (b))
#endif	/*MIN*/

#ifndef	MAX
#define	MAX(a, b)				((a) > (b) ? (a) : (b))
#endif	/*MAX*/

typedef struct {
	double h;
	double v;
} fov_t;

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
		const sct_size_2d_t *size, const fov_t *fov, sct_point_2d_t *dst);

void map_screen_to_ptz_point(
		const sct_point_sp_t *ptz, const sct_point_2d_t *src,
		const sct_size_2d_t *size, const fov_t *fov, sct_point_sp_t *dst);

int is_point_in_fov(
		const sct_point_sp_t *ptz, const fov_t *fov, const sct_point_sp_t *pt);

void map_screen_to_ptz_rect(
		const sct_point_sp_t *ptz, const rect_ca_t *src,
		const sct_size_2d_t *size, const fov_t *fov, rect_sp_t *dst);

void map_ptz_to_screen_rect(
		const sct_point_sp_t *ptz, const rect_sp_t *src,
		const sct_size_2d_t *size, const fov_t *fov, rect_ca_t *dst);

#endif	/*__PTZMAP_H__*/
