#pragma once
#include <complex>
#include <functional>
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
	Mandelbrot() {}

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
	Newton(std::function<complex<double>(complex<double>)> fz,
		std::function<complex<double>(complex<double>)> dz,
		complex<double> a) :
		roots(), fz(fz), dz(dz), a(a) {}

	Newton(vector<Vec3b> color,
		std::function<complex<double>(complex<double>)> fz,
		std::function<complex<double>(complex<double>)> dz,
		complex<double> a) :
		roots(), fz(fz), dz(dz), a(a) {}

	Newton(vector<complex<double>> roots,
		std::function<complex<double>(complex<double>)> fz,
		std::function<complex<double>(complex<double>)> dz,
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
	std::function<complex<double>(complex<double>)> fz, dz;
	complex<double> a;
};