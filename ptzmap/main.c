#include <stdlib.h>
#include <math.h>
#include "ptzmap.h"
#include "XDebug.h"

#define RAD2DEG(rad)			((rad) * 180.0 / M_PI)
#define DEG2RAD(deg)			((deg) * M_PI / 180.0)

static void test1(void)
{
	sct_point_ca_t ca = { 100, 0, 0 };
	sct_point_sp_t sp;

	sct_cartesian_to_spherical(&ca, &sp);
	DBG("ca = (%f, %f, %f), sp = (%f, %f, %f)\n",
			ca.x, ca.y, ca.z,
			sp.r, RAD2DEG(sp.theta), RAD2DEG(sp.phi));
}

static void test2(void)
{
	sct_point_sp_t ptz = { .r = 2.0, .phi = DEG2RAD(0), .theta = DEG2RAD(90) };
	// point on the screen
	sct_point_ca_t src = { .x = 100, .y = 31, .z = -999999 };
	sct_size_t size = { .w = 1000, .h = 1000, .d = 0 };
	fov_t fov = { .h = DEG2RAD(90), .v = DEG2RAD(90) };
	sct_point_sp_t dst;

	DBG("ptz (r=%f, phi=%f, theta=%f)\n", ptz.r, RAD2DEG(ptz.phi), RAD2DEG(ptz.theta));
	DBG("src (x=%f, y=%f)\n", src.x, src.y);
	map_screen_to_ptz_point(&ptz, &src, &size, &fov, &dst);

	DBG("dst (r=%f, phi=%f, theta=%f)\n", dst.r, RAD2DEG(dst.phi), RAD2DEG(dst.theta));

	map_ptz_to_screen_point(&ptz, &dst, &size, &fov, 0, 0, &src);
	DBG("src (x=%f, y=%f)\n", src.x, src.y);
}

static void test3(void)
{
#if	0
	sct_point_sp_t ptz = { .r = 1.0, .theta = DEG2RAD(0), .phi = DEG2RAD(0) };
	sct_point_2d_t src = { .x = 0, .y = -10 };
	sct_size_2d_t size = { .w = 1920, .h = 1080 };
	fov_t fov = { .v = DEG2RAD(60), .h= DEG2RAD(33.75) };
	sct_point_sp_t dst;

	DBG("src (x=%f, y=%f)\n", src.x, src.y);
	map_screen_to_ptz_point(&ptz, &src, &size, &fov, &dst);

	DBG("ptz (r=%f, theta=%f, phi=%f)\n", ptz.r, RAD2DEG(ptz.theta), RAD2DEG(ptz.phi));
	DBG("dst (r=%f, theta=%f, phi=%f)\n", dst.r, RAD2DEG(dst.theta), RAD2DEG(dst.phi));

	map_ptz_to_screen_point(&ptz, &dst, &size, &fov, &src);
	DBG("src (x=%f, y=%f)\n", src.x, src.y);
#endif	/*0*/
}

static void test4(void)
{
#if	0
	sct_point_sp_t ptz = { .r = 1.0, .theta = DEG2RAD(0), .phi = DEG2RAD(0) };
	sct_point_2d_t src = { .x = 0, .y = 0 };
	sct_size_2d_t size = { .w = 1920, .h = 1080 };
	fov_t fov = { .v = DEG2RAD(60), .h= DEG2RAD(33.75) };
	sct_point_sp_t dst;

	srand(time(NULL));
	for (int y = 0; y < size.h; y++) {
		for (int x = 0; x < size.w; x++) {
			src.x = x;
			src.y = y;

#define	RANDOM_TEST_COUNT			100
			for (int r = 0; r < RANDOM_TEST_COUNT; r++) {
				ptz.theta = DEG2RAD(random() % 360);
				ptz.phi = DEG2RAD(random() % 180);
				map_screen_to_ptz_point(&ptz, &src, &size, &fov, &dst);
				sct_point_2d_t tmp;
				map_ptz_to_screen_point(&ptz, &dst, &size, &fov, &tmp);
				if (fabs(src.x - tmp.x) > 1e-9 ||
						fabs(src.y - tmp.y) > 1e-9) {
					DBG("Not identical (x=%d, y=%d) (%f, %f) (%f, %f) d (%f, %f)\n",
							x, y,
							src.x, src.y, tmp.x, tmp.y,
							fabs(src.x - tmp.x), fabs(src.y - tmp.y));
				}
			}
		}
		DBG("Done row %d\n", y);
	}
#endif	/*0*/
}

static void test5(void)
{
#if	0
	sct_point_sp_t ptz = { .r = 1.0, .theta = DEG2RAD(0), .phi = DEG2RAD(0) };
	sct_point_2d_t src = { .x = 0, .y = 0 };
	sct_size_2d_t size = { .w = 1920, .h = 1080 };
	fov_t fov = { .v = DEG2RAD(60), .h= DEG2RAD(33.75) };
	sct_point_sp_t dst;

	for (int y = 0; y < size.h; y++) {
		for (int x = 0; x < size.w; x++) {
			src.x = x;
			src.y = y;

			for (int theta = 0; theta < 360; theta++) {
				for (int phi = 0; phi < 180; phi++) {
					ptz.theta = DEG2RAD(theta);
					ptz.phi = DEG2RAD(phi);
					map_screen_to_ptz_point(&ptz, &src, &size, &fov, &dst);
					sct_point_2d_t tmp;
					map_ptz_to_screen_point(&ptz, &dst, &size, &fov, &tmp);
					if (fabs(src.x - tmp.x) > 1e-9 ||
							fabs(src.y - tmp.y) > 1e-9) {
						DBG("Not identical (x=%d, y=%d) (%f, %f) (%f, %f) d (%f, %f)\n",
								x, y,
								src.x, src.y, tmp.x, tmp.y,
								fabs(src.x - tmp.x), fabs(src.y - tmp.y));
					}
				}
			}
		}
		DBG("Done row %d\n", y);
	}
#endif	/*0*/
}

int main(int argc, char **argv)
{
	DBG("=======================================\n");
	test1();
	DBG("=======================================\n");
	test2();
	DBG("=======================================\n");
	test3();
	DBG("=======================================\n");
	//test4();

	return 0;
}
