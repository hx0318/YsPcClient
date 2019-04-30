#include "courseinfodialog.h"
#include "ui_courseinfodialog.h"
#include "global.h"
#include "classroomdialog.h"
#include "recordplaydialog.h"
#include <QScrollBar>
#include <QCloseEvent>
#pragma execution_character_set("utf-8")

UrlImg::UrlImg(QString url,QString path)
{
	img_url = url;
	img_path = path;
}

CourseInfoDialog::CourseInfoDialog(QJsonObject obj,QWidget *parent) :
	QDialog(parent),
	ui(new Ui::CourseInfoDialog)
{
	ui->setupUi(this);
	setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint);
	setWindowTitle("课程详情");
	connect(ui->PButtonAbout,SIGNAL(clicked(bool)),this,SLOT(slot_PButtonAbout_clicked(bool)));
	QButtonGroup *bg = new QButtonGroup(this);
	bg->addButton(ui->PButtonCourse,0);
	bg->addButton(ui->PButtonAbout,1);
	bg->addButton(ui->PButtonMessage,2);
	ui->PButtonCourse->setChecked(true);
	ui->stackedWidget->setCurrentWidget(ui->PageCourse);
	connect(ui->PButtonCourse,SIGNAL(clicked(bool)),this,SLOT(slot_PButtonCourse_clicked(bool)));
	connect(ui->PButtonMessage,SIGNAL(clicked(bool)),this,SLOT(slot_PButtonMessage_clicked(bool)));
	connect(ui->PButtonPublish,SIGNAL(clicked(bool)),this,SLOT(slot_PButtonPublish_clicked()));
	ui->TEditMessage->setContextMenuPolicy(Qt::NoContextMenu);
	connect(ui->TEditMessage,SIGNAL(textChanged()),this,SLOT(slot_TEditMessage_textChanged()));

	m_string_html = "";
	MapUrlImg.clear();
	m_LookWidget = new LookWidget(this);

	m_obj = obj;
	QTimer::singleShot(0,this,SLOT(slot_init()));
}

CourseInfoDialog::~CourseInfoDialog()
{
	QMapIterator<QString,UrlImg *> mui(MapUrlImg);
	while (mui.hasNext())
	{
		mui.next();
		UrlImg *ulig = mui.value();
		delete ulig;
		ulig = nullptr;
	}
	Global::DelDir(Global::g_AppDataPath + "html_image_temp/");

	delete ui;
}

void CourseInfoDialog::slot_GetHtmlImage(QPixmap pix, void *da)
{
	if(pix.isNull())
		return ;
	UrlImg *ulig = (UrlImg *)da;
	pix.save(ulig->img_path);
	if(MapUrlImg.contains(ulig->img_url))
		MapUrlImg.remove(ulig->img_url);
	delete ulig;
	ulig = nullptr;
	if(MapUrlImg.count() == 0)
		ui->TEditAbout->setHtml(m_string_html);
}

void CourseInfoDialog::queryDetail()
{
	ui->PButtonPublish->hide();

	NAManager *nam = new NAManager();
	connect(nam,SIGNAL(signal_reply_finished(QJsonObject,void*)),this,SLOT(slot_queryDetail(QJsonObject,void*)));
	QByteArray ba = QCryptographicHash::hash(QString("ciId=%1&page=1&rows=10000&userId=%2&key=RMS").arg(m_ciId).
											 arg(Global::g_LoginerId).toUtf8(),QCryptographicHash::Md5);
	QString md5(ba.toHex().toUpper());
	QJsonObject json;
	json.insert("ciId", QString::number(m_ciId));
	json.insert("page", QString::number(1));
	json.insert("rows", QString::number(10000));
	json.insert("userId", QString::number(Global::g_LoginerId));
	json.insert("sign", md5);
	QString strParam = QString("service=course&method=queryListLeavemsg&params=%1").arg(QString(QJsonDocument(json).
																						  toJson(QJsonDocument::Compact)));
	nam->SendRequest(QString("http://%1/rms/service/sys/execute").arg(Global::g_ServerIp),strParam);
}

void CourseInfoDialog::slot_queryDetail(QJsonObject obj, void *)
{
	if(ui->PButtonPublish->isHidden())
		ui->PButtonPublish->show();
	if(obj.isEmpty())
	{
		Global::g_NoticeDialog->init("网络异常，获取课程详情失败！","","",2000);
		Global::g_NoticeDialog->exec();
		return ;
	}
	if (obj["msg"].toString() != "")
	{
		Global::g_NoticeDialog->init(obj["msg"].toString(),"","",2000);
		Global::g_NoticeDialog->exec();
		return ;
	}
	while(ui->VLayoutMessage->isEmpty() == false)
	{
		MessageWidget *mw = (MessageWidget *)ui->VLayoutMessage->itemAt(0)->widget();
		ui->VLayoutMessage->removeWidget(mw);
		delete mw;
		mw = nullptr;
	}

	QJsonArray leaveMsg = obj["rows"].toArray();
	ui->LabelMessageCount->setText(QString("留言(%1)").arg(leaveMsg.count()));
	for(int i = 0;i < leaveMsg.count();i++)
	{
		QJsonObject jo = leaveMsg.at(i).toObject();
		MessageWidget *mw = new MessageWidget(jo,this);
		connect(mw,SIGNAL(signal_MessageReplySuccess()),this,SLOT(slot_MessageReplySuccess()));
		connect(mw,SIGNAL(signal_MessagePraiseSuccess()),this,SLOT(slot_MessagePraiseSuccess()));
		ui->VLayoutMessage->addWidget(mw);
	}
	ui->SAWContentsMessage->setFixedHeight(ui->VLayoutMessage->count()*170);
	ui->scrollArea_2->verticalScrollBar()->setSliderPosition(0);
}

void CourseInfoDialog::slot_PButtonAbout_clicked(bool IsChecked)
{
	m_LookWidget->hide();
	if(IsChecked)
		ui->stackedWidget->setCurrentWidget(ui->PageAbout);
}

void CourseInfoDialog::slot_PButtonCourse_clicked(bool IsChecked)
{
	if(IsChecked)
		ui->stackedWidget->setCurrentWidget(ui->PageCourse);
}

void CourseInfoDialog::slot_cow_EnterClassroom(CoursewareOutWidget *cow)
{
	setEnabled(false);
	NAManager *nam = new NAManager();
	connect(nam,SIGNAL(signal_reply_finished(QJsonObject,void*)),this,SLOT(slot_joinClass(QJsonObject,void*)));
	QByteArray ba = QCryptographicHash::hash(QString("classId=%1&uiId=%2&key=RMS").arg(cow->m_RoomId).arg(Global::g_LoginerId).
											 toUtf8(),QCryptographicHash::Md5);
	QString md5(ba.toHex().toUpper());
	QJsonObject json;
	json.insert("classId", QString::number(cow->m_RoomId));
	json.insert("uiId", QString::number(Global::g_LoginerId));
	json.insert("sign", md5);
	QString strParam = QString("service=classroom&method=joinClass&params=%1").arg(QString(QJsonDocument(json).
																						   toJson(QJsonDocument::Compact)));
	nam->SendRequest(QString("http://%1/rms/service/sys/execute").arg(Global::g_ServerIp),strParam,Post,cow);
}

void CourseInfoDialog::slot_joinClass(QJsonObject obj, void *da)
{
	setEnabled(true);
	if(obj.isEmpty())
	{
		Global::g_NoticeDialog->init("网络异常，进入教室失败！","","",2000);
		Global::g_NoticeDialog->exec();
		return ;
	}
	if (obj["msg"].toString() != "")
	{
		Global::g_NoticeDialog->init(obj["msg"].toString(),"","",2000);
		Global::g_NoticeDialog->exec();
		return ;
	}

	QJsonObject body = obj["body"].toObject();
	QStringList strList = body["document_server"].toString().split(":");
	Global::g_strDCS_IP = strList.at(0);
	Global::g_iDCS_port = strList.at(1).toInt();
	strList = body["file_server"].toString().split(":");
	Global::g_strFS_IP = strList.at(0);
	Global::g_iFS_port = strList.at(1).toInt();
	strList = body["chat_server"].toString().split(":");
	QString ChatMsgServerIP = strList.at(0);
	int ChatMsgServerPort = strList.at(1).toInt();
	QJsonObject roomInfo = body["roomInfo"].toObject();
	strList = body["video_server"].toString().split(":");
	QString dstip = strList.at(0);
	int dstport = strList.at(1).toInt();
	CoursewareOutWidget *cow = (CoursewareOutWidget *)da;
	ClassroomDialog *cd = new ClassroomDialog(roomInfo["id"].toInt(),body["mcu_ip_address"].toString(),
			body["port"].toString().toInt(),ChatMsgServerIP,ChatMsgServerPort,cow->m_cpiName,dstip,dstport,
			roomInfo["ciId"].toInt(),cow->m_crcpId,cow->DTimeClassBegin,cow->DTimeClassOver);
	connect(this,SIGNAL(signal_TeacherDialog_close()),cd,SLOT(slot_TeacherDialog_close()));
	cd->exec();
	delete cd;
	cd = nullptr;
}

void CourseInfoDialog::slot_PButtonMessage_clicked(bool IsChecked)
{
	m_LookWidget->hide();
	if(IsChecked)
		ui->stackedWidget->setCurrentWidget(ui->PageMessage);
}

void CourseInfoDialog::slot_PButtonPublish_clicked()
{
	if(ui->TEditMessage->toPlainText().isEmpty())
		return ;
	if(ui->TEditMessage->toPlainText().length() > 200)
	{
		Global::g_NoticeDialog->init("留言过长！","","",2000);
		Global::g_NoticeDialog->exec();
		return ;
	}
	ui->PButtonPublish->hide();

	NAManager *nr = new NAManager();
	connect(nr,SIGNAL(signal_reply_finished(QJsonObject,void*)),this,SLOT(slot_PButtonPublish_reply(QJsonObject,void*)));
	QByteArray ba = QCryptographicHash::hash(QString("content=%1&criId=%2&type=1&uiId=%3&userType=2&key=RMS").
											 arg(ui->TEditMessage->toPlainText()).arg(m_ciId).arg(Global::g_LoginerId).toUtf8(),
											 QCryptographicHash::Md5);
	QString md5(ba.toHex().toUpper());
	QJsonObject json;
	json.insert("content", ui->TEditMessage->toPlainText());
	json.insert("criId", QString::number(m_ciId));//后端接口字段（课程ID）命名有误
	json.insert("type", QString::number(1));//1-发布留言
	json.insert("uiId", QString::number(Global::g_LoginerId));
	json.insert("userType", QString::number(2));//2-老师
	json.insert("sign", md5);
	QString strParam = QString("service=course&method=saveOrUploadLeavemsg&params=%1").
			arg(QString(QJsonDocument(json).toJson(QJsonDocument::Compact)));
	nr->SendRequest(QString("http://%1/rms/service/sys/execute").arg(Global::g_ServerIp),strParam);
}

void CourseInfoDialog::slot_PButtonPublish_reply(QJsonObject obj, void *)
{
	if(ui->PButtonPublish->isHidden())
		ui->PButtonPublish->show();
	if(obj.isEmpty())
	{
		Global::g_NoticeDialog->init("网络异常，发布留言失败！","","",2000);
		Global::g_NoticeDialog->exec();
		return ;
	}
	if (obj["msg"].toString() != "")
	{
		Global::g_NoticeDialog->init(obj["msg"].toString(),"","",2000);
		Global::g_NoticeDialog->exec();
		return ;
	}

	ui->TEditMessage->setText("");
	queryDetail();
}

void CourseInfoDialog::slot_MessageReplySuccess()
{
	queryDetail();
}

void CourseInfoDialog::slot_MessagePraiseSuccess()
{
	queryDetail();
}

void CourseInfoDialog::slot_cow_OpenRecordPlay(CoursewareOutWidget *cow)
{
	NAManager *nam = new NAManager();
	connect(nam,SIGNAL(signal_reply_finished(QJsonObject,void*)),this,SLOT(slot_RecordPlay(QJsonObject,void*)));
	QByteArray ba = QCryptographicHash::hash(QString("crcpId=%1&criId=%2&key=RMS").arg(cow->m_crcpId).arg(cow->m_RoomId).
											 toUtf8(),QCryptographicHash::Md5);
	QString md5(ba.toHex().toUpper());
	QJsonObject json;
	json.insert("crcpId", QString::number(cow->m_crcpId));
	json.insert("criId", QString::number(cow->m_RoomId));
	json.insert("sign", md5);
	QString strParam = QString("service=classroom&method=queryVideoList&params=%1").arg(QString(QJsonDocument(json).
																						   toJson(QJsonDocument::Compact)));
	nam->SendRequest(QString("http://%1/rms/service/sys/execute").arg(Global::g_ServerIp),strParam,Post,cow);
}

void CourseInfoDialog::slot_RecordPlay(QJsonObject obj, void *da)
{
	if(obj.isEmpty())
	{
		Global::g_NoticeDialog->init("网络异常，查询班级视频列表失败！","","",2000);
		Global::g_NoticeDialog->exec();
		return ;
	}
	if (obj["msg"].toString() != "")
	{
		Global::g_NoticeDialog->init(obj["msg"].toString(),"","",2000);
		Global::g_NoticeDialog->exec();
		return ;
	}

	QJsonObject body = obj["body"].toObject();
	QJsonArray rows = body["rows"].toArray();
	CoursewareOutWidget *cow = (CoursewareOutWidget *)da;
	RecordPlayDialog *rpd = new RecordPlayDialog(rows,cow->m_crcpId,cow->m_RoomId);
	connect(this,SIGNAL(signal_TeacherDialog_close()),rpd,SLOT(reject()));
	rpd->exec();
	delete rpd;
	rpd = nullptr;
}

void CourseInfoDialog::slot_SeeCurriculumSchedule()
{
	m_LookWidget->move((width()-m_LookWidget->width())/2,(height()-m_LookWidget->height())/2);
	m_LookWidget->show();
}

void CourseInfoDialog::mousePressEvent(QMouseEvent *)
{
	m_LookWidget->hide();
}

void CourseInfoDialog::slot_TeacherDialog_close()
{
	emit signal_TeacherDialog_close();
	slot_PButtonClose_clicked();
}

void CourseInfoDialog::slot_init()
{
	ui->PButtonClose->move(ui->WidgetTop->width()-30,0);
	connect(ui->PButtonClose,SIGNAL(clicked(bool)),this,SLOT(slot_PButtonClose_clicked()));

	QString HtmlImagePath = Global::g_AppDataPath + "html_image_temp/";
	QDir dir(HtmlImagePath);
	if(dir.exists() == false)
		dir.mkpath(HtmlImagePath);

	ui->MCWidgetCourse->setStatus(m_obj);
	ui->MCWidgetCourse->SetObj(m_obj);
	ui->MCWidgetCourse->ClearWidgetMain();
	QJsonObject courseInfo = m_obj["courseInfo"].toObject();
	QJsonArray cpList = courseInfo["cpList"].toArray();
	QString str_des = "\n";
	for(int i = 0;i < cpList.count();i++)
	{
		QJsonObject jo = cpList.at(i).toObject();
		str_des = str_des + " " + jo["date"].toString() + " " + jo["name"].toString();
		if(i < cpList.count() - 1)
			str_des = str_des + "\n\n";
	}
	m_LookWidget->setLookText(str_des);
	m_LookWidget->hide();
	QJsonArray courseplanList = m_obj["courseplanList"].toArray();
	str_des.replace("\n","    ");
	CoursewareOutWidget *cow_see = new CoursewareOutWidget(this);
	if(courseplanList.count() > 0)
		cow_see->Init_see(str_des,1);
	else
		cow_see->Init_see(str_des,3);
	connect(cow_see,SIGNAL(signal_SeeCurriculumSchedule()),this,SLOT(slot_SeeCurriculumSchedule()));
	ui->VLayoutCourse->addWidget(cow_see);
	for(int i = 0;i < courseplanList.count();i++)
	{
		QJsonObject jo = courseplanList.at(i).toObject();
		int LineMark = 0;
		if(i == courseplanList.count() - 1)
			LineMark = 2;
		CoursewareOutWidget *cow = new CoursewareOutWidget(this);
		cow->Init_enter_reply(jo,LineMark);
		connect(cow,SIGNAL(signal_EnterClassroom(CoursewareOutWidget*)),this,SLOT(slot_cow_EnterClassroom(CoursewareOutWidget*)));
		connect(cow,SIGNAL(signal_OpenRecordPlay(CoursewareOutWidget*)),this,SLOT(slot_cow_OpenRecordPlay(CoursewareOutWidget*)));
		ui->VLayoutCourse->addWidget(cow);
	}
	ui->SAWContentsCourse->setFixedHeight(ui->VLayoutCourse->count()*70);
	QString string_html = courseInfo["detailDesc"].toString();
	if(string_html.contains("<img"))
	{
		string_html.replace("<img","*gxy830*<img");
		QStringList string_list = string_html.split("*gxy830*");
		int img_inx = 0;
		for(int i = 0; i < string_list.count();i++)
		{
			QString string_list_i = string_list.at(i);
			if(string_list_i.contains("<img"))
			{
				int index_src = string_list_i.indexOf("src=\"")+5;
				QString img_url = string_list_i.mid(index_src,string_list_i.indexOf("\"",index_src)-index_src);
				img_inx++;
				QString img_path = HtmlImagePath + QString("%1.png").arg(img_inx);
				UrlImg *ulig = new UrlImg(img_url,img_path);
				MapUrlImg.insert(img_url,ulig);
				NAManager *nr = new NAManager();
				connect(nr,SIGNAL(signal_reply_finished(QPixmap,void*)),this,SLOT(slot_GetHtmlImage(QPixmap,void*)));
				nr->SendRequest(img_url,QString(),Get,ulig);
				QString string_list_i_right = string_list_i.right(string_list_i.length()-1-string_list_i.indexOf(">"));
				string_list_i = "<img src=\"" + img_path + "\">" + string_list_i_right;
			}
			m_string_html = m_string_html + string_list_i;
		}
	}
	else
		ui->TEditAbout->setHtml(string_html);
	m_ciId = courseInfo["id"].toInt();
	queryDetail();
}

void CourseInfoDialog::slot_PButtonClose_clicked()
{
	reject();
}

void CourseInfoDialog::closeEvent(QCloseEvent *event)
{
	event->ignore();
	reject();
}

void CourseInfoDialog::slot_TEditMessage_textChanged()
{
	QByteArray ba = ui->TEditMessage->toPlainText().toUtf8();
	QByteArray ba_temp;
	ba_temp.append('\xEF');
	ba_temp.append('\xBF');
	ba_temp.append('\xBC');
	if(ba.contains(ba_temp))
	{
		ui->TEditMessage->clear();
		Global::g_NoticeDialog->init("不支持表情输入！","","",2000);
		Global::g_NoticeDialog->exec();
	}
}
