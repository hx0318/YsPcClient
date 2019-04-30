#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QJsonObject>

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
	Q_OBJECT

public:
	explicit LoginDialog(QWidget *parent = 0);
	~LoginDialog();

protected:
	void closeEvent(QCloseEvent *event);
	void keyPressEvent(QKeyEvent *event);

private:
	Ui::LoginDialog *ui;
	QString username;
	QString chat_server;
	QPushButton *m_PButtonClose;
	QString m_ticketId;

private slots:
	void slot_PButtonLogin_clicked();
	void slot_login_reply(QJsonObject, void *);
	void slot_PButtonLoadapp_clicked();
	void slot_ChangePassword_reply(QJsonObject,void*);
};

#endif // LOGINDIALOG_H
