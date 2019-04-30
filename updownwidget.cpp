#include "updownwidget.h"
#include "ui_updownwidget.h"
#pragma execution_character_set("utf-8")

UpDownWidget::UpDownWidget(int iSendUserId,QString sendUsername,int content,QWidget *parent) :
	QWidget(parent),
	ui(new Ui::UpDownWidget)
{
	ui->setupUi(this);

	setAction(content);
	m_senderId = iSendUserId;
	ui->LabelName->setToolTip(sendUsername);
	if(sendUsername.toStdString().length() > 20)
		sendUsername = sendUsername.left(6) + "...";
	ui->LabelName->setText(sendUsername);

	connect(ui->PButtonAgree,SIGNAL(clicked(bool)),this,SLOT(slot_PButtonAgree_clicked()));
	connect(ui->PButtonIgnore,SIGNAL(clicked(bool)),this,SLOT(slot_PButtonIgnore_clicked()));
}

UpDownWidget::~UpDownWidget()
{
	delete ui;
}

void UpDownWidget::slot_PButtonAgree_clicked()
{
	emit signal_agree(this);
}

void UpDownWidget::slot_PButtonIgnore_clicked()
{
	emit signal_ignore(this);
}

void UpDownWidget::setAction(int content)
{
	m_UpOrDown = content;
	if(m_UpOrDown == 1)
		ui->LabelAction->setText("请求上台");
	else
		ui->LabelAction->setText("请求下台");
}
