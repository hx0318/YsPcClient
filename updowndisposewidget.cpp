#include "updowndisposewidget.h"
#include "ui_updowndisposewidget.h"
#include "global.h"
#pragma execution_character_set("utf-8")

UpDownDisposeWidget::UpDownDisposeWidget(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::UpDownDisposeWidget)
{
	ui->setupUi(this);

	connect(ui->PButtonClose,SIGNAL(clicked(bool)),this,SLOT(slot_PButtonClose_clicked()));
}

UpDownDisposeWidget::~UpDownDisposeWidget()
{
	delete ui;
}

void UpDownDisposeWidget::slot_PButtonClose_clicked()
{
	Global::g_NoticeDialog->init("确定清空列表？","确定","取消",5000);
	if(Global::g_NoticeDialog->exec() == QDialog::Accepted)
	{
		while(ui->VLayout->isEmpty() == false)
		{
			UpDownWidget *udw = (UpDownWidget *)ui->VLayout->itemAt(0)->widget();
			ui->VLayout->removeWidget(udw);
			delete udw;
			udw = nullptr;
		}
		hide();
	}
}

void UpDownDisposeWidget::add_udw(int iSendUserId, QString sendUsername, int content)
{
	bool if_exist = false;
	for(int i = 0;i < ui->VLayout->count();i++)
	{
		UpDownWidget *udw = (UpDownWidget *)ui->VLayout->itemAt(i)->widget();
		if(udw->m_senderId == iSendUserId)
		{
			if_exist = true;
			udw->setAction(content);
			break;
		}
	}
	if(if_exist == false)
	{
		UpDownWidget *udw = new UpDownWidget(iSendUserId,sendUsername,content,this);
		connect(udw,SIGNAL(signal_agree(UpDownWidget*)),this,SLOT(slot_udw_agree(UpDownWidget*)));
		connect(udw,SIGNAL(signal_ignore(UpDownWidget*)),this,SLOT(slot_udw_ignore(UpDownWidget*)));
		ui->VLayout->addWidget(udw);
		ui->SAWContents->setFixedHeight(ui->VLayout->count()*40);
	}
	show();
}

void UpDownDisposeWidget::slot_udw_agree(UpDownWidget *udw)
{
	emit signal_udw_arrive(udw);
}

void UpDownDisposeWidget::slot_udw_ignore(UpDownWidget *udw)
{
	remove_udw(udw);
}

void UpDownDisposeWidget::remove_udw(UpDownWidget *udw)
{
	if(ui->VLayout->indexOf(udw) >= 0)
	{
		ui->VLayout->removeWidget(udw);
		delete udw;
		udw = nullptr;
	}
	ui->SAWContents->setFixedHeight(ui->VLayout->count()*40);
	if(ui->VLayout->count() <= 0)
		hide();
}
