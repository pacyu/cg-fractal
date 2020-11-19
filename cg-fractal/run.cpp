#include <iostream>
#include <complex>
#include <opencv2/opencv.hpp>

using namespace cv;
using std::vector;
using std::complex;

class BaseFractal {
public:
	virtual size_t fractal(size_t max_iter, complex<double> z) = 0;
};

class Mandelbrot : public BaseFractal {
public:
	size_t fractal(size_t max_iter, complex<double> z)
	{
		complex<double> c = z;
		for (int k = 0; k < max_iter; k++)
		{
			if (std::abs(z) > 2.) return k;
			z = std::pow(z, 2.) + c;
		}
		return max_iter;
	}
};

class Newton : public BaseFractal {
public:
	Newton(std::function<std::complex<double>(complex<double>)> fz,
		std::function<std::complex<double>(complex<double>)> dz,
		complex<double> a) :
		roots(), fz(fz), dz(dz), a(a) {}

	Newton(vector<Vec3b> color,
		std::function<std::complex<double>(complex<double>)> fz,
		std::function<std::complex<double>(complex<double>)> dz,
		complex<double> a) :
		roots(), fz(fz), dz(dz), a(a) {}

	Newton(vector<complex<double>> roots,
		std::function<std::complex<double>(complex<double>)> fz,
		std::function<std::complex<double>(complex<double>)> dz,
		complex<double> a) :
		roots(roots), fz(fz), dz(dz), a(a) {}

	size_t fractal(size_t max_iter, complex<double> z)
	{
		for (size_t k = 0; k < max_iter; k++)
		{
			if (std::abs(z) > 2.0)
			{
				return k;
			}
			z -= a * fz(z) / dz(z);
			//for (size_t r = 0; r < 5; r++)
			//{
			//	complex<double> distance = z - roots[r];
			//	if (std::abs(distance) < 1e-6)
			//		return Vec3b(
			//			(color[r][0] * k) % 256,
			//			(color[r][1] * k) % 256,
			//			(color[r][2] * k) % 256);
			//}
		}
		return max_iter;
	}

private:
	vector<complex<double>> roots;
	std::function<std::complex<double>(complex<double>)> fz, dz;
	complex<double> a;
};

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
	auto fz = [](complex<double> z) { return std::pow(z, 5.) - complex<double>(1., 0.); };
	auto dz = [](complex<double> z) { return 5. * std::pow(z, 4.); };

	vector<complex<double>> roots = {
		complex<double>(1, 0),
		complex<double>(-(std::sqrt(5) + 1) / 4., -std::sqrt((5 - std::sqrt(5)) / 8.)),
		complex<double>((std::sqrt(5) - 1) / 4., std::sqrt((5 + std::sqrt(5)) / 8.)),
		complex<double>((std::sqrt(5) - 1) / 4., -std::sqrt((5 + std::sqrt(5)) / 8.)),
		complex<double>(-(1. + std::sqrt(5)) / 4., std::sqrt((5 - std::sqrt(5)) / 8.)),
	};

	complex<double> a(-0.5, 0);

	double zoom_lx = -1., zoom_rx = 1.,
		zoom_ly = -1., zoom_ry = 1.;

	render(zoom_lx, zoom_rx, zoom_ly, zoom_ry, 40, "newton frac", std::make_unique<Newton>(fz, dz, a));

	return 0;
}
