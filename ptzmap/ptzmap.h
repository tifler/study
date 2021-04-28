#ifndef	__PTZMAP_H__
#define	__PTZMAP_H__

#include "sct.h"

typedef struct {
	double h;
	double v;
} fov_t;

#if	1
void map_ptz_to_screen(
		const sct_point_sp_t *ptz, const sct_point_sp_t *src,
		const sct_size_2d_t *size, double fov, sct_point_2d_t *dst);

void map_screen_to_ptz(
		const sct_point_sp_t *ptz, const sct_point_2d_t *src,
		const sct_size_2d_t *size, double fov, sct_point_sp_t *dst);
#else
void map_ptz_to_screen(
		const sct_point_sp_t *ptz, const sct_point_sp_t *src,
		const sct_size_2d_t *size, fov_t *fov, sct_point_2d_t *dst);

void map_screen_to_ptz(
		const sct_point_sp_t *ptz, const sct_point_2d_t *src,
		const sct_size_2d_t *size, fov_t *fov, sct_point_sp_t *dst);
#endif	/*0*/

#endif	/*__PTZMAP_H__*/
