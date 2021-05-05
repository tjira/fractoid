#ifndef FRACTOID_INTERACTIVELABEL_H
#define FRACTOID_INTERACTIVELABEL_H

#include <QLabel>
#include <QMouseEvent>
#include <QWidget>

class InteractiveLabel : public QLabel {
Q_OBJECT
public:
	explicit InteractiveLabel(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());

protected:
	void mouseDoubleClickEvent(QMouseEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);

private:
	QPoint clickedPosition;

signals:
	void doubleClicked(QPoint point, Qt::MouseButton button);
	void dragged(QPoint point);
};

#endif
