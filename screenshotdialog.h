#ifndef SCREENSHOTDIALOG_H
#define SCREENSHOTDIALOG_H

#include <QDialog>
#include <QMouseEvent>
#include <QPainter>

namespace Ui {
class ScreenshotDialog;
}

class ScreenshotDialog : public QDialog
{
	Q_OBJECT

public:
	explicit ScreenshotDialog(QWidget *parent = 0);
	~ScreenshotDialog();

protected:
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void paintEvent(QPaintEvent *event);
	void mouseDoubleClickEvent(QMouseEvent *event);

private:
	Ui::ScreenshotDialog *ui;
	QPixmap Pixmap;
	QPoint PointBegin;
	QPoint PointEnd;
	bool IfMouseLeftPress;
	QPoint PointBeginTemp;

signals:
	void signal_screenshot(QRect rect);
};

#endif // SCREENSHOTDIALOG_H
