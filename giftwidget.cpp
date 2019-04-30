#include "giftwidget.h"
#include "ui_giftwidget.h"
#include "global.h"
#pragma execution_character_set("utf-8")

GiftWidget::GiftWidget(QString SenderIcon,QString SenderName,double GiftCount,QWidget *parent) :
	QWidget(parent),
	ui(new Ui::GiftWidget)
{
	ui->setupUi(this);

	ui->LabelSenderName->setToolTip(SenderName);
	if(SenderName.toStdString().length() > 20)
		SenderName = SenderName.left(6) + "...";
	ui->LabelSenderName->setText(SenderName);
	ui->LabelGiftCount->setText(QString::number(GiftCount,'f',2) + "元红包");

	m_ImagePath = Global::g_ImagePath + SenderIcon;
	QFile FileImage(m_ImagePath);
	if(FileImage.exists())
		slot_GetSenderIcon(QPixmap(m_ImagePath),nullptr);
	else
	{
		NAManager *nam = new NAManager();
		connect(nam,SIGNAL(signal_reply_finished(QPixmap,void*)),this,SLOT(slot_GetSenderIcon(QPixmap,void*)));
		nam->SendRequest(QString("http://%1/uds/upload%2").arg(Global::g_ServerIp).arg(SenderIcon),QString(),Get,nullptr);
	}
}

GiftWidget::~GiftWidget()
{
	delete ui;
}

void GiftWidget::slot_GetSenderIcon(QPixmap pix, void *)
{
	if(pix.isNull())
	{
		qDebug() << "GiftWidget::slot_GetSenderIcon:pix.isNull()";
		return ;
	}

	QDir dir(m_ImagePath.left(m_ImagePath.lastIndexOf("/")));
	if(dir.exists() == false)
		dir.mkpath(m_ImagePath.left(m_ImagePath.lastIndexOf("/")));
	pix.scaled(256,256,Qt::IgnoreAspectRatio,Qt::SmoothTransformation).save(m_ImagePath);

	QPixmap pixmap(ui->LabelSenderIcon->size());
	pixmap.fill(Qt::transparent);
	QPainter Painter(&pixmap);
	Painter.setRenderHint(QPainter::Antialiasing);
	QPainterPath PainterPath;
	PainterPath.addEllipse(pixmap.rect());
	Painter.setClipPath(PainterPath);
	Painter.drawPixmap(pixmap.rect(), pix);
	ui->LabelSenderIcon->setPixmap(pixmap);
}
