#include "SettingsWindow.h"

SettingsWindow::SettingsWindow(QWidget *parent)
: QDialog(parent) {
	ui = new Ui::SettingsWindow;
	ui->setupUi(this);
}
