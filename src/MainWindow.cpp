#include "MainWindow.h"


MainWindow::MainWindow(QWidget *parent)
: QMainWindow(parent) {
	ui.setupUi(this);
	resetView();
	connect(ui.generateButton, &QPushButton::clicked, this, &MainWindow::generate);
	connect(ui.fractal, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::resetView);
	connect(ui.save, &QAction::triggered, this, &MainWindow::save);
}

void MainWindow::generate() {
	cv::Mat3b canvas(600, 800);
	QThread *thread = QThread::create([this, canvas]{
		generator(canvas);
		cv::cvtColor(canvas, canvas, cv::COLOR_BGR2RGB);
		QPixmap image = QPixmap::fromImage(QImage(canvas.data, canvas.cols, canvas.rows, QImage::Format_RGB888));
		ui.imageLabel->setPixmap(image);
	});
	ui.centerIm->setSingleStep(0.1 / ui.zoom->value());
	ui.centerRe->setSingleStep(0.1 / ui.zoom->value());
	ui.zoom->setSingleStep(0.1 * ui.zoom->value());
	thread->start();
}

void MainWindow::resetView() {
	std::map<std::string, std::pair<std::array<double, 3>, std::function<void(cv::Mat3b)>>> fractalMap;
	fractalMap.insert(std::pair("Burning Ship Fractal", std::pair(std::array<double, 3>{-0.45, 0.55, 1}, [this] (auto canvas) {
		BurningShip fractal(ui.iterations->value(), 10, true);
		fractal.paint(canvas, ui.centerRe->value(), ui.centerIm->value(), ui.zoom->value());
	})));
	fractalMap.insert(std::pair("Julia Set", std::pair(std::array<double, 3>{0, 0, 1.2}, [this] (auto canvas) {
		Julia fractal(ui.iterations->value(), 10, true, std::complex<double>{ui.juliaComplexRe->value(), ui.juliaComplexIm->value()});
		fractal.paint(canvas, ui.centerRe->value(), ui.centerIm->value(), ui.zoom->value());
	})));
	fractalMap.insert(std::pair("Mandelbrot Set", std::pair(std::array<double, 3>{-0.75, 0, 1}, [this] (auto canvas) {
		Mandelbrot fractal(ui.iterations->value(), 10, true);
		fractal.paint(canvas, ui.centerRe->value(), ui.centerIm->value(), ui.zoom->value());
	})));
	fractalMap.insert(std::pair("Manowar Fractal", std::pair(std::array<double, 3>{-0.35, 0, 1.7}, [this] (auto canvas) {
		Manowar fractal(ui.iterations->value(), 10, true);
		fractal.paint(canvas, ui.centerRe->value(), ui.centerIm->value(), ui.zoom->value());
	})));
	fractalMap.insert(std::pair("Newton Fractal", std::pair(std::array<double, 3>{0, 0, 1}, [this] (auto canvas) {
		Newton fractal(ui.iterations->value());
		fractal.paint(canvas, ui.centerRe->value(), ui.centerIm->value(), ui.zoom->value());
	})));
	fractalMap.insert(std::pair("Phoenix Fractal", std::pair(std::array<double, 3>{0, 0, 1.1}, [this] (auto canvas) {
		Phoenix fractal(ui.iterations->value(), 10, true, std::complex<double>{ui.phoenixComplexRe->value(), ui.phoenixComplexIm->value()});
		fractal.paint(canvas, ui.centerRe->value(), ui.centerIm->value(), ui.zoom->value());
	})));
	ui.centerRe->setValue(fractalMap[ui.fractal->currentText().toUtf8().constData()].first[0]);
	ui.centerIm->setValue(fractalMap[ui.fractal->currentText().toUtf8().constData()].first[1]);
	ui.zoom->setValue(fractalMap[ui.fractal->currentText().toUtf8().constData()].first[2]);
	generator = fractalMap[ui.fractal->currentText().toUtf8().constData()].second;
	if (ui.fractal->currentText() == "Julia Set") {
		ui.juliaComplexRe->show();
		ui.juliaComplexIm->show();
		ui.phoenixComplexRe->hide();
		ui.phoenixComplexIm->hide();
		ui.separator3->show();
	} else if (ui.fractal->currentText() == "Phoenix Fractal") {
		ui.juliaComplexRe->hide();
		ui.juliaComplexIm->hide();
		ui.phoenixComplexRe->show();
		ui.phoenixComplexIm->show();
		ui.separator3->show();
	} else {
		ui.juliaComplexRe->hide();
		ui.juliaComplexIm->hide();
		ui.phoenixComplexRe->hide();
		ui.phoenixComplexIm->hide();
		ui.separator3->hide();
	}
	generate();
}

void MainWindow::save() {
	QString fileName = QFileDialog::getSaveFileName(this, "Save Image");
	QThread *thread = QThread::create([this, fileName] {
		cv::Mat3b canvas(1440, 1920);
		generator(canvas);
		if (!fileName.isEmpty()) {
			cv::imwrite(fileName.toUtf8().constData(), canvas);
		}
	});
	thread->start();
}
