#ifndef PICTUREWIDGET_H
#define PICTUREWIDGET_H

#include <QWidget>

class PictureWidget : public QWidget
{
	Q_OBJECT
public:
	explicit PictureWidget(QWidget *parent = nullptr);
	void SetBackground(QPixmap pix);
	void clearWnd();

protected:
	void paintEvent(QPaintEvent *);
	void mouseDoubleClickEvent(QMouseEvent *event);

private:
	QPixmap m_Pixmap;

signals:
	void signal_DoubleClick();
};

#endif // PICTUREWIDGET_H
