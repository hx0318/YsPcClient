#include "barragewidget.h"
#include "ui_barragewidget.h"
#include "namanager.h"
#include "global.h"
#include <QPainter>
#include <QFile>

BarrageWidget::BarrageWidget(int UserId,QString Username,QString SendTime,QString content,QWidget *parent) :
	QWidget(parent),
	ui(new Ui::BarrageWidget)
{
	ui->setupUi(this);
	ui->LabelName->setToolTip(Username);
	if(Username.toStdString().length() > 20)
		Username = Username.left(6) + "...";
	ui->LabelName->setText(Username);
	ui->LabelTime->setText(SendTime);
	ui->TEditContent->setText(content.replace("\n",""));

	NAManager *nam = new NAManager();
	connect(nam,SIGNAL(signal_reply_finished(QJsonObject,void*)),this,SLOT(slot_GetHeadPath(QJsonObject,void*)));
	QByteArray ba = QCryptographicHash::hash(QString("uiId=%1&key=AAS").arg(UserId).toUtf8(),QCryptographicHash::Md5);
	QString md5(ba.toHex().toUpper());
	QJsonObject json;
	json.insert("uiId", QString::number(UserId));
	json.insert("sign", md5);
	QString strParam = QString("service=user&method=queryDetail&params=%1").arg(QString(QJsonDocument(json).
																						toJson(QJsonDocument::Compact)));
	nam->SendRequest(QString("http://%1/aas/service/sys/execute").arg(Global::g_ServerIp),strParam);
}

BarrageWidget::~BarrageWidget()
{
	delete ui;
}

void BarrageWidget::slot_GetHeadPath(QJsonObject obj, void *)
{
	if(obj.isEmpty())
		return ;
	if (obj["msg"].toString() != "")
		return ;

	QJsonObject body = obj["body"].toObject();
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

void BarrageWidget::slot_GetHead(QPixmap pix, void *)
{
	if(pix.isNull())
	{
		qDebug() << "BarrageWidget::slot_GetHead:pix.isNull()";
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

QString BarrageWidget::GetContent()
{
	return ui->TEditContent->toPlainText();
}
