#include <QDialog>
#include "ui_SettingsWindow.h"

#ifndef FRACTOID_SETTINGSWINDOW_H
#define FRACTOID_SETTINGSWINDOW_H

namespace Ui {
	class SettingsWindow;
}

class SettingsWindow : public QDialog {
public:
	explicit SettingsWindow(QWidget *parent = nullptr);
	Ui::SettingsWindow *ui;
};

#endif
