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
	sct_point_sp_t ptz = { .r = 2.0, .theta = DEG2RAD(0), .phi = DEG2RAD(0) };
	sct_point_2d_t src = { .x = 0, .y = 0 };
	sct_size_2d_t size = { .w = 1000, .h = 1000 };
	double fov = DEG2RAD(60);
	sct_point_sp_t dst;
	map_screen_to_ptz(&ptz, &src, &size, fov, &dst);

	DBG("ptz (r=%f, theta=%f, phi=%f)\n", ptz.r, RAD2DEG(ptz.theta), RAD2DEG(ptz.phi));
	DBG("dst (r=%f, theta=%f, phi=%f)\n", dst.r, RAD2DEG(dst.theta), RAD2DEG(dst.phi));

	map_ptz_to_screen(&ptz, &dst, &size, fov, &src);
	DBG("src (x=%f, y=%f)\n", src.x, src.y);
}

static void test3(void)
{
	sct_point_sp_t ptz = { .r = 2.0, .theta = DEG2RAD(0), .phi = DEG2RAD(0) };
	sct_point_2d_t src = { .x = 960, .y = 540 };
	sct_size_2d_t size = { .w = 1000, .h = 1000 };
	double fov = DEG2RAD(60);
	sct_point_sp_t dst;
	map_screen_to_ptz(&ptz, &src, &size, fov, &dst);

	DBG("ptz (r=%f, theta=%f, phi=%f)\n", ptz.r, RAD2DEG(ptz.theta), RAD2DEG(ptz.phi));
	DBG("dst (r=%f, theta=%f, phi=%f)\n", dst.r, RAD2DEG(dst.theta), RAD2DEG(dst.phi));

	map_ptz_to_screen(&ptz, &dst, &size, fov, &src);
	DBG("src (x=%f, y=%f)\n", src.x, src.y);
}

static void test4(void)
{
	sct_point_sp_t ptz = { .r = 2.0, .theta = DEG2RAD(15), .phi = DEG2RAD(17) };
	sct_point_2d_t src = { .x = 1920, .y = 1080 };
	sct_size_2d_t size = { .w = 1920, .h = 1080 };
	double fov = DEG2RAD(60);
	sct_point_sp_t dst;
	map_screen_to_ptz(&ptz, &src, &size, fov, &dst);

	DBG("ptz (r=%f, theta=%f, phi=%f)\n", ptz.r, RAD2DEG(ptz.theta), RAD2DEG(ptz.phi));
	DBG("dst (r=%f, theta=%f, phi=%f)\n", dst.r, RAD2DEG(dst.theta), RAD2DEG(dst.phi));

	map_ptz_to_screen(&ptz, &dst, &size, fov, &src);
	DBG("src (x=%f, y=%f)\n", src.x, src.y);
}

int main(int argc, char **argv)
{
	test1();
	test2();
	test3();
	test4();

	return 0;
}
