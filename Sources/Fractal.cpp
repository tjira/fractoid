#include "../Headers/Fractal.h"

Fractal::Fractal(std::function<cv::Vec3b(double, double)> *function, double height) {
	this->function = *function;
    this->height = height;
}

void Fractal::paint(cv::Mat canvas, double pRe, double pIm, double zoom) const {
    double unit = height / canvas.rows / zoom;
    double origin[2] = {pRe - unit * canvas.cols / 2, pIm + unit * canvas.rows / 2};
    for (int row = 0; row < canvas.rows; row++) {
        for (int column = 0; column < canvas.cols; column++) {
            cv::Vec3b color = function(origin[0] + unit * column, -origin[1] + unit * row);
            canvas.ptr<cv::Vec3b>(row)[column] = color;
        }
    }
}

Fractal Fractal::BurningShip(int iterations, int escape, bool smooth) {
	auto *randomizer = new cv::Mat2d(3, 2);
	randu(*randomizer, cv::Scalar(0), cv::Scalar(1));
	std::function<cv::Vec3b(double, double)> function = [iterations, escape, smooth, randomizer] (double pRe, double pIm) {
		int n = 0;
		double zRe = 0, zIm = 0, zReTemp;
		while (zRe * zRe + zIm * zIm < escape * escape && n < iterations) {
			zReTemp = zRe;
			zRe = zRe * zRe - zIm * zIm + pRe;
			zIm = 2 * abs(zReTemp * zIm) + pIm;
			n++;
		}
		SMOOTH; return (n == iterations) ? cv::Vec3b(0, 0, 0) : COLOR;
	};
	return Fractal(&function, 2.6);
}

Fractal Fractal::Julia(int iterations, int escape, bool smooth, std::complex<double> c) {
	auto *randomizer = new cv::Mat2d(3, 2);
	randu(*randomizer, cv::Scalar(0), cv::Scalar(1));
	std::function<cv::Vec3b(double, double)> function = [iterations, escape, smooth, cRe = c.real(), cIm = c.imag(), randomizer] (double pRe, double pIm) {
		int n = 0;
		double zRe = pRe, zIm = pIm, zReTemp;
		while (zRe * zRe + zIm * zIm < escape * escape && n < iterations) {
			zReTemp = zRe;
			zRe = zRe * zRe - zIm * zIm + cRe;
			zIm = 2 * zReTemp * zIm + cIm;
			n++;
		}
		SMOOTH; return (n == iterations) ? cv::Vec3b(0, 0, 0) : COLOR;
	};
	return Fractal(&function, 2.9);
}

Fractal Fractal::Mandelbrot(int iterations, int escape, bool smooth) {
	auto *randomizer = new cv::Mat2d(3, 2);
	randu(*randomizer, cv::Scalar(0), cv::Scalar(1));
	std::function<cv::Vec3b(double, double)> function = [iterations, escape, smooth, randomizer] (double pRe, double pIm) {
		int n = 0;
		double zRe = 0, zIm = 0, zReTemp;
		while (zRe * zRe + zIm * zIm < escape * escape && n < iterations) {
			zReTemp = zRe;
			zRe = zRe * zRe - zIm * zIm + pRe;
			zIm = 2 * zReTemp * zIm + pIm;
			n++;
		}
		SMOOTH; return (n == iterations) ? cv::Vec3b(0, 0, 0) : COLOR;
	};
	return Fractal(&function, 2.4);
}

Fractal Fractal::Manowar(int iterations, int escape, bool smooth) {
	auto *randomizer = new cv::Mat2d(3, 2);
	randu(*randomizer, cv::Scalar(0), cv::Scalar(1));
	std::function<cv::Vec3b(double, double)> function = [iterations, escape, smooth, randomizer] (double pRe, double pIm) {
		int n = 0;
		double zRe = pRe, zIm = pIm, zRePrev = pRe, zImPrev = pIm, zReTemp, zImTemp;
		while (zRe * zRe + zIm * zIm < escape * escape && n < iterations) {
			zReTemp = zRe;
			zImTemp = zIm;
			zRe = zRe * zRe - zIm * zIm + zRePrev + pRe;
			zIm = 2 * zReTemp * zIm +zImPrev + pIm;
			zRePrev = zReTemp;
			zImPrev = zImTemp;
			n++;
		}
		SMOOTH; return (n == iterations) ? cv::Vec3b(0, 0, 0) : COLOR;
	};
	return Fractal(&function, 2.2);
}

Fractal Fractal::Newton(int iterations) {
	auto *randomizer = new cv::Mat2d(3, 2);
	randu(*randomizer, cv::Scalar(0), cv::Scalar(1));
	std::function<cv::Vec3b(double, double)> function = [iterations, randomizer] (double pRe, double pIm) {
		int n = 0;
		double zRe = pRe, zIm = pIm, zReTemp, denominator;
		while (n < iterations && ((zRe - 1) * (zRe - 1) + zIm * zIm > 0.001) && ((zRe + 0.5) * (zRe + 0.5) + (zIm + sqrt(3) / 2) * (zIm + sqrt(3) / 2) > 0.001) && ((zRe + 0.5) * (zRe + 0.5) + (zIm - sqrt(3) / 2) * (zIm - sqrt(3) / 2) > 0.001)) {
			zReTemp = zRe;
			denominator = 3 * (zRe * zRe + zIm * zIm) * (zRe * zRe + zIm * zIm);
			zRe = zRe - ((-zRe * zRe + zRe * zRe * zRe * zRe * zRe + zIm * zIm + 2 * zRe * zRe * zRe * zIm * zIm + zIm * zIm * zIm * zIm * zRe) / denominator);
			zIm = zIm - (2 * zReTemp * zIm + zReTemp * zReTemp * zReTemp * zReTemp * zIm + 2 * zReTemp * zReTemp * zIm * zIm * zIm + zIm * zIm * zIm * zIm * zIm) / denominator;
			n++;
		}
		auto sn = (double) n;
		return (n == iterations) ? cv::Vec3b(0, 0, 0) : COLOR;
	};
	return Fractal(&function, 3);
}

Fractal Fractal::Phoenix(int iterations, int escape, bool smooth, std::complex<double> c) {
	auto *randomizer = new cv::Mat2d(3, 2);
	randu(*randomizer, cv::Scalar(0), cv::Scalar(1));
	std::function<cv::Vec3b(double, double)> function = [iterations, escape, smooth, cRe = c.real(), cIm = c.imag(), randomizer] (double pRe, double pIm) {
		int n = 0;
		double zRe = -pIm, zIm = pRe, zRePrev = cRe, zImPrev = cIm, zReTemp, zImTemp;
		while (zRe * zRe + zIm * zIm < escape * escape && n < iterations) {
			zReTemp = zRe;
			zImTemp = zIm;
			zRe = zRe * zRe - zIm * zIm + 0.56667 - 0.5 * zRePrev;
			zIm = 2 * zReTemp * zIm - 0.5 * zImPrev;
			zRePrev = zReTemp;
			zImPrev = zImTemp;
			n++;
		}
		SMOOTH; return (n == iterations) ? cv::Vec3b(0, 0, 0) : COLOR;
	};
	return Fractal(&function, 2.4);
}
