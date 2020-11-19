#include <iostream>
#include "fractal.h"

Vec3b choose_color(size_t iter) {
	vector<Vec3b> color = {
		Vec3b(130, 18, 245),
		Vec3b(44, 245, 18),
		Vec3b(254, 183, 18),
		Vec3b(245, 14, 160),
		Vec3b(14, 121, 245),
		Vec3b(0, 0, 0)
	};
	if (iter < 7) return color[0];
	else if (iter < 14) return color[1];
	else if (iter < 21) return color[2];
	else if (iter < 28) return color[3];
	else if (iter < 35) return color[4];
	else return color[5];
}

void render(double zoom_lx, double zoom_rx, double zoom_ly, double zoom_ry, size_t max_iter,
	const char* winname, std::unique_ptr<BaseFractal> frac, size_t w = 500, size_t h = 500)
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
			im.at<Vec3b>(i, j) = choose_color(frac->fractal(max_iter, z));
			//fprintf(fp, "%c%c%c", uint8_t(r), uint8_t(g), uint8_t(b));
		}
	}
	//fclose(fp);
	imshow(winname, im);
	imwrite("picture.png", im);
}

int main()
{
	auto fz = [](complex<double> z) { return std::sin(z) - complex<double>(1., 0.); };
	auto dz = [](complex<double> z) { return std::cos(z); };

	//vector<complex<double>> roots = {
	//	complex<double>(1, 0),
	//	complex<double>(-(std::sqrt(5) + 1) / 4., -std::sqrt((5 - std::sqrt(5)) / 8.)),
	//	complex<double>((std::sqrt(5) - 1) / 4., std::sqrt((5 + std::sqrt(5)) / 8.)),
	//	complex<double>((std::sqrt(5) - 1) / 4., -std::sqrt((5 + std::sqrt(5)) / 8.)),
	//	complex<double>(-(1. + std::sqrt(5)) / 4., std::sqrt((5 - std::sqrt(5)) / 8.)),
	//};

	complex<double> a(1., 0);

	double zoom_lx = -2., zoom_rx = 2.,
		zoom_ly = -2., zoom_ry = 2.;

	render(zoom_lx, zoom_rx, zoom_ly, zoom_ry, 40, "newton frac", std::make_unique<Newton>(fz, dz, a));
	//render(-2, 1, -1.5, 1.5, 255, "mandelbrot frac", std::make_unique<Mandelbrot>());
	return 0;
}
