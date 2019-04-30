#include "mycoursewidget.h"
#include "ui_mycoursewidget.h"
#include "global.h"
#include <QPainter>
#include <QMouseEvent>
#include <QTimer>
#pragma execution_character_set("utf-8")

ImagePathHx::ImagePathHx()
{
	image_path = "";
	label_index = -1;
}

MyCourseWidget::MyCourseWidget(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::MyCourseWidget)
{
	ui->setupUi(this);
	ui->PButtonLeft->hide();
	ui->PButtonRight->hide();
	ui->LabelHead1->hide();
	ui->LabelHead2->hide();
	ui->LabelHead3->hide();
	ui->LabelHead4->hide();
	ui->LabelHead5->hide();
	connect(ui->PButtonLeft,SIGNAL(clicked(bool)),this,SLOT(slot_PButtonLeft_clicked()));
	connect(ui->PButtonRight,SIGNAL(clicked(bool)),this,SLOT(slot_PButtonRight_clicked()));
	ui->WidgetMain->installEventFilter(this);
}

MyCourseWidget::~MyCourseWidget()
{
	ListStringHead.clear();

	delete ui;
}

void MyCourseWidget::SetObj(QJsonObject obj)
{
	m_obj = obj;
	QTimer::singleShot(0,this,SLOT(slot_init()));
}

void MyCourseWidget::slot_init()
{
	ListIndex = -1;

	QJsonObject courseInfo = m_obj["courseInfo"].toObject();
	QJsonObject classroomInfo = m_obj["classroomInfo"].toObject();
	int criId = classroomInfo["criId"].toInt();
	QJsonArray courseplanList = m_obj["courseplanList"].toArray();

	QString coverImg = courseInfo["coverImg"].toString();
	if(coverImg.contains("/") == false)
		coverImg = "/" + coverImg;
	m_CoverImage = Global::g_ImagePath + coverImg;
	QFile FileImage(m_CoverImage);
	if(FileImage.exists())
		slot_GetPoster(QPixmap(m_CoverImage),nullptr);
	else
	{
		NAManager *nam = new NAManager();
		connect(nam,SIGNAL(signal_reply_finished(QPixmap,void*)),this,SLOT(slot_GetPoster(QPixmap,void*)));
		nam->SendRequest(QString("http://%1/uds/upload%2").arg(Global::g_ServerIp).arg(coverImg),QString(),Get,nullptr);
	}
	QStringList str_lis = classroomInfo["name"].toString().split("|");
	ui->LabelName->setText(QString("%1(%2)").arg(str_lis.at(0)).arg(criId));
	QString begintime = courseInfo["begintime"].toString();
	QString endtime = courseInfo["endtime"].toString();
	if(courseInfo["type"].toInt() == 2)
		ui->LabelTime->setText(QString("%1-%2 %3 %4 上课").arg(begintime.replace("-",".")).arg(endtime.replace("-",".")).
							   arg(str_lis.at(1)).arg(str_lis.at(2)));
	else
		ui->LabelTime->setText(QString("%1-%2 %3 上课").arg(begintime.replace("-",".")).arg(endtime.replace("-","."))
							   .arg(str_lis.at(2)));
	int cnt = 0;
	for (int i = 0; i< courseplanList.size(); i++)
	{
		QJsonObject jo = courseplanList.at(i).toObject();
		if (jo["ccpStatus"].toInt() == 3)
			cnt++;
	}
	ui->LabelNumber->setText(QString("更新%1课/共%2课").arg(cnt).arg(courseplanList.size()));

	QJsonObject buyInfo = courseInfo["buyInfo"].toObject();
	ui->LabelHaveNumber->setText(QString::number(buyInfo["buyTotal"].toInt()));
	ui->LabelVip->setText(QString("人在听课，VIP用户%1人").arg(buyInfo["vipTotal"].toInt()));

	QJsonArray vipInfos = buyInfo["vipInfos"].toArray();
	if(vipInfos.count() > 5)
	{
		ui->PButtonLeft->show();
		ui->PButtonRight->show();
	}
	for(int i = 0;i < vipInfos.count();i++)
	{
		QJsonObject jo = vipInfos.at(i).toObject();
		QString imagePath = jo["imagePath"].toString();
		if(imagePath.contains("/") == false)
			imagePath = "/" + imagePath;
		ListStringHead.append(imagePath);
		if(ListIndex <= 3)
		{
			ImagePathHx *iph = new ImagePathHx();
			iph->image_path = Global::g_ImagePath + imagePath;
			iph->label_index = ListIndex + 2;
			GetLabel(iph)->show();
			QFile FileImage(iph->image_path);
			if(FileImage.exists())
				slot_GetVipHead(QPixmap(iph->image_path),iph);
			else
			{
				NAManager *nr = new NAManager();
				connect(nr,SIGNAL(signal_reply_finished(QPixmap,void*)),this,SLOT(slot_GetVipHead(QPixmap,void*)));
				nr->SendRequest(QString("http://%1/uds/upload%2").arg(Global::g_ServerIp).arg(imagePath),QString(),Get,iph);
			}
			ListIndex++;
		}
		else
		{
			ImagePathHx *iph = new ImagePathHx();
			iph->image_path = Global::g_ImagePath + imagePath;
			iph->label_index = -1;
			QFile FileImage(iph->image_path);
			if(FileImage.exists() == false)
			{
				NAManager *nr = new NAManager();
				connect(nr,SIGNAL(signal_reply_finished(QPixmap,void*)),this,SLOT(slot_GetVipHead(QPixmap,void*)));
				nr->SendRequest(QString("http://%1/uds/upload%2").arg(Global::g_ServerIp).arg(imagePath),QString(),Get,iph);
			}
		}
	}
}

void MyCourseWidget::slot_GetPoster(QPixmap pix, void *)
{
	if(pix.isNull())
	{
		qDebug() << "MyCourseWidget::slot_GetPoster:pix.isNull()";
		return ;
	}

	QDir dir(m_CoverImage.left(m_CoverImage.lastIndexOf("/")));
	if(dir.exists() == false)
		dir.mkpath(m_CoverImage.left(m_CoverImage.lastIndexOf("/")));
	pix.scaled(60,84,Qt::IgnoreAspectRatio,Qt::SmoothTransformation).save(m_CoverImage);

	ui->LabelPoster->setPixmap(pix.scaled(ui->LabelPoster->size(),Qt::IgnoreAspectRatio,Qt::SmoothTransformation));
}

void MyCourseWidget::slot_GetVipHead(QPixmap pix, void *da)
{
	ImagePathHx *iph = (ImagePathHx *)da;
	if(pix.isNull())
	{
		qDebug() << "MyCourseWidget::slot_GetVipHead:pix.isNull()";
		delete iph;
		iph = nullptr;
		return ;
	}

	QDir dir(iph->image_path.left(iph->image_path.lastIndexOf("/")));
	if(dir.exists() == false)
		dir.mkpath(iph->image_path.left(iph->image_path.lastIndexOf("/")));
	pix.scaled(256,256,Qt::IgnoreAspectRatio,Qt::SmoothTransformation).save(iph->image_path);

	if(iph->label_index != -1)
		SetLabelPixmap(GetLabel(iph),pix);

	delete iph;
	iph = nullptr;
}

void MyCourseWidget::SetLabelPixmap(QLabel *lab, QPixmap pix)
{
	QPixmap pixmap(lab->size());
	pixmap.fill(Qt::transparent);
	QPainter Painter(&pixmap);
	Painter.setRenderHint(QPainter::Antialiasing);
	QPainterPath PainterPath;
	PainterPath.addEllipse(pixmap.rect());
	Painter.setClipPath(PainterPath);
	Painter.drawPixmap(pixmap.rect(), pix);
	lab->setPixmap(pixmap);
}

void MyCourseWidget::slot_PButtonLeft_clicked()
{
	if(ListIndex == 4)
		return ;

	ui->LabelHead1->setPixmap(QPixmap());
	ui->LabelHead2->setPixmap(QPixmap());
	ui->LabelHead3->setPixmap(QPixmap());
	ui->LabelHead4->setPixmap(QPixmap());
	ui->LabelHead5->setPixmap(QPixmap());

	for(int i = 0;i < 5;i++)
	{
		switch(i)
		{
		case 0:
		{
			ListIndex = ListIndex - (ListIndex + 1) % 5;

			ImagePathHx *iph = new ImagePathHx();
			iph->image_path = Global::g_ImagePath + ListStringHead.at(ListIndex);
			iph->label_index = 5;
			QFile FileImage(iph->image_path);
			if(FileImage.exists())
				slot_GetVipHead(QPixmap(iph->image_path),iph);
			else
			{
				delete iph;
				iph = nullptr;
			}
		}
			break;
		case 1:
		case 2:
		case 3:
		case 4:
		{
			ListIndex--;

			ImagePathHx *iph = new ImagePathHx();
			iph->image_path = Global::g_ImagePath + ListStringHead.at(ListIndex);
			iph->label_index = 5 - i;
			QFile FileImage(iph->image_path);
			if(FileImage.exists())
				slot_GetVipHead(QPixmap(iph->image_path),iph);
			else
			{
				delete iph;
				iph = nullptr;
			}
		}
			break;
		default:
			break;
		}
	}
	ListIndex = ListIndex + 4;
}

void MyCourseWidget::slot_PButtonRight_clicked()
{
	if(ListStringHead.count() - 1 == ListIndex)
		return ;

	ui->LabelHead1->setPixmap(QPixmap());
	ui->LabelHead2->setPixmap(QPixmap());
	ui->LabelHead3->setPixmap(QPixmap());
	ui->LabelHead4->setPixmap(QPixmap());
	ui->LabelHead5->setPixmap(QPixmap());

	for(int i = 0;i < 5;i++)
	{
		switch(i)
		{
		case 0:
		{
			if(ListStringHead.count() - 1 - ListIndex >= 5)
				ListIndex++;
			else
				ListIndex = (ListStringHead.count() - 1 - ListIndex) % 5 + (ListIndex / 5) * 5;

			ImagePathHx *iph = new ImagePathHx();
			iph->image_path = Global::g_ImagePath + ListStringHead.at(ListIndex);
			iph->label_index = i + 1;
			QFile FileImage(iph->image_path);
			if(FileImage.exists())
				slot_GetVipHead(QPixmap(iph->image_path),iph);
			else
			{
				delete iph;
				iph = nullptr;
			}
		}
			break;
		case 1:
		case 2:
		case 3:
		case 4:
		{
			ListIndex++;

			ImagePathHx *iph = new ImagePathHx();
			iph->image_path = Global::g_ImagePath + ListStringHead.at(ListIndex);
			iph->label_index = i + 1;
			QFile FileImage(iph->image_path);
			if(FileImage.exists())
				slot_GetVipHead(QPixmap(iph->image_path),iph);
			else
			{
				delete iph;
				iph = nullptr;
			}
		}
			break;
		default:
			break;
		}
	}
}

bool MyCourseWidget::eventFilter(QObject *obj, QEvent *event)
{
	if(obj == ui->WidgetMain)
	{
		if(event->type() == QEvent::MouseButtonPress)
		{
			QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
			if (mouseEvent->button() == Qt::LeftButton)
				emit signal_OpenCourseInfo(m_obj);
		}
	}

	return QObject::eventFilter(obj, event);
}

void MyCourseWidget::ClearWidgetMain()
{
	ui->WidgetMain->setStyleSheet("");
}

void MyCourseWidget::setStatus(QJsonObject obj)
{
	int byear, bmonth, bday, bhour, bminute, eyear, emonth, eday, ehour, eminute;
	QJsonArray courseplanList = obj["courseplanList"].toArray();
	for (int i = 0; i< courseplanList.size(); i++)
	{
		QJsonObject jo = courseplanList.at(i).toObject();
		if (i == 0)
		{
			QString beginDate = jo["beginDate"].toString();
			sscanf(beginDate.toStdString().c_str(), "%d-%d-%d", &byear, &bmonth, &bday);
			QString beginTime = jo["beginTime"].toString();
			sscanf(beginTime.toStdString().c_str(), "%d:%d", &bhour, &bminute);
		}
		if(i == courseplanList.size()-1)
		{
			QString beginDate = jo["beginDate"].toString();
			sscanf(beginDate.toStdString().c_str(), "%d-%d-%d", &eyear, &emonth, &eday);
			QString endTime = jo["endTime"].toString();
			sscanf(endTime.toStdString().c_str(), "%d:%d", &ehour, &eminute);
		}
	}
	QDateTime DTimeBegin = QDateTime(QDate(byear,bmonth,bday),QTime(bhour,bminute,0));
	QDateTime DTimeEnd = QDateTime(QDate(eyear,emonth,eday),QTime(ehour,eminute,0));
	QDateTime DTimeCur = QDateTime::currentDateTime();
	int cur_sub_end = DTimeEnd.secsTo(DTimeCur);
	if(cur_sub_end >= 0)
	{
		m_status = "已完结";
		ui->LabelStatus->setText("已完结");
		ui->LabelStatus->setStyleSheet("background:none;color: rgb(154, 154, 154);");
	}
	else
	{
		int cur_sub_begin = DTimeBegin.secsTo(DTimeCur);
		if(cur_sub_begin >= 0)
		{
			m_status = "正在更新";
			ui->LabelStatus->setText("正在更新");
			ui->LabelStatus->setStyleSheet("background:none;color: rgb(237, 113, 46);");
		}
		else
		{
			m_status = "未开始";
			ui->LabelStatus->setText("未开始");
			ui->LabelStatus->setStyleSheet("background:none;color: rgb(21, 30, 52);");
		}
	}
}

QLabel *MyCourseWidget::GetLabel(ImagePathHx *iph)
{
	switch(iph->label_index)
	{
	case 1:
	default:
		return ui->LabelHead1;
	case 2:
		return ui->LabelHead2;
	case 3:
		return ui->LabelHead3;
	case 4:
		return ui->LabelHead4;
	case 5:
		return ui->LabelHead5;
	}
}
