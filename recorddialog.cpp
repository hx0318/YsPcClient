#include "recorddialog.h"
#include "ui_recorddialog.h"
#include "global.h"
#pragma execution_character_set("utf-8")

RecordDialog::RecordDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::RecordDialog)
{
	ui->setupUi(this);
	setWindowFlags(windowFlags()|Qt::FramelessWindowHint);
	setWindowTitle("发布课件");
	ui->editCoursewareNote->setContextMenuPolicy(Qt::NoContextMenu);
	ui->editCoursewareName->setContextMenuPolicy(Qt::NoContextMenu);

	connect(ui->btnUpload,SIGNAL(clicked(bool)),this,SLOT(btnUpload_clicked()));
	connect(ui->btnCancel,SIGNAL(clicked(bool)),this,SLOT(btnCancel_clicked()));
	connect(ui->btnClose,SIGNAL(clicked(bool)),this,SLOT(btnCancel_clicked()));
	connect(ui->editCoursewareNote,SIGNAL(textChanged()),this,SLOT(editCoursewareNote_textChanged()));
	connect(ui->editCoursewareName,SIGNAL(textChanged(QString)),this,SLOT(editCoursewareName_textChanged(QString)));

	ListName.clear();

	NAManager *nam = new NAManager();
	connect(nam,SIGNAL(signal_reply_finished(QJsonObject,void*)),this,SLOT(slot_GetRecordList(QJsonObject,void*)));
	QByteArray ba = QCryptographicHash::hash(QString("criId=%1&page=1&rows=10000&key=RMS").arg(Global::g_CurRoomId).toUtf8(),
											 QCryptographicHash::Md5);
	QString md5(ba.toHex().toUpper());
	QJsonObject json;
	json.insert("criId", QString::number(Global::g_CurRoomId));
	json.insert("page", QString::number(1));
	json.insert("rows", QString::number(10000));
	json.insert("sign", md5);
	QString strParam = QString("service=classroom&method=queryVideoList&params=%1").arg(QString(QJsonDocument(json).
																							toJson(QJsonDocument::Compact)));
	nam->SendRequest(QString("http://%1/rms/service/sys/execute").arg(Global::g_ServerIp),strParam);
}

RecordDialog::~RecordDialog()
{
	delete ui;
}

void RecordDialog::closeEvent(QCloseEvent *event)
{
	event->ignore();
}

void RecordDialog::btnCancel_clicked()
{
	reject();
}

void RecordDialog::editCoursewareNote_textChanged()
{
	if(ui->editCoursewareNote->toPlainText().length() > 200)
		ui->editCoursewareNote->setStyleSheet("border:1px solid rgb(179,179,179);color: rgb(255, 0, 0);");
	else
		ui->editCoursewareNote->setStyleSheet("border:1px solid rgb(179,179,179);color: rgb(0, 0, 0);");
}

void RecordDialog::editCoursewareName_textChanged(QString)
{
	if(ui->editCoursewareName->text().length() > 20)
		ui->editCoursewareName->setStyleSheet("border:1px solid rgb(179,179,179);color: rgb(255, 0, 0);");
	else
		ui->editCoursewareName->setStyleSheet("border:1px solid rgb(179,179,179);color: rgb(0, 0, 0);");
}

void RecordDialog::btnUpload_clicked()
{
	if(ui->editCoursewareName->text() == "")
	{
		Global::g_NoticeDialog->init("请输入录像名！","","",2000);
		Global::g_NoticeDialog->exec();
		return ;
	}
	if(ui->editCoursewareName->text().length() > 20)
	{
		Global::g_NoticeDialog->init("录像名过长！","","",2000);
		Global::g_NoticeDialog->exec();
		return ;
	}
	if(ui->editCoursewareNote->toPlainText().length() > 200)
	{
		Global::g_NoticeDialog->init("录像描述过长！","","",2000);
		Global::g_NoticeDialog->exec();
		return ;
	}
	if(ListName.contains(ui->editCoursewareName->text()))
	{
		Global::g_NoticeDialog->init("录像名已存在！","","",2000);
		Global::g_NoticeDialog->exec();
		return ;
	}
	emit signal_NameAndDescribe(ui->editCoursewareName->text(),ui->editCoursewareNote->toPlainText());
	accept();
}

void RecordDialog::slot_GetRecordList(QJsonObject obj, void *)
{
	if(obj.isEmpty())
	{
		Global::g_NoticeDialog->init("网络异常，获取历史录像信息失败！","","",2000);
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
	QJsonArray rows = body["rows"].toArray();
	for(int i = 0;i < rows.count();i++)
	{
		QJsonObject jor = rows.at(i).toObject();
		ListName.append(jor["crvName"].toString());
	}
}
