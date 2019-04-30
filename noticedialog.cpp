#include "noticedialog.h"
#include "ui_noticedialog.h"
#pragma execution_character_set("utf-8")

NoticeDialog::NoticeDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::NoticeDialog)
{
	ui->setupUi(this);
	setWindowFlags(windowFlags()|Qt::FramelessWindowHint);
	setWindowTitle("提示");
	connect(ui->btnClose,SIGNAL(clicked()),this,SLOT(reject()));
	connect(ui->btnCancel,SIGNAL(clicked()),this,SLOT(reject()));
	connect(ui->btnOK,SIGNAL(clicked()),this,SLOT(accept()));

	m_Timer = new QTimer(this);
	m_Timer->setSingleShot(true);
	connect(m_Timer,SIGNAL(timeout()),this,SLOT(reject()));
}

NoticeDialog::~NoticeDialog()
{
	if(m_Timer->isActive())
		m_Timer->stop();
	delete m_Timer;
	m_Timer = nullptr;

	delete ui;
}

void NoticeDialog::init(QString strContent, QString strBtn1, QString strBtn2, int iStayTime)
{
	ui->lblContent->setText(strContent);

	if(strBtn1 == "")
		ui->widget_btns->hide();
	else
	{
		ui->btnOK->setText(strBtn1);
		ui->btnCancel->setText(strBtn2);
		ui->widget_btns->show();
	}

	if(iStayTime >= 0)
	{
		if(m_Timer->isActive())
			m_Timer->stop();
		m_Timer->start(iStayTime);
	}
}
