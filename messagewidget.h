#ifndef MESSAGEWIDGET_H
#define MESSAGEWIDGET_H

#include <QWidget>
#include "namanager.h"

namespace Ui {
class MessageWidget;
}

class MessageWidget : public QWidget
{
	Q_OBJECT

public:
	explicit MessageWidget(QJsonObject obj,QWidget *parent = 0);
	~MessageWidget();

	int m_clmId;
	int m_ciId;

private:
	Ui::MessageWidget *ui;
	QJsonObject m_obj;
	QString m_ImagePath;

signals:
	void signal_MessageReplySuccess();
	void signal_MessagePraiseSuccess();

private slots:
	void slot_GetHead(QPixmap pix, void *);
	void slot_PButtonReplyTop_clicked();
	void slot_PButtonReplyBottom_clicked();
	void slot_PButtonReplyBottom_reply(QJsonObject obj, void *);
	void slot_PButtonPraise_clicked(bool IsChecked);
	void slot_PButtonPraiseOk_reply(QJsonObject obj, void *);
	void slot_PButtonPraiseCa_reply(QJsonObject obj, void *);
	void slot_init();
};

#endif // MESSAGEWIDGET_H
