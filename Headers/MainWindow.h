#include <QDebug>
#include <QFileDialog>
#include <QInputDialog>
#include <QShortcut>
#include <QThread>
#include "ui_MainWindow.h"
#include "SettingsWindow.h"
#include "InteractiveLabel.h"
#include "Fractal.h"

#ifndef FRACTOID_MAINWINDOW_H
#define FRACTOID_MAINWINDOW_H

#define CONSTRUCT(FRACTAL) std::map<std::string, std::function<Fractal()>> {{ \
	"Burning Ship", [this] {return Fractal::BurningShip(ui->iterations->value(), settings->ui->escape->value(), settings->ui->smooth->checkState());}}, { \
	"Julia", [this] {return Fractal::Julia(ui->iterations->value(), settings->ui->escape->value(), settings->ui->smooth->checkState(), settings->ui->juliaComplexRe->text().toDouble(), settings->ui->juliaComplexIm->text().toDouble());}}, { \
	"Mandelbar", [this] {return Fractal::Mandelbar(ui->iterations->value(), settings->ui->escape->value(), settings->ui->smooth->checkState());}}, { \
	"Mandelbrot", [this] {return Fractal::Mandelbrot(ui->iterations->value(), settings->ui->escape->value(), settings->ui->smooth->checkState());}}, { \
	"Manowar", [this] {return Fractal::Manowar(ui->iterations->value(), settings->ui->escape->value(), settings->ui->smooth->checkState());}}, { \
	"Phoenix", [this] {return Fractal::Phoenix(ui->iterations->value(), settings->ui->escape->value(), settings->ui->smooth->checkState(), settings->ui->phoenixComplexRe->text().toDouble(), settings->ui->phoenixComplexIm->text().toDouble());}}} \
	[FRACTAL]

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow {
Q_OBJECT
public:
	explicit MainWindow(QWidget *parent = nullptr);

private:
	QThread *worker = QThread::create([] {});
	Ui::MainWindow *ui = new Ui::MainWindow;
	SettingsWindow *settings = new SettingsWindow(this);
	cv::Mat3b generate(int height, int width);

private slots:
	void display();
	void reset();
	void render();
	void zoom(QPoint point);
};

#endif
