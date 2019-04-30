#include "screenshotwidget.h"
#include "ui_screenshotwidget.h"
#include <Windows.h>

ScreenshotWidget::ScreenshotWidget(QString cpdImgAddr,QString cpdCreateTime,QWidget *parent) :
	QWidget(parent),
	ui(new Ui::ScreenshotWidget)
{
	ui->setupUi(this);
	ui->LabelTime->setText(cpdCreateTime);
	connect(ui->PButtonSee,SIGNAL(clicked(bool)),this,SLOT(slot_PButtonSee_clicked()));

	m_ImagePath = Global::g_ImagePath + cpdImgAddr;
	QFile FileImage(m_ImagePath);
	if(FileImage.exists())
		slot_GetScreenshot(QPixmap(m_ImagePath),nullptr);
	else
	{
		NAManager *nam = new NAManager();
		connect(nam,SIGNAL(signal_reply_finished(QPixmap,void*)),this,SLOT(slot_GetScreenshot(QPixmap,void*)));
		nam->SendRequest(QString("http://%1/uds/upload%2").arg(Global::g_ServerIp).arg(cpdImgAddr),QString(),Get,nullptr);
	}
}

ScreenshotWidget::~ScreenshotWidget()
{
	delete ui;
}

void ScreenshotWidget::slot_GetScreenshot(QPixmap pixmap, void *)
{
	if(pixmap.isNull())
	{
		qDebug() << "ScreenshotWidget::slot_GetScreenshot:pixmap.isNull()";
		return ;
	}

	QDir dir(m_ImagePath.left(m_ImagePath.lastIndexOf("/")));
	if(dir.exists() == false)
		dir.mkpath(m_ImagePath.left(m_ImagePath.lastIndexOf("/")));
	pixmap.save(m_ImagePath);

	int rectW = 0;
	int rectH = 0;
	if(pixmap.width() <= 354 && pixmap.height() <= 155)
	{
		rectW = pixmap.width();
		rectH = pixmap.height();
	}
	else
	{
		float imgBILI = (float)pixmap.width() / (float)pixmap.height();
		if(155 * imgBILI < 354)
		{
			rectH = 155;
			rectW = rectH * imgBILI;
		}
		else
		{
			rectW = 354;
			rectH = rectW / imgBILI;
		}
	}
	pixmap = pixmap.scaled(rectW,rectH,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
	ui->LabelPicture->setFixedSize(QSize(rectW,rectH));
	ui->LabelPicture->setPixmap(pixmap);
}

void ScreenshotWidget::slot_PButtonSee_clicked()
{
	LPCWSTR strValue = reinterpret_cast<LPCWSTR>(m_ImagePath.replace("/","\\").unicode());
	ShellExecute(NULL,TEXT("open"), strValue,NULL, NULL, SW_SHOWNORMAL);
}
