#include "mediawidget.h"
#include "ui_mediawidget.h"
#include <QPainter>
#include "global.h"

MediaWidget::MediaWidget(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::MediaWidget)
{
	ui->setupUi(this);

	ClearAll();
	connect(ui->PButtonVideo,SIGNAL(clicked(bool)),this,SLOT(slot_PButtonVideo_clicked()));
	connect(ui->PButtonAudio,SIGNAL(clicked(bool)),this,SLOT(slot_PButtonAudio_clicked()));
	connect(ui->PButtonUp,SIGNAL(clicked(bool)),this,SLOT(slot_PButtonUp_clicked()));
	connect(ui->PButtonDown,SIGNAL(clicked(bool)),this,SLOT(slot_PButtonDown_clicked()));
}

MediaWidget::~MediaWidget()
{
	delete ui;
}

void MediaWidget::paintEvent(QPaintEvent *)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setBrush(QBrush(Qt::transparent,m_Pixmap));
	painter.setPen(Qt::transparent);
	painter.drawRect(m_Pixmap.rect());
//	painter.drawRoundRect(QRect(m_Pixmap.rect().x(),m_Pixmap.rect().y(),m_Pixmap.width()-1,m_Pixmap.height()-1),15,15);
}

void MediaWidget::ChangePixmap(QPixmap Pixmap)
{
	if(Pixmap.isNull())
		Pixmap = QPixmap(":/res/media/video_default.png");
	m_Pixmap = Pixmap.scaled(size(),Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
	update();
}

void MediaWidget::resizeEvent(QResizeEvent *)
{
	ChangePixmap(QPixmap());
}

void MediaWidget::SetVideoStatus(int st)
{
	m_VideoStatus = st;
	if(m_VideoStatus == 1)
		ui->PButtonVideo->setStyleSheet("QPushButton{border-image: url(:/res/media/VideoOpen.png);}");
	else
		ui->PButtonVideo->setStyleSheet("QPushButton{border-image: url(:/res/media/VideoClose.png);}");
}

void MediaWidget::SetAudioStatus(int st)
{
	m_AudioStatus = st;
	if(m_AudioStatus == 1)
		ui->PButtonAudio->setStyleSheet("QPushButton{border-image: url(:/res/media/AudioOpen.png);}");
	else
		ui->PButtonAudio->setStyleSheet("QPushButton{border-image: url(:/res/media/AudioClose.png);}");
}

void MediaWidget::SetHeadName()
{
	NAManager *nam = new NAManager();
	connect(nam,SIGNAL(signal_reply_finished(QJsonObject,void*)),this,SLOT(slot_GetHeadPath(QJsonObject,void*)));
	if(m_id == Global::g_LoginerId)
	{
		QByteArray ba = QCryptographicHash::hash(QString("tiId=%1&key=AAS").arg(Global::g_LoginerId).toUtf8(),
												 QCryptographicHash::Md5);
		QString md5(ba.toHex().toUpper());
		QJsonObject json;
		json.insert("tiId", QString::number(Global::g_LoginerId));
		json.insert("sign", md5);
		QString strParam = QString("service=teacher&method=queryDetail&params=%1").arg(QString(QJsonDocument(json).
																						 toJson(QJsonDocument::Compact)));
		nam->SendRequest(QString("http://%1/aas/service/sys/execute").arg(Global::g_ServerIp),strParam);
	}
	else
	{
		QByteArray ba = QCryptographicHash::hash(QString("uiId=%1&key=AAS").arg(m_id).toUtf8(),QCryptographicHash::Md5);
		QString md5(ba.toHex().toUpper());
		QJsonObject json;
		json.insert("uiId", QString::number(m_id));
		json.insert("sign", md5);
		QString strParam = QString("service=user&method=queryDetail&params=%1").arg(QString(QJsonDocument(json).
																							toJson(QJsonDocument::Compact)));
		nam->SendRequest(QString("http://%1/aas/service/sys/execute").arg(Global::g_ServerIp),strParam);
	}
}

void MediaWidget::slot_GetHeadPath(QJsonObject obj, void *)
{
	if(obj.isEmpty())
		return ;
	if (obj["msg"].toString() != "")
		return ;

	QJsonObject body = obj["body"].toObject();
	QString str_name = "";
	if(m_id == Global::g_LoginerId)
		str_name = body["name"].toString();
	else
		str_name = body["nickName"].toString();
	ui->LabelName->setToolTip(str_name);
	if(str_name.toStdString().length() > 20)
		str_name = str_name.left(6) + "...";
	ui->LabelName->setText(str_name);
	QString imagePath = "";
	if(m_id == Global::g_LoginerId)
		imagePath = body["headimg"].toString();
	else
		imagePath = body["imagePath"].toString();
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

void MediaWidget::slot_GetHead(QPixmap pix, void *)
{
	if(pix.isNull())
	{
		qDebug() << "MediaWidget::slot_GetHead:pix.isNull()";
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

void MediaWidget::ClearAll()
{
	m_id = -1;
	m_VideoStatus = 0;
	m_AudioStatus = 0;
	ui->LabelHead->setPixmap(QPixmap());
	ui->LabelName->setText("");
	ui->PButtonUp->hide();
	ui->PButtonDown->hide();
	ui->PButtonVideo->setStyleSheet("QPushButton{border-image: url(:/res/media/VideoClose.png);}");
	ui->PButtonAudio->setStyleSheet("QPushButton{border-image: url(:/res/media/AudioClose.png);}");
	ChangePixmap(QPixmap());
}

void MediaWidget::slot_PButtonVideo_clicked()
{
	if(m_VideoStatus == 0)
		emit signal_VideoOpen(m_id);
	else
		emit signal_VideoClose(m_id);
	ui->PButtonVideo->setEnabled(false);
	QTimer::singleShot(1000, this, SLOT(slot_PButtonVideo_Enable()));
}

void MediaWidget::slot_PButtonVideo_Enable()
{
	ui->PButtonVideo->setEnabled(true);
}

void MediaWidget::slot_PButtonAudio_clicked()
{
	if(m_AudioStatus == 0)
		emit signal_AudioOpen(m_id);
	else
		emit signal_AudioClose(m_id);
}

void MediaWidget::enterEvent(QEvent *)
{
	if(m_id != -1 && m_id != Global::g_LoginerId && ui->PButtonDown->isHidden())
		ui->PButtonUp->show();
}

void MediaWidget::leaveEvent(QEvent *)
{
	ui->PButtonUp->hide();
}

void MediaWidget::slot_PButtonUp_clicked()
{
	emit signal_MediaUp(m_id);
}

void MediaWidget::slot_PButtonDown_clicked()
{
	emit signal_MediaDown(m_id);
}

void MediaWidget::ShowPButtonDown()
{
	ui->PButtonDown->show();
}
