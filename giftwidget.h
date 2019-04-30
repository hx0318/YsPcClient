#ifndef GIFTWIDGET_H
#define GIFTWIDGET_H

#include <QWidget>
#include "namanager.h"
#include <QPainter>

namespace Ui {
class GiftWidget;
}

class GiftWidget : public QWidget
{
	Q_OBJECT

public:
	explicit GiftWidget(QString SenderIcon,QString SenderName,double GiftCount,QWidget *parent = 0);
	~GiftWidget();

private:
	Ui::GiftWidget *ui;
	QString m_ImagePath;

private slots:
	void slot_GetSenderIcon(QPixmap,void*);
};

#endif // GIFTWIDGET_H
