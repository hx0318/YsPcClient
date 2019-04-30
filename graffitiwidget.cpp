#include "graffitiwidget.h"
#include "ui_graffitiwidget.h"
#include <QMouseEvent>
#include "global.h"
#include <QPainter>
#include <QDebug>

ShapePoints::ShapePoints()
{
	VectorPointF.clear();
	ShapeType = 1;
	PenWidth = 1;
	PenColor = QColor(0,0,0,255);
}

GraffitiWidget::GraffitiWidget(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::GraffitiWidget)
{
	ui->setupUi(this);

	ui->WidgetPencil->hide();
	connect(ui->PButtonHide,SIGNAL(clicked(bool)),this,SLOT(slot_PButtonHide_clicked()));
	QButtonGroup *bg = new QButtonGroup(this);
	bg->addButton(ui->PButtonColor_1,0);
	bg->addButton(ui->PButtonColor_2,1);
	bg->addButton(ui->PButtonColor_3,2);
	bg->addButton(ui->PButtonColor_4,3);
	bg->addButton(ui->PButtonColor_5,4);
	bg->addButton(ui->PButtonColor_6,5);
	bg->addButton(ui->PButtonColor_7,6);
	bg->addButton(ui->PButtonColor_8,7);
	bg->addButton(ui->PButtonColor_9,8);
	bg->addButton(ui->PButtonColor_10,9);
	bg->addButton(ui->PButtonColor_11,10);
	bg->addButton(ui->PButtonColor_12,11);
	bg->addButton(ui->PButtonColor_13,12);
	bg->addButton(ui->PButtonColor_14,13);
	bg->addButton(ui->PButtonColor_15,14);
	connect(bg, SIGNAL(buttonClicked(int)), this, SLOT(slot_bg_buttonClicked(int)));
	QButtonGroup* bg1 = new QButtonGroup(this);
	bg1->addButton(ui->PButtonBlod_1,0);
	bg1->addButton(ui->PButtonBlod_2,1);
	bg1->addButton(ui->PButtonBlod_3,2);
	connect(bg1, SIGNAL(buttonClicked(int)), this, SLOT(slot_bg1_buttonClicked(int)));
	connect(ui->PButtonSure,SIGNAL(clicked(bool)),this,SLOT(slot_PButtonSure_clicked()));

	m_Pen = QPen();
	m_PenWidth = 1;
	m_Pen.setWidth(m_PenWidth);
	m_PenColor = QColor(0,0,0,255);
	m_Pen.setColor(m_PenColor);
	ListShapePoints.clear();
	m_SPCur = ShapePoints();
	IfMouseLeftPress = false;
	m_Pixmap = QPixmap();
	PixmapC = QPixmap();
	m_CursorGlowSticks = QCursor(QPixmap(":/res/tool/GlowSticks.png"),-1,-1);
}

GraffitiWidget::~GraffitiWidget()
{
	delete ui;
}

void GraffitiWidget::mouseReleaseEvent(QMouseEvent *event)
{
	if(event->button() == Qt::LeftButton)
	{
		if(m_SPCur.VectorPointF.count() != 1)
		{
			ListShapePoints.append(m_SPCur);
			update();

			for(int i = 0;i < m_SPCur.VectorPointF.count();i++)
			{
				QJsonObject obj;
				if(i == 0)
				{
					obj.insert("typeId", 100);
					obj.insert("content", 1);
					obj.insert("sendUserId", Global::g_LoginerId);
					if(PixmapC.isNull() == false)
					{
						obj.insert("x", (m_SPCur.VectorPointF.at(i).x() - (m_Pixmap.width()-PixmapC.width())/2) /
								   PixmapC.width());
						obj.insert("y", (m_SPCur.VectorPointF.at(i).y() - (m_Pixmap.height()-PixmapC.height())/2) /
								   PixmapC.height());
					}
					else
					{
						obj.insert("x", m_SPCur.VectorPointF.at(i).x() / m_Pixmap.width());
						obj.insert("y", m_SPCur.VectorPointF.at(i).y() / m_Pixmap.height());
					}
				}
				else
				{
					obj.insert("typeId", 100);
					obj.insert("content", 2);
					obj.insert("sendUserId", Global::g_LoginerId);
					if(PixmapC.isNull() == false)
					{
						obj.insert("x", (m_SPCur.VectorPointF.at(i).x() - (m_Pixmap.width()-PixmapC.width())/2) /
								   PixmapC.width());
						obj.insert("y", (m_SPCur.VectorPointF.at(i).y() - (m_Pixmap.height()-PixmapC.height())/2) /
								   PixmapC.height());
					}
					else
					{
						obj.insert("x", m_SPCur.VectorPointF.at(i).x() / m_Pixmap.width());
						obj.insert("y", m_SPCur.VectorPointF.at(i).y() / m_Pixmap.height());
					}
				}
				emit signal_send_graffiti(obj,"*");
			}
			QJsonObject obj;
			obj.insert("typeId", 100);
			obj.insert("content", 3);
			obj.insert("sendUserId", Global::g_LoginerId);
			emit signal_send_graffiti(obj,"*");
		}
        IfMouseLeftPress = false;
	}
}

void GraffitiWidget::slot_SetSize(QSize sz)
{
	m_Pixmap = QPixmap(sz);
    slot_btnBrush_clicked();
//    update();
	emit signal_SetSizeFinished();
}

void GraffitiWidget::slot_bg_buttonClicked(int inx)
{
	switch(inx)
	{
	case 0:
		m_PenColor = QColor(255, 0, 0, 255);
		break;
	case 1:
		m_PenColor = QColor(170, 0, 255, 255);
		break;
	case 2:
		m_PenColor = QColor(255, 255, 0, 255);
		break;
	case 3:
		m_PenColor = QColor(255, 0, 255, 255);
		break;
	case 4:
		m_PenColor = QColor(0, 255, 0, 255);
		break;
	case 5:
		m_PenColor = QColor(255, 170, 255, 255);
		break;
	case 6:
		m_PenColor = QColor(255, 85, 0, 255);
		break;
	case 7:
		m_PenColor = QColor(85, 170, 255, 255);
		break;
	case 8:
		m_PenColor = QColor(0, 0, 255, 255);
		break;
	case 9:
		m_PenColor = QColor(200,200, 200, 255);
		break;
	case 10:
		m_PenColor = QColor(0, 85, 0, 255);
		break;
	case 11:
		m_PenColor = QColor(170, 0, 85, 255);
		break;
	case 12:
		m_PenColor = QColor(120,120, 120, 255);
		break;
	case 13:
	default:
		m_PenColor = QColor(0,0,0, 255);
		break;
	case 14:
		m_PenColor = QColor(255, 255, 255, 255);
		break;
	}
}

void GraffitiWidget::slot_btnRevoke_clicked()
{
    if(ListShapePoints.isEmpty() == false && IfMouseLeftPress == false)
	{
		m_SPCur.VectorPointF.clear();
		ListShapePoints.removeLast();
		update();
        send_graffiti(103,8);
	}
}

void GraffitiWidget::slot_btnBrush_clicked()
{
	m_SPCur.VectorPointF.clear();
	ListShapePoints.clear();
	update();
    send_graffiti(103,10);
}

void GraffitiWidget::paintEvent(QPaintEvent *event)
{
	m_Pixmap.fill(Qt::black);
	QPainter pr(&m_Pixmap);
	pr.setRenderHint(QPainter::Antialiasing);

	if(PixmapC.isNull() == false)
		pr.drawPixmap((m_Pixmap.width()-PixmapC.width())/2,(m_Pixmap.height()-PixmapC.height())/2,PixmapC.width(),
					  PixmapC.height(),PixmapC);

	QPen pn = QPen();
	for(int i = 0;i < ListShapePoints.count();i++)
	{
		ShapePoints sp = ListShapePoints[i];
		pn.setWidth(sp.PenWidth);
		pn.setColor(sp.PenColor);
		pr.setPen(pn);
		switch(sp.ShapeType)
		{
		case 1:
			pr.drawPolyline(sp.VectorPointF);
			break;
		case 3:
			pr.drawRect(sp.VectorPointF.at(0).x(),sp.VectorPointF.at(0).y(),
						sp.VectorPointF.at(sp.VectorPointF.count()-1).x()-sp.VectorPointF.at(0).x(),
						sp.VectorPointF.at(sp.VectorPointF.count()-1).y()-sp.VectorPointF.at(0).y());
			break;
		case 4:
			pr.drawEllipse(sp.VectorPointF.at(0).x(),sp.VectorPointF.at(0).y(),
						   sp.VectorPointF.at(sp.VectorPointF.count()-1).x()-sp.VectorPointF.at(0).x(),
						   sp.VectorPointF.at(sp.VectorPointF.count()-1).y()-sp.VectorPointF.at(0).y());
			break;
		case 5:
		{
			QPointF StartPF = QPointF(sp.VectorPointF.at(0).x(),sp.VectorPointF.at(0).y());
			QPointF EndPF = QPointF(sp.VectorPointF.at(sp.VectorPointF.count()-1).x(),
									sp.VectorPointF.at(sp.VectorPointF.count()-1).y());
			pr.drawLine(StartPF,EndPF);
			float x1 = StartPF.x();
			float y1 = StartPF.y();
			float x2 = EndPF.x();
			float y2 = EndPF.y();
			float l = 10.0;
			float a = 0.5;
			float x3 = x2 - l * cos(atan2((y2 - y1), (x2 - x1)) - a);
			float y3 = y2 - l * sin(atan2((y2 - y1), (x2 - x1)) - a);
			float x4 = x2 - l * sin(atan2((x2 - x1), (y2 - y1)) - a);
			float y4 = y2 - l * cos(atan2((x2 - x1), (y2 - y1)) - a);
			pr.drawLine(x2, y2, x3, y3);
			pr.drawLine(x2, y2, x4, y4);
		}
			break;
		case 2:
			pr.drawLine(sp.VectorPointF.at(0).x(),sp.VectorPointF.at(0).y(),sp.VectorPointF.at(sp.VectorPointF.count()-1).x(),
						sp.VectorPointF.at(sp.VectorPointF.count()-1).y());
			break;
		default:
			break;
		}
	}
	if(m_SPCur.VectorPointF.count() > 1 && IfMouseLeftPress)
	{
		pn.setWidth(m_SPCur.PenWidth);
		pn.setColor(m_SPCur.PenColor);
		pr.setPen(pn);
		switch(m_SPCur.ShapeType)
		{
		case 1:
			pr.drawPolyline(m_SPCur.VectorPointF);
			break;
		case 3:
			pr.drawRect(m_SPCur.VectorPointF.at(0).x(),m_SPCur.VectorPointF.at(0).y(),
						m_SPCur.VectorPointF.at(m_SPCur.VectorPointF.count()-1).x()-m_SPCur.VectorPointF.at(0).x(),
						m_SPCur.VectorPointF.at(m_SPCur.VectorPointF.count()-1).y()-m_SPCur.VectorPointF.at(0).y());
			break;
		case 4:
			pr.drawEllipse(m_SPCur.VectorPointF.at(0).x(),m_SPCur.VectorPointF.at(0).y(),
						   m_SPCur.VectorPointF.at(m_SPCur.VectorPointF.count()-1).x()-m_SPCur.VectorPointF.at(0).x(),
						   m_SPCur.VectorPointF.at(m_SPCur.VectorPointF.count()-1).y()-m_SPCur.VectorPointF.at(0).y());
			break;
		case 5:
		{
			QPointF StartPF = QPointF(m_SPCur.VectorPointF.at(0).x(),m_SPCur.VectorPointF.at(0).y());
			QPointF EndPF = QPointF(m_SPCur.VectorPointF.at(m_SPCur.VectorPointF.count()-1).x(),
									m_SPCur.VectorPointF.at(m_SPCur.VectorPointF.count()-1).y());
			pr.drawLine(StartPF,EndPF);
			float x1 = StartPF.x();
			float y1 = StartPF.y();
			float x2 = EndPF.x();
			float y2 = EndPF.y();
			float l = 10.0;
			float a = 0.5;
			float x3 = x2 - l * cos(atan2((y2 - y1), (x2 - x1)) - a);
			float y3 = y2 - l * sin(atan2((y2 - y1), (x2 - x1)) - a);
			float x4 = x2 - l * sin(atan2((x2 - x1), (y2 - y1)) - a);
			float y4 = y2 - l * cos(atan2((x2 - x1), (y2 - y1)) - a);
			pr.drawLine(x2, y2, x3, y3);
			pr.drawLine(x2, y2, x4, y4);
		}
			break;
		case 2:
			pr.drawLine(m_SPCur.VectorPointF.at(0).x(),m_SPCur.VectorPointF.at(0).y(),
						m_SPCur.VectorPointF.at(m_SPCur.VectorPointF.count()-1).x(),
						m_SPCur.VectorPointF.at(m_SPCur.VectorPointF.count()-1).y());
			break;
		default:
			break;
		}
	}

    QPainter par(this);
    par.drawPixmap(0,0,m_Pixmap);

    return QWidget::paintEvent(event);
}

void GraffitiWidget::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
	{
		m_SPCur.VectorPointF.clear();
		if(PixmapC.isNull() == false)
		{
			if(event->pos().x() > (m_Pixmap.width()-PixmapC.width())/2+PixmapC.width() ||
					event->pos().y() > (m_Pixmap.height()-PixmapC.height())/2+PixmapC.height() ||
					event->pos().x() < (m_Pixmap.width()-PixmapC.width())/2 ||
					event->pos().y() < (m_Pixmap.height()-PixmapC.height())/2)
				return ;
		}
		else
		{
			if(event->pos().x() > m_Pixmap.width() || event->pos().y() > m_Pixmap.height() || event->pos().x() < 0 ||
					event->pos().y() < 0)
				return ;
		}
		m_SPCur.VectorPointF.append(event->pos());
		m_SPCur.ShapeType = 1;
		m_SPCur.PenWidth = m_Pen.width();
		m_SPCur.PenColor = m_Pen.color();
		IfMouseLeftPress = true;
	}
	else if(event->button() == Qt::RightButton)
		IfMouseLeftPress = false;
}

void GraffitiWidget::mouseMoveEvent(QMouseEvent *event)
{
	if(IfMouseLeftPress)
	{
		int ix = event->pos().x();
		if(PixmapC.isNull() == false)
		{
			ix = ix > (m_Pixmap.width()-PixmapC.width())/2+PixmapC.width() ?
						(m_Pixmap.width()-PixmapC.width())/2+PixmapC.width() : ix;
			ix = ix < (m_Pixmap.width()-PixmapC.width())/2 ? (m_Pixmap.width()-PixmapC.width())/2 : ix;
		}
		else
		{
			ix = ix > m_Pixmap.width() ? m_Pixmap.width() : ix;
			ix = ix < 0 ? 0 : ix;
		}
		int iy = event->pos().y();
		if(PixmapC.isNull() == false)
		{
			iy = iy > (m_Pixmap.height()-PixmapC.height())/2+PixmapC.height() ?
						(m_Pixmap.height()-PixmapC.height())/2+PixmapC.height() : iy;
			iy = iy < (m_Pixmap.height()-PixmapC.height())/2 ? (m_Pixmap.height()-PixmapC.height())/2 : iy;
		}
		else
		{
			iy = iy > m_Pixmap.height() ? m_Pixmap.height() : iy;
			iy = iy < 0 ? 0 : iy;
		}
		m_SPCur.VectorPointF.append(QPoint(ix,iy));
		update();
	}
}

void GraffitiWidget::send_graffiti(int type, QJsonValue value)
{
    QJsonObject obj;
    obj.insert("typeId", type);
    obj.insert("sendUserId", Global::g_LoginerId);
    obj.insert("content", value);
	emit signal_send_graffiti(obj,"*");
}

QJsonObject GraffitiWidget::serializeShape(ShapePoints oneShape)
{
	QJsonObject item;
	item.insert("shapeType", oneShape.ShapeType);
	QJsonObject color;
	color.insert("r", oneShape.PenColor.red());
	color.insert("g", oneShape.PenColor.green());
	color.insert("b", oneShape.PenColor.blue());
	color.insert("a", oneShape.PenColor.alpha());
	item.insert("shapeColor", color);
	item.insert("lineWidth", oneShape.PenWidth);
	QJsonArray points;
	switch (oneShape.ShapeType)
	{
	case 1:
		for(int i = 0;i < oneShape.VectorPointF.count();i++)
		{
			QJsonObject pointObj;
			if(PixmapC.isNull() == false)
			{
				pointObj.insert("x", QString::number((oneShape.VectorPointF.at(i).x() - (m_Pixmap.width()-PixmapC.width())/2)
													 / PixmapC.width()).left(6));
				pointObj.insert("y", QString::number((oneShape.VectorPointF.at(i).y() - (m_Pixmap.height()-PixmapC.height())/2)
													 / PixmapC.height()).left(6));
			}
			else
			{
				pointObj.insert("x", QString::number(oneShape.VectorPointF.at(i).x() / m_Pixmap.width()).left(6));
				pointObj.insert("y", QString::number(oneShape.VectorPointF.at(i).y() / m_Pixmap.height()).left(6));
			}
			points.append(pointObj);
		}
		break;
	default:
	{
		QJsonObject startPointObj;
		if(PixmapC.isNull() == false)
		{
			startPointObj.insert("x", QString::number((oneShape.VectorPointF.first().x() - (m_Pixmap.width()-PixmapC.width())/2)
													  / PixmapC.width()).left(6));
			startPointObj.insert("y", QString::number((oneShape.VectorPointF.first().y() - (m_Pixmap.height()-PixmapC.height())/2)
													  / PixmapC.height()).left(6));
		}
		else
		{
			startPointObj.insert("x", QString::number(oneShape.VectorPointF.first().x() / m_Pixmap.width()).left(6));
			startPointObj.insert("y", QString::number(oneShape.VectorPointF.first().y() / m_Pixmap.height()).left(6));
		}
		points.append(startPointObj);
		QJsonObject endPointObj;
		if(PixmapC.isNull() == false)
		{
			endPointObj.insert("x", QString::number((oneShape.VectorPointF.last().x() - (m_Pixmap.width()-PixmapC.width())/2)
													/ PixmapC.width()).left(6));
			endPointObj.insert("y", QString::number((oneShape.VectorPointF.last().y() - (m_Pixmap.height()-PixmapC.height())/2)
													/ PixmapC.height()).left(6));
		}
		else
		{
			endPointObj.insert("x", QString::number(oneShape.VectorPointF.last().x() / m_Pixmap.width()).left(6));
			endPointObj.insert("y", QString::number(oneShape.VectorPointF.last().y() / m_Pixmap.height()).left(6));
		}
		points.append(endPointObj);
	}
		break;
	}
	item.insert("points", points);
	return item;
}

QJsonArray GraffitiWidget::getCurShapes()
{
	QJsonArray shapeArray;
	for(int i = 0;i < ListShapePoints.count();i++)
	{
		shapeArray.append(serializeShape(ListShapePoints.at(i)));
	}
	return shapeArray;
}

void GraffitiWidget::ChangePixmap(QPixmap pix)
{
	m_SPCur.VectorPointF.clear();
	ListShapePoints.clear();
	send_graffiti(103,10);
	PixmapC = pix;
	update();
}

void GraffitiWidget::slot_CursorChanged(int inx)
{
	switch(inx)
	{
	case 0:
	default:
		setCursor(Qt::ArrowCursor);
		break;
	case 1:
		ui->WidgetPencil->move(width()-ui->WidgetPencil->width()-60,0);
		ui->WidgetPencil->show();
		break;
	case 2:
		setCursor(m_CursorGlowSticks);
		break;
	}
}

void GraffitiWidget::slot_PButtonHide_clicked()
{
	ui->WidgetPencil->hide();
}

void GraffitiWidget::slot_bg1_buttonClicked(int inx)
{
	switch(inx)
	{
	case 0:
	default:
		m_PenWidth = 1;
		break;
	case 1:
		m_PenWidth = 2;
		break;
	case 2:
		m_PenWidth = 3;
		break;
	}
}

void GraffitiWidget::slot_PButtonSure_clicked()
{
	m_Pen.setColor(m_PenColor);
	QJsonObject obj;
	obj.insert("r", m_PenColor.red());
	obj.insert("g", m_PenColor.green());
	obj.insert("b", m_PenColor.blue());
	obj.insert("a", m_PenColor.alpha());
	send_graffiti(104,obj);
	m_Pen.setWidth(m_PenWidth);
	send_graffiti(105,m_PenWidth);
	ui->WidgetPencil->hide();
}
