#include "../Headers/MainWindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
	ui->setupUi(this);
	connect(ui->settings, &QAction::triggered, settings, &QDialog::show);
	connect(ui->quit, &QAction::triggered, this, &QGuiApplication::quit);
	connect(ui->image, &InteractiveLabel::doubleClicked, this, &MainWindow::zoom);
	auto *validator = new QDoubleValidator();
	ui->re->setValidator(validator);
	ui->im->setValidator(validator);
	ui->zoom->setValidator(validator);
	settings->ui->juliaComplexRe->setValidator(validator);
	settings->ui->juliaComplexIm->setValidator(validator);
	settings->ui->phoenixComplexRe->setValidator(validator);
	settings->ui->phoenixComplexIm->setValidator(validator);
	display();
}

void MainWindow::display() {
	if (worker->isRunning()) {return;}
	worker = QThread::create([this] {
		cv::Mat3b canvas = generate(600, 800);
		cv::cvtColor(canvas, canvas, cv::COLOR_BGR2RGB);
		ui->image->setPixmap(QPixmap::fromImage(QImage(canvas.data, canvas.cols, canvas.rows, QImage::Format_RGB888)));
	});
	worker->start();
}

cv::Mat3b MainWindow::generate(int height, int width) {
	cv::Mat3b canvas = cv::Mat3b(height, width);
	cv::theRNG().state = settings->ui->seed->text().toUInt();
	Fractal fractal = CONSTRUCT(ui->fractal->currentText().toUtf8().constData())();
	connect(&fractal, &Fractal::progress, ui->progress, &QProgressBar::setValue);
	auto start = std::chrono::high_resolution_clock::now();
	fractal.paint(canvas, ui->re->text().toDouble(), ui->im->text().toDouble(), ui->zoom->text().toDouble());
	auto end = std::chrono::high_resolution_clock::now();
	ui->lcd->display((int) std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count());
	return canvas;
}

void MainWindow::render() {
	if (worker->isRunning()) {return;}
	bool resolutionConfirmed;
	int resolution = QInputDialog::getInt(this, "Resolution Dialog", "Vertical Height in Pixels:", 1440, 3, 21000, 3, &resolutionConfirmed);
	if (!resolutionConfirmed) {return;}
	QString filename = QFileDialog::getSaveFileName(this, "Save Image", "fractal.png", "Image Files (*.jpg *.png)");
	if (filename.isEmpty()) {return;}
	worker = QThread::create([this, resolution, filename] {
		cv::imwrite(filename.toUtf8().constData(), generate(resolution, resolution / 3 * 4));
	});
	worker->start();
}

void MainWindow::reset() {
	std::map<std::string, std::array<double, 3>> defaults;
	defaults.insert(std::pair("Burning Ship", std::array<double, 3>{-0.42, 0.57, 1.2}));
	defaults.insert(std::pair("Julia", std::array<double, 3>{0, 0, 1.25}));
	defaults.insert(std::pair("Mandelbar", std::array<double, 3>{-0.35, 0, 0.8}));
	defaults.insert(std::pair("Mandelbrot", std::array<double, 3>{-0.75, 0, 1.25}));
	defaults.insert(std::pair("Manowar", std::array<double, 3>{-0.35, 0, 2.4}));
	defaults.insert(std::pair("Phoenix", std::array<double, 3>{0, 0, 1.5}));
	ui->re->setText(QString::number(defaults[ui->fractal->currentText().toUtf8().constData()][0]));
	ui->im->setText(QString::number(defaults[ui->fractal->currentText().toUtf8().constData()][1]));
	ui->zoom->setText(QString::number(defaults[ui->fractal->currentText().toUtf8().constData()][2]));
	ui->iterations->setValue(100);
	display();
}

void MainWindow::zoom(QPoint point) {
	double re = ui->re->text().toDouble() + 3.0 / 800 / ui->zoom->text().toDouble() * (point.x() - 400);
	double im = ui->im->text().toDouble() - 3.0 / 600 / ui->zoom->text().toDouble() * (point.y() - 300);
	ui->re->setText(QString::number(re));
	ui->im->setText(QString::number(im));
	ui->zoom->setText(QString::number(ui->zoom->text().toDouble() * 1.3));
	display();
}
