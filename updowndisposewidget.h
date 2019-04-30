#ifndef UPDOWNDISPOSEWIDGET_H
#define UPDOWNDISPOSEWIDGET_H

#include <QWidget>
#include "updownwidget.h"

namespace Ui {
class UpDownDisposeWidget;
}

class UpDownDisposeWidget : public QWidget
{
	Q_OBJECT

public:
	explicit UpDownDisposeWidget(QWidget *parent = 0);
	~UpDownDisposeWidget();
	void add_udw(int iSendUserId,QString sendUsername,int content);
	void remove_udw(UpDownWidget *udw);

private:
	Ui::UpDownDisposeWidget *ui;

signals:
	void signal_udw_arrive(UpDownWidget *udw);

private slots:
	void slot_PButtonClose_clicked();
	void slot_udw_agree(UpDownWidget *udw);
	void slot_udw_ignore(UpDownWidget *udw);
};

#endif // UPDOWNDISPOSEWIDGET_H
