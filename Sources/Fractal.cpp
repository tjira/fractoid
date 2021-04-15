#include "../Headers/Fractal.h"

Fractal::Fractal(std::function<double(double, double)> *function) {
	this->function = *function;
}

Fractal::~Fractal() = default;

void Fractal::paint(cv::Mat canvas, double pRe, double pIm, double zoom) {
	cv::Mat2d randomizer = cv::Mat2d(3, 2);
	randu(randomizer, cv::Scalar(0), cv::Scalar(100));
    double unit = 3.0 / canvas.rows / zoom;
    double origin[2] = {pRe - unit * canvas.cols / 2, pIm + unit * canvas.rows / 2};
    for (int row = 0; row < canvas.rows; row++) {
    	emit progress((int) ((double) (row + 1) / canvas.rows * 100));
        for (int column = 0; column < canvas.cols; column++) {
            double sn = function(origin[0] + unit * column, -origin[1] + unit * row);
            canvas.ptr<cv::Vec3b>(row)[column] = (sn == 1) ? cv::Vec3b(0, 0, 0) : cv::Vec3b(
            	(int) ((sin(sn * randomizer.at<double>(0, 0) / 1.5 + randomizer.at<double>(0, 1)) + 1) * 127.5),
        		(int) ((sin(sn * randomizer.at<double>(1, 0) / 1.5 + randomizer.at<double>(1, 1)) + 1) * 127.5),
        		(int) ((sin(sn * randomizer.at<double>(2, 0) / 1.5 + randomizer.at<double>(2, 1)) + 1) * 127.5)
        	);
        }
    }
}

Fractal Fractal::BurningShip(int iterations, int escape, bool smooth) {
	std::function<double(double, double)> function = [iterations, escape, smooth] (double pRe, double pIm) {
		int n = 0;
		double zRe = 0, zIm = 0, zReTemp;
		while (zRe * zRe + zIm * zIm < escape * escape && n < iterations) {
			zReTemp = zRe;
			zRe = zRe * zRe - zIm * zIm + pRe;
			zIm = 2 * abs(zReTemp * zIm) + pIm;
			n++;
		}
		return ((smooth && n < iterations) ? n + 1 - log2(log(zRe * zRe + zIm * zIm) / 2) : (double) n) / iterations;
	};
	return Fractal(&function);
}

Fractal Fractal::Julia(int iterations, int escape, bool smooth, double cRe, double cIm) {
	std::function<double(double, double)> function = [iterations, escape, smooth, cRe, cIm] (double pRe, double pIm) {
		int n = 0;
		double zRe = pRe, zIm = pIm, zReTemp;
		while (zRe * zRe + zIm * zIm < escape * escape && n < iterations) {
			zReTemp = zRe;
			zRe = zRe * zRe - zIm * zIm + cRe;
			zIm = 2 * zReTemp * zIm + cIm;
			n++;
		}
		return ((smooth && n < iterations) ? n + 1 - log2(log(zRe * zRe + zIm * zIm) / 2) : (double) n) / iterations;
	};
	return Fractal(&function);
}

Fractal Fractal::Mandelbrot(int iterations, int escape, bool smooth) {
	std::function<double(double, double)> function = [iterations, escape, smooth] (double pRe, double pIm) {
		int n = 0;
		double zRe = 0, zIm = 0, zReTemp;
		while (zRe * zRe + zIm * zIm < escape * escape && n < iterations) {
			zReTemp = zRe;
			zRe = zRe * zRe - zIm * zIm + pRe;
			zIm = 2 * zReTemp * zIm + pIm;
			n++;
		}
		return ((smooth && n < iterations) ? n + 1 - log2(log(zRe * zRe + zIm * zIm) / 2) : (double) n) / iterations;
	};
	return Fractal(&function);
}

Fractal Fractal::Manowar(int iterations, int escape, bool smooth) {
	std::function<double(double, double)> function = [iterations, escape, smooth] (double pRe, double pIm) {
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
		return ((smooth && n < iterations) ? n + 1 - log2(log(zRe * zRe + zIm * zIm) / 2) : (double) n) / iterations;
	};
	return Fractal(&function);
}

Fractal Fractal::Phoenix(int iterations, int escape, bool smooth, double cRe, double cIm) {
	std::function<double(double, double)> function = [iterations, escape, smooth, cRe, cIm] (double pRe, double pIm) {
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
		return ((smooth && n < iterations) ? n + 1 - log2(log(zRe * zRe + zIm * zIm) / 2) : (double) n) / iterations;
	};
	return Fractal(&function);
}
