#include "../Headers/MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
: QMainWindow(parent) {
	ui = new Ui::MainWindow;
	ui->setupUi(this);
	settings = new SettingsWindow(this);
	constructors.insert(std::pair("Burning Ship Fractal", [this] {
		return Fractal::BurningShip(ui->iterationsSpin->value(), 10, true);}));
	constructors.insert(std::pair("Julia Set", [this] {
		return Fractal::Julia(ui->iterationsSpin->value(), 10, true, std::complex<double>{settings->ui->juliaComplexReSpin->value(), settings->ui->juliaComplexImSpin->value()});}));
	constructors.insert(std::pair("Mandelbrot Set", [this] {
		return Fractal::Mandelbrot(ui->iterationsSpin->value(), 10, true);}));
	constructors.insert(std::pair("Manowar Fractal", [this] {
		return Fractal::Manowar(ui->iterationsSpin->value(), 10, true);}));
	constructors.insert(std::pair("Newton Fractal", [this] {
		return Fractal::Newton(ui->iterationsSpin->value());}));
	constructors.insert(std::pair("Phoenix Fractal", [this] {
		return Fractal::Phoenix(ui->iterationsSpin->value(), 10, true, std::complex<double>{settings->ui->phoenixComplexReSpin->value(), settings->ui->phoenixComplexImSpin->value()});}));
	defaults.insert(std::pair("Burning Ship Fractal", std::array<double, 3>{-0.45, 0.55, 1.2}));
	defaults.insert(std::pair("Julia Set", std::array<double, 3>{0, 0, 1.25}));
	defaults.insert(std::pair("Mandelbrot Set", std::array<double, 3>{-0.75, 0, 1.25}));
	defaults.insert(std::pair("Manowar Fractal", std::array<double, 3>{-0.35, 0, 2.4}));
	defaults.insert(std::pair("Newton Fractal", std::array<double, 3>{0, 0, 1}));
	defaults.insert(std::pair("Phoenix Fractal", std::array<double, 3>{0, 0, 1.5}));
	connect(ui->generateButton, &QPushButton::clicked, this, &MainWindow::generate);
	connect(ui->fractalCombo, qOverload<int>(&QComboBox::currentIndexChanged), this, &MainWindow::reset);
	connect(ui->saveAsAction, &QAction::triggered, this, &MainWindow::saveAs);
	connect(ui->settingsAction, &QAction::triggered, settings, &QDialog::show);
	generate();
}

void MainWindow::generate() {
	cv::theRNG().state = 4294967295;
	fractal = constructors[ui->fractalCombo->currentText().toUtf8().constData()]();
	QThread *thread = QThread::create([this] {
		cv::Mat3b canvas = cv::Mat3b(600, 800);
		fractal.paint(canvas, ui->centerReSpin->value(), ui->centerImSpin->value(), ui->zoomSpin->value());
		cv::cvtColor(canvas, canvas, cv::COLOR_BGR2RGB);
		QPixmap image = QPixmap::fromImage(QImage(canvas.data, canvas.cols, canvas.rows, QImage::Format_RGB888));
		ui->imageLabel->setPixmap(image);
	});
	ui->centerImSpin->setSingleStep(0.1 / ui->zoomSpin->value());
	ui->centerReSpin->setSingleStep(0.1 / ui->zoomSpin->value());
	ui->zoomSpin->setSingleStep(0.1 * ui->zoomSpin->value());
	thread->start();
}

void MainWindow::reset() {
	ui->centerReSpin->setValue(defaults[ui->fractalCombo->currentText().toUtf8().constData()][0]);
	ui->centerImSpin->setValue(defaults[ui->fractalCombo->currentText().toUtf8().constData()][1]);
	ui->zoomSpin->setValue(defaults[ui->fractalCombo->currentText().toUtf8().constData()][2]);
	generate();
}

void MainWindow::saveAs() {
	QString fileName = QFileDialog::getSaveFileName(this, "Save Image", "fractal.png", "Image Files (*.jpg *.png)");
	fractal = constructors[ui->fractalCombo->currentText().toUtf8().constData()]();
	QThread *thread = QThread::create([this, fileName] {
		if (!fileName.isEmpty()) {
			cv::Mat3b canvas(1440, 1920);
			fractal.paint(canvas, ui->centerReSpin->value(), ui->centerImSpin->value(), ui->zoomSpin->value());
			cv::imwrite(fileName.toUtf8().constData(), canvas);
		}
	});
	thread->start();
}
