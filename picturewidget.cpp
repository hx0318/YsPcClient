#include "picturewidget.h"
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>

PictureWidget::PictureWidget(QWidget *parent) : QWidget(parent)
{
	m_Pixmap = QPixmap();
}

void PictureWidget::paintEvent(QPaintEvent *)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setBrush(QBrush(Qt::transparent,m_Pixmap));
	painter.setPen(Qt::transparent);
	painter.drawRect(m_Pixmap.rect());
}

void PictureWidget::SetBackground(QPixmap pix)
{
	if(pix.isNull())
	{
		qDebug() << "PictureWidget::SetBackground:pix.isNull()";
		return ;
	}

	m_Pixmap = pix.scaled(size(),Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
	update();
}

void PictureWidget::clearWnd()
{
	m_Pixmap = QPixmap(size());
	m_Pixmap.fill(QColor(199,171,204));
	update();
}

void PictureWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
	if(event->button() == Qt::LeftButton)
		emit signal_DoubleClick();
}
