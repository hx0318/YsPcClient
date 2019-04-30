#include "studentwidget.h"
#include "ui_studentwidget.h"
#include <QMenu>
#include <QContextMenuEvent>
#include "namanager.h"
#include "global.h"
#include <QPainter>
#pragma execution_character_set("utf-8")

StudentWidget::StudentWidget(int id,int UserType,QWidget *parent) :
	QWidget(parent),
	ui(new Ui::StudentWidget)
{
	ui->setupUi(this);

	m_id = id;
	m_type = UserType;
	ui->LabelId->setText(QString("ID: %1").arg(id));
	NAManager *nam = new NAManager();
	connect(nam,SIGNAL(signal_reply_finished(QJsonObject,void*)),this,SLOT(slot_GetHeadPath(QJsonObject,void*)));
	QByteArray ba = QCryptographicHash::hash(QString("uiId=%1&key=AAS").arg(id).toUtf8(),QCryptographicHash::Md5);
	QString md5(ba.toHex().toUpper());
	QJsonObject json;
	json.insert("uiId", QString::number(id));
	json.insert("sign", md5);
	QString strParam = QString("service=user&method=queryDetail&params=%1").arg(QString(QJsonDocument(json).toJson(QJsonDocument::Compact)));
	nam->SendRequest(QString("http://%1/aas/service/sys/execute").arg(Global::g_ServerIp),strParam);

	VideoStatus = 0;
	AudioStatus = 0;
	HaveVideoStream = false;
	IsTeacherCloseVideo = false;
}

StudentWidget::~StudentWidget()
{
	delete ui;
}

void StudentWidget::slot_GetHeadPath(QJsonObject obj, void *)
{
	if(obj.isEmpty())
		return ;
	if (obj["msg"].toString() != "")
		return ;

	QJsonObject body = obj["body"].toObject();
	QString str_name = body["nickName"].toString();
	ui->LabelName->setToolTip(str_name);
	if(str_name.toStdString().length() > 20)
		str_name = str_name.left(6) + "...";
	ui->LabelName->setText(str_name);
	QString imagePath = body["imagePath"].toString();
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
}

void StudentWidget::slot_GetHead(QPixmap pix, void *)
{
	if(pix.isNull())
	{
		qDebug() << "StudentWidget::slot_GetHead:pix.isNull()";
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

void StudentWidget::contextMenuEvent(QContextMenuEvent *event)
{
	if(m_type == 2)
		return ;

	QMenu menu;
	if(VideoStatus == 1 || HaveVideoStream)
		menu.addAction("关闭视频",this, SLOT(slot_VideoClose()));
	else if(VideoStatus == 0)
		menu.addAction("打开视频",this, SLOT(slot_VideoOpen()));
	if(AudioStatus == 0)
		menu.addAction("打开音频",this, SLOT(slot_AudioOpen()));
	else if(AudioStatus == 1)
		menu.addAction("关闭音频",this, SLOT(slot_AudioClose()));
	menu.setStyleSheet("QMenu{background-color: rgb(255,255,255);}QMenu::item {background-color: transparent;height:32px;"
					   "padding-left:30px;padding-right:30px;font: 12px \"宋体\";}QMenu::item:selected { background-color: "
					   "rgb(0,180,255);}");
	menu.exec(event->globalPos());
}

void StudentWidget::slot_VideoOpen()
{
	emit signal_VideoOpen(m_id);
}

void StudentWidget::slot_VideoClose()
{
	emit signal_VideoClose(m_id);
}

void StudentWidget::slot_AudioOpen()
{
	emit signal_AudioOpen(m_id);
}

void StudentWidget::slot_AudioClose()
{
	emit signal_AudioClose(m_id);
}
