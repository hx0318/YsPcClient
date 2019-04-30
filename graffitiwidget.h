#ifndef GRAFFITIWIDGET_H
#define GRAFFITIWIDGET_H

#include <QWidget>
#include <QPen>
#include <QJsonObject>
#include <QJsonArray>

namespace Ui {
class GraffitiWidget;
}

class ShapePoints
{
public:
	ShapePoints();

	QVector<QPointF> VectorPointF;
	int ShapeType;
	int PenWidth;
	QColor PenColor;
};

class GraffitiWidget : public QWidget
{
	Q_OBJECT

public:
	explicit GraffitiWidget(QWidget *parent = 0);
	~GraffitiWidget();

	QList<ShapePoints> ListShapePoints;
	QPen m_Pen;
	QPixmap m_Pixmap;

	QJsonArray getCurShapes();
	void ChangePixmap(QPixmap pix);

protected:
	void mouseReleaseEvent(QMouseEvent *event);
	void paintEvent(QPaintEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);

private:
	Ui::GraffitiWidget *ui;
	ShapePoints m_SPCur;
	bool IfMouseLeftPress;
	QPixmap PixmapC;
	QCursor m_CursorGlowSticks;
	QColor m_PenColor;
	int m_PenWidth;

	void send_graffiti(int type, QJsonValue value);
	QJsonObject serializeShape(ShapePoints oneShape);

signals:
	void signal_send_graffiti(QJsonObject,QString);
	void signal_SetSizeFinished();

private slots:
	void slot_SetSize(QSize sz);
	void slot_bg_buttonClicked(int);
	void slot_btnRevoke_clicked();
    void slot_btnBrush_clicked();
	void slot_CursorChanged(int inx);
	void slot_PButtonHide_clicked();
	void slot_bg1_buttonClicked(int);
	void slot_PButtonSure_clicked();
};

#endif // GRAFFITIWIDGET_H
