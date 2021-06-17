#include "InteractiveLabel.h"

InteractiveLabel::InteractiveLabel(QWidget *parent, Qt::WindowFlags f) : QLabel(parent) {

}

void InteractiveLabel::mouseDoubleClickEvent(QMouseEvent* event) {
	emit doubleClicked(event->pos(), event->button());
}

void InteractiveLabel::mousePressEvent(QMouseEvent *event) {
	if (event->button() == Qt::MouseButton::LeftButton) {
		clickedPosition = event->pos();
	}
}

void InteractiveLabel::mouseReleaseEvent(QMouseEvent *event) {
	if (event->button() == Qt::MouseButton::LeftButton) {
		QPoint vector = event->pos() - clickedPosition;
		if (vector.manhattanLength() > 5) {
			emit dragged(vector);
		}
	}
}
