#include <iostream>
#include "fractal.h"

Vec3b lerp_color(Vec3b c1, Vec3b c2, double t) {
	return Vec3b(
		uchar(c1[0] + (c2[0] - c1[0]) * t),
		uchar(c1[1] + (c2[1] - c1[1]) * t),
		uchar(c1[2] + (c2[2] - c1[2]) * t)
	);
}

Vec3b choose_color_advanced(size_t iter, size_t max_iter) {
	if (iter == max_iter) return Vec3b(0, 0, 0);

	// 映射到 0.0 - 1.0 之间
	double mu = (double)iter / (double)max_iter;

	// 定义一个深邃的渐变：深蓝 -> 亮蓝 -> 金黄 -> 白色
	if (mu < 0.2) return lerp_color(Vec3b(50, 0, 0), Vec3b(255, 100, 0), mu / 0.2);
	if (mu < 0.5) return lerp_color(Vec3b(255, 100, 0), Vec3b(0, 200, 255), (mu - 0.2) / 0.3);
	return lerp_color(Vec3b(0, 200, 255), Vec3b(255, 255, 255), (mu - 0.5) / 0.5);
}

void render(double zoom_lx, double zoom_rx, double zoom_ly, double zoom_ry, size_t max_iter,
	const cv::String winname, std::unique_ptr<BaseFractal> frac, size_t w = 500, size_t h = 500)
{
	Mat im = Mat::zeros(Size(w, h), CV_8UC3);
	//FILE* fp = fopen("picture.ppm", "wb");
	//fprintf(fp, "P6 %d %d 255 ", w, h);
	for (size_t i = 0; i < h; i++)
	{
		double zy = zoom_ly + (zoom_ry - zoom_ly) * (double(i) / double(h - 1));
		for (size_t j = 0; j < w; j++)
		{
			double zx = zoom_lx + (zoom_rx - zoom_lx) * (double(j) / double(w - 1));
			complex<double> z(zx, zy);
			im.at<Vec3b>(i, j) = choose_color_advanced(frac->fractal(max_iter, z), max_iter);
			//fprintf(fp, "%c%c%c", uint8_t(r), uint8_t(g), uint8_t(b));
		}
	}
	//fclose(fp);
	//imshow(winname, im);
	//waitKey(0);
	imwrite(winname + ".png", im);
}

int main()
{
	//auto fz = [](complex<double> z) { return std::pow(z, 5.) - complex<double>(1., 0.); };
	//auto dz = [](complex<double> z) { return 5. * std::pow(z, 4); };

	//vector<complex<double>> roots = {
	//	complex<double>(1, 0),
	//	complex<double>(-(std::sqrt(5) + 1) / 4., -std::sqrt((5 - std::sqrt(5)) / 8.)),
	//	complex<double>((std::sqrt(5) - 1) / 4., std::sqrt((5 + std::sqrt(5)) / 8.)),
	//	complex<double>((std::sqrt(5) - 1) / 4., -std::sqrt((5 + std::sqrt(5)) / 8.)),
	//	complex<double>(-(1. + std::sqrt(5)) / 4., std::sqrt((5 - std::sqrt(5)) / 8.)),
	//};

	//complex<double> a(.5, 0);

	//double zoom_lx = -2., zoom_rx = 2.,
	//	zoom_ly = -2., zoom_ry = 2.;

	//render(zoom_lx, zoom_rx, zoom_ly, zoom_ry, 40, "newton_frac", std::make_unique<Newton>(fz, dz, a));
	//render(-2, 1, -1.5, 1.5, 255, "mandelbrot_frac", std::make_unique<Mandelbrot>());
	// 著名的 "Seahorse Valley" (海马谷) 坐标附近
	double center_x = -0.743643887037158;
	double center_y = 0.131825904205312;
	double range = 0.0001; // 尝试不断缩小这个 range
	render(center_x - range, center_x + range, center_y - range, center_y + range, 10000, "zoom_test", std::make_unique<Mandelbrot>());
	return 0;
}
