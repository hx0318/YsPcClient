#include "screenshotdialog.h"
#include "ui_screenshotdialog.h"
#pragma execution_character_set("utf-8")

ScreenshotDialog::ScreenshotDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::ScreenshotDialog)
{
	ui->setupUi(this);
	setWindowFlags(windowFlags()|Qt::FramelessWindowHint);
	setWindowTitle("截图");
	setWindowOpacity(0.5);
	setCursor(Qt::CrossCursor);

	Pixmap = QPixmap();
	IfMouseLeftPress = false;
	PointBegin = QPoint(0,0);
	PointEnd = QPoint(0,0);
	PointBeginTemp = QPoint(0,0);
}

ScreenshotDialog::~ScreenshotDialog()
{
	delete ui;
}

void ScreenshotDialog::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
	{
		PointBeginTemp = PointBegin;
		PointBegin = event->pos();
		IfMouseLeftPress = true;
	}
	else if(event->button() == Qt::RightButton)
		IfMouseLeftPress = false;
}

void ScreenshotDialog::mouseMoveEvent(QMouseEvent *event)
{
	if(IfMouseLeftPress)
	{
		int ix = event->pos().x();
		ix = ix > width() ? width() : ix;
		ix = ix < 0 ? 0 : ix;
		int iy = event->pos().y();
		iy = iy > height() ? height() : iy;
		iy = iy < 0 ? 0 : iy;
		PointEnd = QPoint(ix,iy);
		update();
	}
}

void ScreenshotDialog::mouseReleaseEvent(QMouseEvent *)
{
	IfMouseLeftPress = false;
}

void ScreenshotDialog::paintEvent(QPaintEvent *event)
{
	Pixmap = QPixmap(size());
	QPainter pr(&Pixmap);
	QPen pn = QPen();
	pn.setWidth(2);
	pn.setColor(Qt::red);
	pr.setPen(pn);
	pr.drawRect(PointBegin.x(),PointBegin.y(),PointEnd.x()-PointBegin.x(),PointEnd.y()-PointBegin.y());
	QPainter painter(this);
	painter.drawPixmap(0, 0, Pixmap);

	return QWidget::paintEvent(event);
}

void ScreenshotDialog::mouseDoubleClickEvent(QMouseEvent *event)
{
	if(event->button() == Qt::LeftButton)
	{
		if(PointBeginTemp != PointEnd)
			emit signal_screenshot(QRect(PointBeginTemp,PointEnd));
		accept();
	}
}
