#include "coursewidget.h"
#include "ui_coursewidget.h"
#pragma execution_character_set("utf-8")

CourseWidget::CourseWidget(QJsonObject obj,int LineMark,QWidget *parent) :
	QWidget(parent),
	ui(new Ui::CourseWidget)
{
	ui->setupUi(this);
	ui->LabelRemind_1->hide();
	ui->LabelRemind_2->hide();
	ui->PButtonEnterclassroom->setEnabled(false);
	connect(ui->PButtonEnterclassroom,SIGNAL(clicked(bool)),this,SLOT(slot_PButtonEnterclassroom_clicked()));

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

	QString beginTime = obj["beginTime"].toString();
	QString endTime = obj["endTime"].toString();
	ui->LabelTimeslot->setText(QString("%1-%2").arg(beginTime).arg(endTime));
	m_cpiName = obj["cpiName"].toString();
	ui->LabelDescribe->setText(m_cpiName);
	QString beginDate = obj["beginDate"].toString();
	int year, month, day, bhour, bminute, ehour, eminute;
	sscanf(beginDate.toStdString().c_str(), "%d-%d-%d", &year, &month, &day);
	sscanf(beginTime.toStdString().c_str(), "%d:%d", &bhour, &bminute);
	sscanf(endTime.toStdString().c_str(), "%d:%d", &ehour, &eminute);
	m_DTimeBegin = QDateTime(QDate(year,month,day),QTime(bhour,bminute));
	m_DTimeEnd = QDateTime(QDate(year,month,day),QTime(ehour,eminute));
	m_RoomId = obj["criId"].toInt();
	m_crcpId = obj["id"].toInt();
	DTimeClassBegin = QDateTime(QDate::currentDate(),QTime(bhour,bminute));
	DTimeClassOver = QDateTime(QDate::currentDate(),QTime(ehour,eminute));

	connect(&m_timer,SIGNAL(timeout()),this,SLOT(slot_timer_timeout()));
	m_timer.start(1000);
}

CourseWidget::~CourseWidget()
{
	if(m_timer.isActive())
		m_timer.stop();

	delete ui;
}

void CourseWidget::slot_timer_timeout()
{
	QDateTime DTimeCur = QDateTime::currentDateTime();
	int cur_sub_end = m_DTimeEnd.secsTo(DTimeCur);
	if(cur_sub_end >= 0)
	{
		ui->LabelRemind_2->setStyleSheet("");
		ui->LabelRemind_2->setText("已上完");
		ui->LabelRemind_2->show();
		ui->LabelRemind_1->hide();
		ui->PButtonEnterclassroom->setEnabled(false);
		ui->LabelStatus->setStyleSheet("image: url(:/res/teacher/实心圆.png);");
	}
	else
	{
		int cur_sub_begin = m_DTimeBegin.secsTo(DTimeCur);
		if(cur_sub_begin >= 0)
		{
			ui->LabelRemind_2->setStyleSheet("color: rgb(254, 132, 49);");
			ui->LabelRemind_2->setText("正在上课");
			ui->LabelRemind_2->show();
			ui->PButtonEnterclassroom->setEnabled(true);
			ui->LabelRemind_1->hide();
			ui->LabelStatus->setStyleSheet("image: url(:/res/teacher/空心圆.png);");
		}
		else
		{
			ui->LabelRemind_1->setText("上课倒计时：");
			ui->LabelRemind_1->show();
			cur_sub_begin = 0 - cur_sub_begin;
			if(cur_sub_begin >= 86400)
			{
				ui->LabelRemind_2->setStyleSheet("color: rgb(255, 0, 0);");
				ui->LabelRemind_2->setText("超过一天");
				ui->LabelRemind_2->show();
				ui->PButtonEnterclassroom->setEnabled(false);
				ui->LabelStatus->setStyleSheet("image: url(:/res/teacher/实心圆.png);");
			}
			else if(cur_sub_begin > 600 && cur_sub_begin < 86400)
			{
				ui->LabelRemind_2->setStyleSheet("color: rgb(254, 132, 49);");
				ui->LabelRemind_2->setText(QString("%1:%2:%3").arg(cur_sub_begin/3600,2,10,QLatin1Char('0'))
										   .arg(cur_sub_begin%3600/60,2,10,QLatin1Char('0')).
										   arg(cur_sub_begin%60,2,10,QLatin1Char('0')));
				ui->LabelRemind_2->show();
				ui->PButtonEnterclassroom->setEnabled(false);
				ui->LabelStatus->setStyleSheet("image: url(:/res/teacher/实心圆.png);");
			}
			else
			{
				ui->LabelRemind_2->setStyleSheet("color: rgb(254, 132, 49);");
				ui->LabelRemind_2->setText(QString("%1:%2:%3").arg(cur_sub_begin/3600,2,10,QLatin1Char('0'))
										   .arg(cur_sub_begin%3600/60,2,10,QLatin1Char('0')).
										   arg(cur_sub_begin%60,2,10,QLatin1Char('0')));
				ui->LabelRemind_2->show();
				ui->PButtonEnterclassroom->setEnabled(true);
				ui->LabelStatus->setStyleSheet("image: url(:/res/teacher/实心圆.png);");
			}
		}
	}
}

void CourseWidget::slot_PButtonEnterclassroom_clicked()
{
	emit signal_EnterClassroom(this);
}
