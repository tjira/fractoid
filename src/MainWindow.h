#include <QFileDialog>
#include <QProgressBar>
#include <QThread>

#include "ui_MainWindow.h"

#include "BurningShip.h"
#include "Julia.h"
#include "Mandelbrot.h"
#include "Newton.h"
#include "Phoenix.h"
#include "Manowar.h"

#ifndef FRACTOID_MAINWINDOW_H
#define FRACTOID_MAINWINDOW_H

class MainWindow : public QMainWindow {
public:
	explicit MainWindow(QWidget* parent = nullptr);

private:
	std::function<void(cv::Mat3b)> generator;
	Ui::MainWindow ui{};

private slots:
	void generate();
	void save();
	void resetView();
};


#endif
