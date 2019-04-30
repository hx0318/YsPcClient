#ifndef NOTICEDIALOG_H
#define NOTICEDIALOG_H

#include <QDialog>
#include <QTimer>

namespace Ui {
class NoticeDialog;
}

class NoticeDialog : public QDialog
{
	Q_OBJECT

public:
	explicit NoticeDialog(QWidget *parent = 0);
	~NoticeDialog();
	void init(QString strContent, QString strBtn1, QString strBtn2, int iStayTime);

private:
	Ui::NoticeDialog *ui;
	QTimer *m_Timer;
};

#endif // NOTICEDIALOG_H
