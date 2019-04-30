#include "coursewareoutwidget.h"
#include "ui_coursewareoutwidget.h"
#include <QJsonObject>
#include <QJsonArray>
#pragma execution_character_set("utf-8")

CoursewareOutWidget::CoursewareOutWidget(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::CoursewareOutWidget)
{
	ui->setupUi(this);
	ui->PButtonEnterClassroom->hide();
	ui->PButtonReplay->hide();
	ui->PButtonSee->hide();
	connect(ui->PButtonEnterClassroom,SIGNAL(clicked(bool)),this,SLOT(slot_PButtonEnterClassroom_clicked()));
	connect(ui->PButtonReplay,SIGNAL(clicked(bool)),this,SLOT(slot_PButtonReplay_clicked()));
	connect(ui->PButtonSee,SIGNAL(clicked(bool)),this,SLOT(slot_PButtonSee_clicked()));
}

CoursewareOutWidget::~CoursewareOutWidget()
{
	delete ui;
}

void CoursewareOutWidget::slot_PButtonEnterClassroom_clicked()
{
	emit signal_EnterClassroom(this);
}

void CoursewareOutWidget::slot_PButtonReplay_clicked()
{
	emit signal_OpenRecordPlay(this);
}

void CoursewareOutWidget::Init_enter_reply(QJsonObject obj, int LineMark)
{
	switch(LineMark)
	{
	case 0:
	default:
		ui->LabelTopLine->show();
		ui->LabelBottomLine->show();
		break;
	case 1:
		ui->LabelTopLine->hide();
		ui->LabelBottomLine->show();
		break;
	case 2:
		ui->LabelTopLine->show();
		ui->LabelBottomLine->hide();
		break;
	case 3:
		ui->LabelTopLine->hide();
		ui->LabelBottomLine->hide();
		break;
	}
	QString beginDate = obj["beginDate"].toString();
	QString beginTime = obj["beginTime"].toString();
	QString endTime = obj["endTime"].toString();
	int year, month, day, bhour, bminute, ehour, eminute;
	sscanf(beginDate.toStdString().c_str(), "%d-%d-%d", &year, &month, &day);
	sscanf(beginTime.toStdString().c_str(), "%d:%d", &bhour, &bminute);
	sscanf(endTime.toStdString().c_str(), "%d:%d", &ehour, &eminute);
	DTimeClassBegin = QDateTime(QDate::currentDate(),QTime(bhour,bminute));
	DTimeClassOver = QDateTime(QDate::currentDate(),QTime(ehour,eminute));
	QDateTime DTimeBegin = QDateTime(QDate(year,month,day),QTime(bhour,bminute));
	QDateTime DTimeEnd = QDateTime(QDate(year,month,day),QTime(ehour,eminute));
	QDateTime DTimeCur = QDateTime::currentDateTime();
	int cur_sub_end = DTimeEnd.secsTo(DTimeCur);
	if(cur_sub_end >= 0)
	{
		ui->LabelTitle->setText(QString("%1月%2日").arg(month).arg(day));
		ui->PButtonReplay->show();
	}
	else
	{
		int cur_sub_begin = DTimeBegin.secsTo(DTimeCur);
		if(cur_sub_begin >= 0)
		{
			ui->LabelTitle->setText("今天");
			ui->PButtonEnterClassroom->show();
		}
		else
			ui->LabelTitle->setText(QString("%1月%2日").arg(month).arg(day));
	}
	m_cpiName = obj["cpiName"].toString();
	ui->LabelDescribe->setText(m_cpiName);

	m_RoomId = obj["criId"].toInt();
	m_crcpId = obj["id"].toInt();
}

void CoursewareOutWidget::Init_see(QString str_des, int LineMark)
{
	switch(LineMark)
	{
	case 0:
	default:
		ui->LabelTopLine->show();
		ui->LabelBottomLine->show();
		break;
	case 1:
		ui->LabelTopLine->hide();
		ui->LabelBottomLine->show();
		break;
	case 2:
		ui->LabelTopLine->show();
		ui->LabelBottomLine->hide();
		break;
	case 3:
		ui->LabelTopLine->hide();
		ui->LabelBottomLine->hide();
		break;
	}
	ui->LabelTitle->setText("课程表");
	ui->PButtonSee->show();
	ui->LabelDescribe->setText(str_des);
}

void CoursewareOutWidget::slot_PButtonSee_clicked()
{
	emit signal_SeeCurriculumSchedule();
}
