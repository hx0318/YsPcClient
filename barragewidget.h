#ifndef BARRAGEWIDGET_H
#define BARRAGEWIDGET_H

#include <QWidget>
#include <QJsonObject>

namespace Ui {
class BarrageWidget;
}

class BarrageWidget : public QWidget
{
	Q_OBJECT

public:
	explicit BarrageWidget(int UserId,QString Username,QString SendTime,QString content,QWidget *parent = 0);
	~BarrageWidget();
	QString GetContent();

private:
	Ui::BarrageWidget *ui;
	QString m_ImagePath;

private slots:
	void slot_GetHeadPath(QJsonObject,void*);
	void slot_GetHead(QPixmap,void*);
};

#endif // BARRAGEWIDGET_H
