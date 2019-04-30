#ifndef UPDOWNWIDGET_H
#define UPDOWNWIDGET_H

#include <QWidget>

namespace Ui {
class UpDownWidget;
}

class UpDownWidget : public QWidget
{
	Q_OBJECT

public:
	explicit UpDownWidget(int iSendUserId,QString sendUsername,int content,QWidget *parent = 0);
	~UpDownWidget();
	void setAction(int content);

	int m_senderId;
	int m_UpOrDown;

private:
	Ui::UpDownWidget *ui;

signals:
	void signal_agree(UpDownWidget *udw);
	void signal_ignore(UpDownWidget *udw);

private slots:
	void slot_PButtonAgree_clicked();
	void slot_PButtonIgnore_clicked();
};

#endif // UPDOWNWIDGET_H
