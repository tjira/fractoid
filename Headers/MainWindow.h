#include <QFileDialog>
#include <QThread>
#include "ui_MainWindow.h"
#include "SettingsWindow.h"
#include "Fractal.h"

#ifndef FRACTOID_MAINWINDOW_H
#define FRACTOID_MAINWINDOW_H

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow {
public:
	explicit MainWindow(QWidget* parent = nullptr);

private:
	Fractal fractal = Fractal::Mandelbrot(100, 10, true);
	std::map<std::string, std::function<Fractal()>> constructors;
	std::map<std::string, std::array<double, 3>> defaults;
	Ui::MainWindow *ui;
	SettingsWindow *settings;

private slots:
	void generate();
	void reset();
	void saveAs();
};

#endif
