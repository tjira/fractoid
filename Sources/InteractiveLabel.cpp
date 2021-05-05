#include "../Headers/InteractiveLabel.h"

InteractiveLabel::InteractiveLabel(QWidget *parent, Qt::WindowFlags f) : QLabel(parent) {

}

void InteractiveLabel::mouseDoubleClickEvent(QMouseEvent* event) {
	emit doubleClicked(event->pos());
}