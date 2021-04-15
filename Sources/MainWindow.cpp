#include "../Headers/MainWindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
	ui = new Ui::MainWindow;
	ui->setupUi(this);
	settings = new SettingsWindow(this);
	constructors.insert(std::pair("Burning Ship Fractal", [this] {
		return Fractal::BurningShip(ui->iterationsSpin->value(), 10, true);}));
	constructors.insert(std::pair("Julia Set", [this] {
		return Fractal::Julia(ui->iterationsSpin->value(), 10, true, settings->ui->juliaComplexReSpin->value(), settings->ui->juliaComplexImSpin->value());}));
	constructors.insert(std::pair("Mandelbrot Set", [this] {
		return Fractal::Mandelbrot(ui->iterationsSpin->value(), 10, true);}));
	constructors.insert(std::pair("Manowar Fractal", [this] {
		return Fractal::Manowar(ui->iterationsSpin->value(), 10, true);}));
	constructors.insert(std::pair("Phoenix Fractal", [this] {
		return Fractal::Phoenix(ui->iterationsSpin->value(), 10, true, settings->ui->phoenixComplexReSpin->value(), settings->ui->phoenixComplexImSpin->value());}));
	defaults.insert(std::pair("Burning Ship Fractal", std::array<double, 3>{-0.45, 0.55, 1.2}));
	defaults.insert(std::pair("Julia Set", std::array<double, 3>{0, 0, 1.25}));
	defaults.insert(std::pair("Mandelbrot Set", std::array<double, 3>{-0.75, 0, 1.25}));
	defaults.insert(std::pair("Manowar Fractal", std::array<double, 3>{-0.35, 0, 2.4}));
	defaults.insert(std::pair("Phoenix Fractal", std::array<double, 3>{0, 0, 1.5}));
	auto *generateShortcut = new QShortcut(QKeySequence("Return"), this);
	connect(ui->saveAsAction, &QAction::triggered, this, &MainWindow::saveAs);
	connect(ui->settingsAction, &QAction::triggered, settings, &QDialog::show);
	connect(ui->quitAction, &QAction::triggered, this, &QGuiApplication::quit);
	connect(ui->fractalCombo, qOverload<int>(&QComboBox::currentIndexChanged), this, &MainWindow::reset);
	connect(ui->generateButton, &QPushButton::clicked, this, &MainWindow::generate);
	connect(settings->ui->seedSpin, qOverload<int>(&QSpinBox::valueChanged), this, &MainWindow::generate);
	connect(generateShortcut, &QShortcut::activated, this, &MainWindow::generate);
	generate();
}

MainWindow::~MainWindow() {
	delete settings;
	delete ui;
}

void MainWindow::generate() {
	if (worker->isRunning()) {return;}
	worker = QThread::create([this] {
		cv::theRNG().state = settings->ui->seedSpin->value();
		Fractal fractal = constructors[ui->fractalCombo->currentText().toUtf8().constData()]();
		connect(&fractal, &Fractal::progress, ui->progressBar, &QProgressBar::setValue);
		cv::Mat3b canvas = cv::Mat3b(600, 800);
		auto start = std::chrono::high_resolution_clock::now();
		fractal.paint(canvas, ui->centerReSpin->value(), ui->centerImSpin->value(), ui->zoomSpin->value());
		auto end = std::chrono::high_resolution_clock::now();
		qDebug() << "Generated in" << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms.";
		ui->imageLabel->setPixmap(QPixmap::fromImage(QImage(canvas.data, canvas.cols, canvas.rows, QImage::Format_RGB888)));
	});
	ui->centerImSpin->setSingleStep(0.1 / ui->zoomSpin->value());
	ui->centerReSpin->setSingleStep(0.1 / ui->zoomSpin->value());
	ui->zoomSpin->setSingleStep(0.1 * ui->zoomSpin->value());
	worker->start();
}

void MainWindow::reset() {
	ui->centerReSpin->setValue(defaults[ui->fractalCombo->currentText().toUtf8().constData()][0]);
	ui->centerImSpin->setValue(defaults[ui->fractalCombo->currentText().toUtf8().constData()][1]);
	ui->zoomSpin->setValue(defaults[ui->fractalCombo->currentText().toUtf8().constData()][2]);
	generate();
}

void MainWindow::saveAs() {
	if (worker->isRunning()) {return;}
	bool resolutionConfirmed;
	int resolution = QInputDialog::getInt(this, "Resolution Dialog", "Vertical Height in Pixels:", 1440, 3, 21000, 3, &resolutionConfirmed);
	if (!resolutionConfirmed) {return;}
	QString filename = QFileDialog::getSaveFileName(this, "Save Image", "fractal.png", "Image Files (*.jpg *.png)");
	if (filename.isEmpty()) {return;}
	worker = QThread::create([this, resolution, filename] {
		cv::theRNG().state = settings->ui->seedSpin->value();
		Fractal fractal = constructors[ui->fractalCombo->currentText().toUtf8().constData()]();
		connect(&fractal, &Fractal::progress, ui->progressBar, &QProgressBar::setValue);
		cv::Mat3b canvas(resolution, resolution / 3 * 4);
		fractal.paint(canvas, ui->centerReSpin->value(), ui->centerImSpin->value(), ui->zoomSpin->value());
		cv::cvtColor(canvas, canvas, cv::COLOR_BGR2RGB);
		cv::imwrite(filename.toUtf8().constData(), canvas);
	});
	worker->start();
}
