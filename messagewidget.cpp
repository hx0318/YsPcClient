#include "messagewidget.h"
#include "ui_messagewidget.h"
#include "global.h"
#include <QPainter>
#pragma execution_character_set("utf-8")

MessageWidget::MessageWidget(QJsonObject obj,QWidget *parent) :
	QWidget(parent),
	ui(new Ui::MessageWidget)
{
	ui->setupUi(this);
	ui->PButtonReplyBottom->hide();
	connect(ui->PButtonReplyTop,SIGNAL(clicked(bool)),this,SLOT(slot_PButtonReplyTop_clicked()));
	connect(ui->PButtonReplyBottom,SIGNAL(clicked(bool)),this,SLOT(slot_PButtonReplyBottom_clicked()));
	connect(ui->PButtonPraise,SIGNAL(clicked(bool)),this,SLOT(slot_PButtonPraise_clicked(bool)));
	ui->TEditReply->setContextMenuPolicy(Qt::NoContextMenu);
	ui->TEditMessage->setContextMenuPolicy(Qt::NoContextMenu);

	m_obj = obj;
	QTimer::singleShot(0,this,SLOT(slot_init()));
}

MessageWidget::~MessageWidget()
{
	delete ui;
}

void MessageWidget::slot_init()
{
	QString imagePath = "";
	QString nickName = "";
	int userType = m_obj["userType"].toInt();
	switch(userType)
	{
	case 1://1-学生
	default:
	{
		QJsonObject sendUserInfo = m_obj["sendUserInfo"].toObject();
		imagePath = sendUserInfo["imagePath"].toString();
		nickName = sendUserInfo["nickName"].toString();
	}
		break;
	case 2://2-老师
	{
		QJsonObject sendTeacherInfo = m_obj["sendTeacherInfo"].toObject();
		imagePath = sendTeacherInfo["headimg"].toString();
		nickName = sendTeacherInfo["name"].toString();
	}
		break;
	}
	if(imagePath.contains("/") == false)
		imagePath = "/" + imagePath;
	m_ImagePath = Global::g_ImagePath + imagePath;
	QFile FileImage(m_ImagePath);
	if(FileImage.exists())
		slot_GetHead(QPixmap(m_ImagePath),nullptr);
	else
	{
		NAManager *nam = new NAManager();
		connect(nam,SIGNAL(signal_reply_finished(QPixmap,void*)),this,SLOT(slot_GetHead(QPixmap,void*)));
		nam->SendRequest(QString("http://%1/uds/upload%2").arg(Global::g_ServerIp).arg(imagePath),QString(),Get,nullptr);
	}
	ui->LabelName->setText(nickName);
	ui->LabelTime->setText(m_obj["clmCreateTime"].toString());
	ui->LabelPraiseCount->setText(QString::number(m_obj["clmSupportTotal"].toInt()));
	ui->TEditMessage->setText(m_obj["clmContent"].toString());
	ui->TEditReply->setText(m_obj["clmReplyContent"].toString());
	ui->PButtonPraise->setChecked(m_obj["support"].toString() == "true");

	m_clmId = m_obj["id"].toInt();
	m_ciId = m_obj["ciId"].toInt();
}

void MessageWidget::slot_GetHead(QPixmap pix, void *)
{
	if(pix.isNull())
	{
		qDebug() << "MessageWidget::slot_GetHead:pix.isNull()";
		return ;
	}

	QDir dir(m_ImagePath.left(m_ImagePath.lastIndexOf("/")));
	if(dir.exists() == false)
		dir.mkpath(m_ImagePath.left(m_ImagePath.lastIndexOf("/")));
	pix.scaled(256,256,Qt::IgnoreAspectRatio,Qt::SmoothTransformation).save(m_ImagePath);

	QPixmap pixmap(ui->LabelHead->size());
	pixmap.fill(Qt::transparent);
	QPainter Painter(&pixmap);
	Painter.setRenderHint(QPainter::Antialiasing);
	QPainterPath PainterPath;
	PainterPath.addEllipse(pixmap.rect());
	Painter.setClipPath(PainterPath);
	Painter.drawPixmap(pixmap.rect(), pix);
	ui->LabelHead->setPixmap(pixmap);
}

void MessageWidget::slot_PButtonReplyTop_clicked()
{
	ui->PButtonReplyBottom->show();
	ui->TEditReply->setStyleSheet("border:0px;background-color:rgb(222,222,222);");
	ui->TEditReply->setReadOnly(false);
	ui->TEditReply->setFocus();
}

void MessageWidget::slot_PButtonReplyBottom_clicked()
{
	if(ui->TEditReply->toPlainText().isEmpty())
	{
		Global::g_NoticeDialog->init("回复的内容不能为空！","","",2000);
		Global::g_NoticeDialog->exec();
		return ;
	}

	ui->PButtonReplyBottom->setEnabled(false);
	NAManager *nr = new NAManager();
	connect(nr,SIGNAL(signal_reply_finished(QJsonObject,void*)),this,SLOT(slot_PButtonReplyBottom_reply(QJsonObject,void*)));
	QByteArray ba = QCryptographicHash::hash(QString("clmId=%1&content=%2&criId=%3&type=2&uiId=%4&key=RMS").arg(m_clmId).
											 arg(ui->TEditReply->toPlainText()).arg(m_ciId).arg(Global::g_LoginerId).toUtf8(),
											 QCryptographicHash::Md5);
	QString md5(ba.toHex().toUpper());
	QJsonObject json;
	json.insert("clmId", QString::number(m_clmId));//留言ID
	json.insert("content", ui->TEditReply->toPlainText());
	json.insert("criId", QString::number(m_ciId));//后端接口字段（课程ID）命名有误
	json.insert("type", QString::number(2));//2-回复留言
	json.insert("uiId", QString::number(Global::g_LoginerId));
	json.insert("sign", md5);
	QString strParam = QString("service=course&method=saveOrUploadLeavemsg&params=%1").
			arg(QString(QJsonDocument(json).toJson(QJsonDocument::Compact)));
	nr->SendRequest(QString("http://%1/rms/service/sys/execute").arg(Global::g_ServerIp),strParam);
}

void MessageWidget::slot_PButtonReplyBottom_reply(QJsonObject obj, void *)
{
	ui->PButtonReplyBottom->setEnabled(true);
	ui->PButtonReplyBottom->hide();
	ui->TEditReply->setStyleSheet("border:0px;background-color:transparent;");
	ui->TEditReply->setReadOnly(true);
	if(obj.isEmpty())
	{
		Global::g_NoticeDialog->init("网络异常，回复留言失败！","","",2000);
		Global::g_NoticeDialog->exec();
		return ;
	}
	if (obj["msg"].toString() != "")
	{
		Global::g_NoticeDialog->init(obj["msg"].toString(),"","",2000);
		Global::g_NoticeDialog->exec();
		return ;
	}
	emit signal_MessageReplySuccess();
}

void MessageWidget::slot_PButtonPraise_clicked(bool IsChecked)
{
	ui->PButtonPraise->setEnabled(false);
	if(IsChecked)
	{
		NAManager *nr = new NAManager();
		connect(nr,SIGNAL(signal_reply_finished(QJsonObject,void*)),this,SLOT(slot_PButtonPraiseOk_reply(QJsonObject,void*)));
		QByteArray ba = QCryptographicHash::hash(QString("clmId=%1&criId=%3&supportTotal=1&type=3&uiId=%4&key=RMS").arg(m_clmId).
												 arg(m_ciId).arg(Global::g_LoginerId).toUtf8(),
												 QCryptographicHash::Md5);
		QString md5(ba.toHex().toUpper());
		QJsonObject json;
		json.insert("clmId", QString::number(m_clmId));//留言ID
		json.insert("criId", QString::number(m_ciId));//后端接口字段（课程ID）命名有误
		json.insert("supportTotal", QString::number(1));
		json.insert("type", QString::number(3));//3-更新留言点赞次数
		json.insert("uiId", QString::number(Global::g_LoginerId));
		json.insert("sign", md5);
		QString strParam = QString("service=course&method=saveOrUploadLeavemsg&params=%1").
				arg(QString(QJsonDocument(json).toJson(QJsonDocument::Compact)));
		nr->SendRequest(QString("http://%1/rms/service/sys/execute").arg(Global::g_ServerIp),strParam);
	}
	else
	{
		NAManager *nr = new NAManager();
		connect(nr,SIGNAL(signal_reply_finished(QJsonObject,void*)),this,SLOT(slot_PButtonPraiseCa_reply(QJsonObject,void*)));
		QByteArray ba = QCryptographicHash::hash(QString("clmId=%1&criId=%3&supportTotal=-1&type=3&uiId=%4&key=RMS").arg(m_clmId).
												 arg(m_ciId).arg(Global::g_LoginerId).toUtf8(),
												 QCryptographicHash::Md5);
		QString md5(ba.toHex().toUpper());
		QJsonObject json;
		json.insert("clmId", QString::number(m_clmId));//留言ID
		json.insert("criId", QString::number(m_ciId));//后端接口字段（课程ID）命名有误
		json.insert("supportTotal", QString::number(-1));
		json.insert("type", QString::number(3));//3-更新留言点赞次数
		json.insert("uiId", QString::number(Global::g_LoginerId));
		json.insert("sign", md5);
		QString strParam = QString("service=course&method=saveOrUploadLeavemsg&params=%1").
				arg(QString(QJsonDocument(json).toJson(QJsonDocument::Compact)));
		nr->SendRequest(QString("http://%1/rms/service/sys/execute").arg(Global::g_ServerIp),strParam);
	}
}

void MessageWidget::slot_PButtonPraiseOk_reply(QJsonObject obj, void *)
{
	ui->PButtonPraise->setEnabled(true);
	if(obj.isEmpty())
	{
		Global::g_NoticeDialog->init("网络异常，点赞留言失败！","","",2000);
		Global::g_NoticeDialog->exec();
		return ;
	}
	if (obj["msg"].toString() != "")
	{
		Global::g_NoticeDialog->init(obj["msg"].toString(),"","",2000);
		Global::g_NoticeDialog->exec();
		return ;
	}
	emit signal_MessagePraiseSuccess();
}

void MessageWidget::slot_PButtonPraiseCa_reply(QJsonObject obj, void *)
{
	ui->PButtonPraise->setEnabled(true);
	if(obj.isEmpty())
	{
		Global::g_NoticeDialog->init("网络异常，取消点赞留言失败！","","",2000);
		Global::g_NoticeDialog->exec();
		return ;
	}
	if (obj["msg"].toString() != "")
	{
		Global::g_NoticeDialog->init(obj["msg"].toString(),"","",2000);
		Global::g_NoticeDialog->exec();
		return ;
	}
	emit signal_MessagePraiseSuccess();
}
