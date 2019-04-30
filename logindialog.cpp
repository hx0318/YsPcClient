#include "logindialog.h"
#include "ui_logindialog.h"
#include "global.h"
#include "namanager.h"
#include "teacherdialog.h"
#include <QCloseEvent>
#include <QDesktopWidget>
#include "downloadappdialog.h"
#include <QUuid>
#pragma execution_character_set("utf-8")

LoginDialog::LoginDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::LoginDialog)
{
	ui->setupUi(this);
	setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint);
	setAttribute(Qt::WA_DeleteOnClose);
	setWindowTitle("登录");
	resize(QApplication::desktop()->availableGeometry().size());
	ui->EditUser->setContextMenuPolicy(Qt::NoContextMenu);
	ui->EditPassword->setContextMenuPolicy(Qt::NoContextMenu);

	connect(ui->PButtonLogin, SIGNAL(clicked(bool)), this, SLOT(slot_PButtonLogin_clicked()));
	connect(ui->PButtonLoadapp,SIGNAL(clicked(bool)),this,SLOT(slot_PButtonLoadapp_clicked()));

	Global::g_AppDataPath = QDir::homePath() + "/AppData/Local/" + AppTarget + "/";
	Global::g_Settings = new QSettings(Global::g_AppDataPath + "config.ini",QSettings::IniFormat);

	Global::g_ServerIp = Global::g_Settings->value("conf/serverIP").toString();
	if(Global::g_ServerIp.isEmpty())
    {
		Global::g_ServerIp = "www.91xxdd.cn";
//		Global::g_ServerIp = "192.168.0.251";
		Global::g_Settings->setValue("conf/serverIP", Global::g_ServerIp);
	}
	username = "";
	chat_server = "";
	m_ticketId = "";

	m_PButtonClose = new QPushButton(this);
	m_PButtonClose->setStyleSheet("QPushButton{font: bold 20px \"宋体\";color: rgb(255, 255, 255);border:none;}");
	m_PButtonClose->setText("×");
	m_PButtonClose->setGeometry(width()-30,0,30,30);
	connect(m_PButtonClose,SIGNAL(clicked(bool)),this,SLOT(close()));
}

LoginDialog::~LoginDialog()
{
	if(Global::g_Settings)
	{
		delete Global::g_Settings;
		Global::g_Settings = nullptr;
	}
	if(Global::g_NoticeDialog)
	{
		delete Global::g_NoticeDialog;
		Global::g_NoticeDialog = nullptr;
	}

	delete ui;
}

void LoginDialog::slot_PButtonLogin_clicked()
{
	if(ui->PButtonLogin->text() == "登录")
	{
		if(ui->EditUser->text() == "")
		{
			Global::g_NoticeDialog->init("请输入用户名！","","",2000);
			Global::g_NoticeDialog->exec();
			return ;
		}
		if(ui->EditPassword->text() == "")
		{
			Global::g_NoticeDialog->init("请输入密码！","","",2000);
			Global::g_NoticeDialog->exec();
			return ;
		}
		ui->PButtonLogin->setEnabled(false);
		NAManager *nam = new NAManager();
		connect(nam,SIGNAL(signal_reply_finished(QJsonObject,void*)),this,SLOT(slot_login_reply(QJsonObject,void*)));
		QByteArray ba = QCryptographicHash::hash(QString("password=%1&type=1&username=%2&key=AAS").arg(ui->EditPassword->text()).
												 arg(ui->EditUser->text()).toUtf8(), QCryptographicHash::Md5);
		QString md5(ba.toHex().toUpper());
		QJsonObject json;
		json.insert("username", ui->EditUser->text());
		json.insert("password", ui->EditPassword->text());
		json.insert("type", "1");
		json.insert("sign", md5);
		QString strParam = QString("service=teacher&method=login&params=%1").arg(QString(QJsonDocument(json).
																						 toJson(QJsonDocument::Compact)));
		nam->SendRequest(QString("http://%1/aas/service/sys/execute").arg(Global::g_ServerIp),strParam);
	}
	else if(ui->PButtonLogin->text() == "重置密码")
	{
		if(ui->EditPassword->text() != ui->EditUser->text())
		{
			Global::g_NoticeDialog->init("两次输入密码不同，请重新输入！","","",2000);
			Global::g_NoticeDialog->exec();
			return ;
		}
		if(ui->EditPassword->text() == "" || ui->EditUser->text() == "")
		{
			Global::g_NoticeDialog->init("密码不能为空！","","",2000);
			Global::g_NoticeDialog->exec();
			return ;
		}
		ui->PButtonLogin->setEnabled(false);
		NAManager *nam = new NAManager();
		connect(nam,SIGNAL(signal_reply_finished(QJsonObject,void*)),this,SLOT(slot_ChangePassword_reply(QJsonObject,void*)));
		QByteArray ba = QCryptographicHash::hash(QString("flag=2&id=%1&password=%2&key=AAS").arg(Global::g_LoginerId).
												 arg(ui->EditPassword->text()).toUtf8(), QCryptographicHash::Md5);
		QString md5(ba.toHex().toUpper());
		QJsonObject json;
		json.insert("password", ui->EditPassword->text());
		json.insert("id", QString::number(Global::g_LoginerId));
		json.insert("flag", "2");
		json.insert("sign", md5);
		QString strParam = QString("service=teacher&method=saveOrUpdate&params=%1").arg(QString(QJsonDocument(json).
																						 toJson(QJsonDocument::Compact)));
		nam->SendRequest(QString("http://%1/aas/service/sys/execute").arg(Global::g_ServerIp),strParam);
	}
}

void LoginDialog::slot_login_reply(QJsonObject obj, void *)
{
	ui->PButtonLogin->setEnabled(true);

	if(obj.isEmpty())
	{
		Global::g_NoticeDialog->init("网络异常，登录失败！","","",2000);
		Global::g_NoticeDialog->exec();
		return ;
	}
	if (obj["msg"].toString() != "")
	{
		Global::g_NoticeDialog->init(obj["msg"].toString(),"","",2000);
		Global::g_NoticeDialog->exec();
		return ;
	}

	QJsonObject body = obj["body"].toObject();
	username = body["username"].toString();
	Global::g_LoginerId = body["id"].toInt();
	Global::g_LoginerName = body["name"].toString();
	chat_server = body["chat_server"].toString();
	m_ticketId = body["ticketId"].toString();
	if (body["tiIsinit"].toInt() == 0)
	{
		ui->LabelLoginTitle->setText("登录成功，请重置密码");
		ui->LabelUser->setText("密码：");
		ui->EditUser->setEchoMode(QLineEdit::Password);
		ui->EditUser->setText("");
		ui->EditPassword->setText("");
		ui->LabelPassword->setText("重置密码：");
		ui->PButtonLogin->setText("重置密码");
	}
	else
	{
		QUuid ud = QUuid::createUuid();
		Global::g_Uuid = ud.toString();

		TeacherDialog *td = new TeacherDialog(m_ticketId,chat_server);
		hide();
		td->show();
	}
}

void LoginDialog::closeEvent(QCloseEvent *event)
{
	event->accept();
}

void LoginDialog::keyPressEvent(QKeyEvent *event)
{
	switch (event->key())
	{
	case Qt::Key_Escape:
		break;
	default:
		QDialog::keyPressEvent(event);
	}
}

void LoginDialog::slot_PButtonLoadapp_clicked()
{
	DownloadAppDialog *dad = new DownloadAppDialog();
	dad->setGeometry(rect());
	dad->exec();
	delete dad;
	dad = nullptr;
}

void LoginDialog::slot_ChangePassword_reply(QJsonObject obj,void*)
{
	ui->PButtonLogin->setEnabled(true);

	if(obj.isEmpty())
	{
		Global::g_NoticeDialog->init("网络异常，修改密码失败！","","",2000);
		Global::g_NoticeDialog->exec();
		return ;
	}
	if (obj["msg"].toString() != "")
	{
		Global::g_NoticeDialog->init(obj["msg"].toString(),"","",2000);
		Global::g_NoticeDialog->exec();
		return ;
	}

	ui->LabelLoginTitle->setText("登录有师");
	ui->LabelUser->setText("账号：");
	ui->EditUser->setEchoMode(QLineEdit::Normal);
	ui->EditUser->setText(username);
	ui->LabelPassword->setText("密码：");
	ui->PButtonLogin->setText("登录");

	QUuid ud = QUuid::createUuid();
	Global::g_Uuid = ud.toString();

	TeacherDialog *td = new TeacherDialog(m_ticketId,chat_server);
	hide();
	td->show();
}
