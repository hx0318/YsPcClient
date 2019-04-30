#include "teacherdialog.h"
#include "ui_teacherdialog.h"
#include <QKeyEvent>
#include "global.h"
#include "classroomdialog.h"
#include "namanager.h"
#include <QDesktopWidget>
#include <QHostInfo>
#pragma execution_character_set("utf-8")

TeacherDialog::TeacherDialog(QString ticketId,QString chat_server,QWidget *parent) :
	QDialog(parent),
	ui(new Ui::TeacherDialog)
{
	ui->setupUi(this);

	setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint);
	setAttribute(Qt::WA_DeleteOnClose);
	setWindowTitle("首页");
	resize(QApplication::desktop()->availableGeometry().size());
	ui->CBoxCourseType->addItem("全部");
	ui->CBoxCourseType->addItem("正在更新");
	ui->CBoxCourseType->addItem("未开始");
	ui->CBoxCourseType->addItem("已完结");
	connect(ui->CBoxCourseType,SIGNAL(activated(int)),this,SLOT(slot_CBoxCourseType_activated(int)));
	ui->PWidgetCoursePage->InitButtons(0);

	connect(ui->PButtonLogout,SIGNAL(clicked(bool)),this,SLOT(slot_PButtonLogout_clicked()));
	connect(ui->GCWidgetDate,SIGNAL(signal_DateSelected(QDate)),this,SLOT(slot_GCWidgetDate_DateSelected(QDate)));
	connect(ui->RButtonCreateCourse,SIGNAL(clicked(bool)),this,SLOT(slot_RButtonCreateCourse_clicked()));
	connect(ui->RButtonUploadCourseware,SIGNAL(clicked(bool)),this,SLOT(slot_RButtonUploadCourseware_clicked()));
	connect(ui->RButtonCourseSignUpRecord,SIGNAL(clicked(bool)),this,SLOT(slot_RButtonCourseSignUpRecord_clicked()));
	connect(ui->PWidgetCoursePage,SIGNAL(signal_SendPageMark(uint)),this,SLOT(slot_PWidgetCoursePageSendPageMark(uint)));

	m_ticketId = ticketId;
	MyCoursePageIndex = 0;
	chat_server_ip = chat_server.split(":").at(0);
	chat_server_port = chat_server.split(":").at(1).toShort();
	m_onelogin = k12msg::k12msgs::create(&oneLogin,Global::g_LoginerId,100000,chat_server_ip.toLatin1().data(), chat_server_port);
	connect(&oneLogin,SIGNAL(signal_SelfLogin(bool)),this,SLOT(selflogin(bool)));
	connect(&oneLogin,SIGNAL(signal_disconnect_from_oneLogin()),this,SLOT(disconnect_from_oneLogin()));
	connect(&oneLogin, SIGNAL(_connect2msgs_state(int)), this, SLOT(__connect2msgs_state(int)));
	ListCourse.clear();

	Global::g_ImagePath = Global::g_AppDataPath + "image_temp";
	QDir dir(Global::g_ImagePath);
	if(dir.exists() == false)
		dir.mkpath(Global::g_ImagePath);
	Global::g_LocalIp = "";

	QTimer::singleShot(0, this, SLOT(slot_init_1()));
}

TeacherDialog::~TeacherDialog()
{
	Global::DelDir(Global::g_ImagePath);
	ListCourse.clear();
	delete m_onelogin;
	m_onelogin = nullptr;

	delete ui;
}

void TeacherDialog::closeEvent(QCloseEvent *event)
{
	Global::g_LoginDialog->show();
	event->accept();
}

void TeacherDialog::keyPressEvent(QKeyEvent *event)
{
	switch (event->key())
	{
	case Qt::Key_Escape:
		break;
	default:
		QDialog::keyPressEvent(event);
	}
}

void TeacherDialog::slot_PButtonLogout_clicked()
{
	close();
}

void TeacherDialog::slot_init_1()
{
	ui->GCWidgetDate->setFixedWidth((ui->WidgetCourse->width()-30)/2);
	ui->WidgetDateCourse->setFixedWidth((ui->WidgetCourse->width()-30)/2);
	ui->GCWidgetDate->setFixedHeight(ui->GCWidgetDate->width()*300/530);
	ui->WidgetDateCourse->setFixedHeight(ui->WidgetDateCourse->width()*300/530);
	ui->SAWContentsDateCourse->setFixedWidth(ui->SAreaDateCourse->width()-20);

	QTimer::singleShot(0, this, SLOT(slot_init_2()));
}

void TeacherDialog::slot_init_2()
{
	ui->SAWContentsMyCourse->setFixedWidth(ui->SAreaMyCourse->width());
	MyCoursePageIndex = ui->SAWContentsMyCourse->height()/120;

	queryMyCourseByDate(QDate::currentDate());
	ui->GCWidgetDate->CheckCurrentDate();

	NAManager *nam = new NAManager();
	connect(nam,SIGNAL(signal_reply_finished(QJsonObject,void*)),this,SLOT(slot_GetNameConcem_reply(QJsonObject,void*)));
	QByteArray ba = QCryptographicHash::hash(QString("tiId=%1&key=AAS").arg(Global::g_LoginerId).toUtf8(),
											 QCryptographicHash::Md5);
	QString md5(ba.toHex().toUpper());
	QJsonObject json;
	json.insert("tiId", QString::number(Global::g_LoginerId));
	json.insert("sign", md5);
	QString strParam = QString("service=teacher&method=queryDetail&params=%1").arg(QString(QJsonDocument(json).
																					 toJson(QJsonDocument::Compact)));
	nam->SendRequest(QString("http://%1/aas/service/sys/execute").arg(Global::g_ServerIp),strParam);

	ui->CBoxCourseType->setCurrentIndex(0);
	slot_CBoxCourseType_activated(0);
}

void TeacherDialog::queryMyCourseByDate(QDate de)
{
	while(ui->VLayoutDateCourse->isEmpty() == false)
	{
		CourseWidget *cw = (CourseWidget *)ui->VLayoutDateCourse->itemAt(0)->widget();
		ui->VLayoutDateCourse->removeWidget(cw);
		delete cw;
		cw = nullptr;
	}
	ui->SAWContentsDateCourse->setFixedHeight(ui->VLayoutDateCourse->count()*70);

	ui->LabelDate->setText(QString("%1月%2日课程").arg(de.month()).arg(de.day()));
	NAManager *nam = new NAManager();
	connect(nam,SIGNAL(signal_reply_finished(QJsonObject,void*)),this,SLOT(slot_queryMyCourseByDate_reply(QJsonObject,void*)));
	QString str_de = de.toString("yyyy-MM-dd");
	QByteArray ba = QCryptographicHash::hash(QString("dateTime=%1&flag=2&uiId=%2&key=RMS").arg(str_de).arg(Global::g_LoginerId).
											 toUtf8(), QCryptographicHash::Md5);
	QString md5(ba.toHex().toUpper());
	QJsonObject json;
	json.insert("dateTime", str_de);
	json.insert("flag", QString::number(2));
	json.insert("uiId", QString::number(Global::g_LoginerId));
	json.insert("sign", md5);
	QString strParam = QString("service=course&method=queryMyCourseByDate&params=%1").arg(QString(QJsonDocument(json).
																					 toJson(QJsonDocument::Compact)));
	nam->SendRequest(QString("http://%1/rms/service/sys/execute").arg(Global::g_ServerIp),strParam);
}

void TeacherDialog::slot_queryMyCourseByDate_reply(QJsonObject obj, void *)
{
	ui->GCWidgetDate->setEnabled(true);

	if(obj.isEmpty())
	{
		Global::g_NoticeDialog->init("网络异常，获取选定日期课程失败！","","",2000);
		Global::g_NoticeDialog->exec();
		return ;
	}
	if (obj["msg"].toString() != "")
		return ;

	QJsonArray body = obj["body"].toArray();
	for (int i=0; i<body.count(); i++)
	{
		QJsonObject jo = body.at(i).toObject();
		QJsonObject job = jo["classroomCoursePlan"].toObject();
		int LineMark = 0;
		if(1 == body.count())
			LineMark = 3;
		else if(i == 0)
			LineMark = 1;
		else if(i == body.count() - 1)
			LineMark = 2;
		CourseWidget *cw = new CourseWidget(job, LineMark, this);
		connect(cw,SIGNAL(signal_EnterClassroom(CourseWidget*)),this,SLOT(slot_cw_EnterClassroom(CourseWidget*)));
		ui->VLayoutDateCourse->addWidget(cw);
	}
	ui->SAWContentsDateCourse->setFixedHeight(ui->VLayoutDateCourse->count()*70);
}

void TeacherDialog::slot_cw_EnterClassroom(CourseWidget *cw)
{
	setEnabled(false);
	NAManager *nam = new NAManager();
	connect(nam,SIGNAL(signal_reply_finished(QJsonObject,void*)),this,SLOT(slot_joinClass(QJsonObject,void*)));
	QByteArray ba = QCryptographicHash::hash(QString("classId=%1&uiId=%2&key=RMS").arg(cw->m_RoomId).arg(Global::g_LoginerId).
											 toUtf8(),QCryptographicHash::Md5);
	QString md5(ba.toHex().toUpper());
	QJsonObject json;
	json.insert("classId", QString::number(cw->m_RoomId));
	json.insert("uiId", QString::number(Global::g_LoginerId));
	json.insert("sign", md5);
	QString strParam = QString("service=classroom&method=joinClass&params=%1").arg(QString(QJsonDocument(json).
																						   toJson(QJsonDocument::Compact)));
	nam->SendRequest(QString("http://%1/rms/service/sys/execute").arg(Global::g_ServerIp),strParam,Post,cw);
}

void TeacherDialog::slot_joinClass(QJsonObject obj, void *da)
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
	CourseWidget *cw = (CourseWidget *)da;
	ClassroomDialog *cd = new ClassroomDialog(roomInfo["id"].toInt(),body["mcu_ip_address"].toString(),
			body["port"].toString().toInt(),ChatMsgServerIP,ChatMsgServerPort,cw->m_cpiName,dstip,dstport,
			roomInfo["ciId"].toInt(),cw->m_crcpId,cw->DTimeClassBegin,cw->DTimeClassOver);
	connect(this,SIGNAL(signal_TeacherDialog_close()),cd,SLOT(slot_TeacherDialog_close()));
	hide();
	cd->exec();
	show();
	delete cd;
	cd = nullptr;
}

void TeacherDialog::slot_GCWidgetDate_DateSelected(QDate date)
{
	ui->GCWidgetDate->setEnabled(false);
	queryMyCourseByDate(date);
}

void TeacherDialog::slot_GetNameConcem_reply(QJsonObject obj, void *)
{
	if(obj.isEmpty())
	{
		Global::g_NoticeDialog->init("网络异常，获取昵称和关注失败！","","",2000);
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
	QString headimg = body["headimg"].toString();
	if(headimg.contains("/") == false)
		headimg = "/" + headimg;
	m_ImagePath = Global::g_ImagePath + headimg;
	QFile FileImage(m_ImagePath);
	if(FileImage.exists())
		slot_GetHeadimg(QPixmap(m_ImagePath),nullptr);
	else
	{
		NAManager *nam = new NAManager();
		connect(nam,SIGNAL(signal_reply_finished(QPixmap,void*)),this,SLOT(slot_GetHeadimg(QPixmap,void*)));
		nam->SendRequest(QString("http://%1/uds/upload%2").arg(Global::g_ServerIp).arg(headimg),QString(),Get,nullptr);
	}
	ui->LabelTeacherName->setText(body["name"].toString());
	ui->LabelConcemCount->setText(QString::number(body["tiWatchNumbers"].toInt()));
	ui->TEditIntroduce->setText(body["teacherShort"].toString());
}

void TeacherDialog::slot_GetHeadimg(QPixmap pix, void *)
{
	if(pix.isNull())
	{
		qDebug() << "TeacherDialog::slot_GetHeadimg:pix.isNull()";
		return ;
	}

	QDir dir(m_ImagePath.left(m_ImagePath.lastIndexOf("/")));
	if(dir.exists() == false)
		dir.mkpath(m_ImagePath.left(m_ImagePath.lastIndexOf("/")));
	pix.scaled(256,256,Qt::IgnoreAspectRatio,Qt::SmoothTransformation).save(m_ImagePath);

	ui->PWidgetHead->SetBackground(pix);
}

void TeacherDialog::slot_RButtonCreateCourse_clicked()
{
	QString str_url = QString("http://%1/cooBook/index.html?tid=%2&page=creatCourse&timestamp=%3&user=%4").arg(Global::g_ServerIp).
			arg(Global::g_LoginerId).arg(QDateTime::currentDateTime().toTime_t()).arg(Global::g_LoginerName);
	LPCWSTR strValue = reinterpret_cast<LPCWSTR>(str_url.replace("/","\\").unicode());
	ShellExecute(NULL,TEXT("open"), strValue,NULL, NULL, SW_SHOWNORMAL);
}

void TeacherDialog::slot_RButtonUploadCourseware_clicked()
{
	QString str_url = QString("http://%1/cooBook/index.html?tid=%2&page=postCourse&timestamp=%3&user=%4").arg(Global::g_ServerIp).
			arg(Global::g_LoginerId).arg(QDateTime::currentDateTime().toTime_t()).arg(Global::g_LoginerName);
	LPCWSTR strValue = reinterpret_cast<LPCWSTR>(str_url.replace("/","\\").unicode());
	ShellExecute(NULL,TEXT("open"), strValue,NULL, NULL, SW_SHOWNORMAL);
}

void TeacherDialog::slot_RButtonCourseSignUpRecord_clicked()
{
	QString str_url = QString("http://%1/cooBook/index.html?tid=%2&page=buyCourse&timestamp=%3&user=%4").arg(Global::g_ServerIp).
			arg(Global::g_LoginerId).arg(QDateTime::currentDateTime().toTime_t()).arg(Global::g_LoginerName);
	LPCWSTR strValue = reinterpret_cast<LPCWSTR>(str_url.replace("/","\\").unicode());
	ShellExecute(NULL,TEXT("open"), strValue,NULL, NULL, SW_SHOWNORMAL);
}

void TeacherDialog::GetCourseList()
{
	ListCourse.clear();

	NAManager *nam = new NAManager();
	connect(nam,SIGNAL(signal_reply_finished(QJsonObject,void*)),this,SLOT(slot_GetCourseList_reply(QJsonObject,void*)));
	QByteArray ba = QCryptographicHash::hash(QString("page=1&rows=10000&tiId=%1&key=MBS").arg(Global::g_LoginerId).toUtf8(),
											 QCryptographicHash::Md5);
	QString md5(ba.toHex().toUpper());
	QJsonObject json;
	json.insert("page", QString::number(1));
	json.insert("rows", QString::number(10000));
	json.insert("tiId", QString::number(Global::g_LoginerId));
	json.insert("sign", md5);
	QString strParam = QString("service=orders&method=queryTeacherCourseList&params=%1").
			arg(QString(QJsonDocument(json).toJson(QJsonDocument::Compact)));
	nam->SendRequest(QString("http://%1/mbs/service/sys/execute").arg(Global::g_ServerIp),strParam);
}

void TeacherDialog::slot_GetCourseList_reply(QJsonObject obj, void *)
{
	ui->CBoxCourseType->setEnabled(true);
	if(obj.isEmpty())
	{
		Global::g_NoticeDialog->init("网络异常，获取我的课程失败！","","",2000);
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
	for (int i=0; i<rows.count(); i++)
	{
		ListCourse.append(rows.at(i).toObject());
	}

	int PageCount = ListCourse.count() / MyCoursePageIndex;
	if(ListCourse.count() % MyCoursePageIndex != 0)
		PageCount = PageCount + 1;
	ui->PWidgetCoursePage->InitButtons(PageCount);
}

void TeacherDialog::slot_PWidgetCoursePageSendPageMark(uint PageMark)
{
	PageOfListCourse(PageMark);
}

void TeacherDialog::slot_OpenCourseInfo(QJsonObject obj)
{
	CourseInfoDialog *cid = new CourseInfoDialog(obj);
	connect(this,SIGNAL(signal_TeacherDialog_close()),cid,SLOT(slot_TeacherDialog_close()));
	cid->setGeometry(rect());
	hide();
	cid->exec();
	delete cid;
	cid = nullptr;
	show();
}

void TeacherDialog::selflogin(bool IfSelf)
{
	if(IfSelf)
	{
		emit signal_TeacherDialog_close();
		close();
		Global::g_LoginDialog->show();
		Global::g_NoticeDialog->init("账号在其他地方登录！","","",1800000);
		Global::g_NoticeDialog->exec();
	}
	else
	{
		NAManager *nam = new NAManager();
		connect(nam,SIGNAL(signal_reply_finished(QJsonObject,void*)),this,SLOT(slot_Onelogin_reply(QJsonObject,void*)));
		QByteArray ba = QCryptographicHash::hash(QString("ticketId=%1&type=1&key=AAS").arg(m_ticketId).toUtf8(),
												 QCryptographicHash::Md5);
		QString md5(ba.toHex().toUpper());
		QJsonObject json;
		json.insert("ticketId", m_ticketId);
		json.insert("type", "1");
		json.insert("sign", md5);
		QString strParam = QString("service=teacher&method=login&params=%1").arg(QString(QJsonDocument(json).
																						 toJson(QJsonDocument::Compact)));
		nam->SendRequest(QString("http://%1/aas/service/sys/execute").arg(Global::g_ServerIp),strParam);
	}
}

void TeacherDialog::slot_Onelogin_reply(QJsonObject obj, void *)
{
	if(obj.isEmpty())
	{
		Global::g_NoticeDialog->init("网络异常，单点登录验证失败！","","",2000);
		Global::g_NoticeDialog->exec();
		return ;
	}
	if (obj["msg"].toString() != "")
	{
		selflogin(true);
		return ;
	}
	if(Global::g_LocalIp != Global::getLocalIp())
	{
		emit signal_TeacherDialog_close();
		close();
		Global::g_LoginDialog->show();
		Global::g_NoticeDialog->init("IP地址发生变更，请重新登录！","","",1800000);
		Global::g_NoticeDialog->exec();
		return ;
	}

	QJsonObject body = obj["body"].toObject();
	m_ticketId = body["ticketId"].toString();
}

void TeacherDialog::disconnect_from_oneLogin()//与聊天服务器断开连接
{
	delete m_onelogin;
	m_onelogin = nullptr;
	m_onelogin = k12msg::k12msgs::create(&oneLogin,Global::g_LoginerId,100000,chat_server_ip.toLatin1().data(),chat_server_port);
}

void TeacherDialog::__connect2msgs_state(int state)
{
	if(state == 1)//连接聊天服务器成功，则发送单点登录消息
	{
		QJsonObject json;
		json.insert("typeString", "dddl");
		json.insert("Uuid", Global::g_Uuid);
		QString strMsgContent = QString(QJsonDocument(json).toJson()).replace("\n","").replace(" ", "");
		m_onelogin->msgsend(strMsgContent.toUtf8().data(),QString::number((int)Global::g_LoginerId).toLatin1().data());
		if(Global::g_LocalIp == "")
			Global::g_LocalIp = Global::getLocalIp();
	}
	else//state == 0，未能连接上
		disconnect_from_oneLogin();
}

void TeacherDialog::PageOfListCourse(int PageMark)
{
	while(ui->VLayoutMyCourse->isEmpty() == false)
	{
		MyCourseWidget *mcw = (MyCourseWidget *)ui->VLayoutMyCourse->itemAt(0)->widget();
		ui->VLayoutMyCourse->removeWidget(mcw);
		delete mcw;
		mcw = nullptr;
	}

	switch(ui->CBoxCourseType->currentIndex())
	{
	case 0://全部
		for(int i = 0;i < ListCourse.count();i++)
		{
			if(i >= (PageMark-1)*MyCoursePageIndex && i <= PageMark*MyCoursePageIndex-1)
			{
				MyCourseWidget *mcw = new MyCourseWidget(this);
				mcw->setStatus(ListCourse.at(i));
				mcw->SetObj(ListCourse.at(i));
				connect(mcw,SIGNAL(signal_OpenCourseInfo(QJsonObject)),this,SLOT(slot_OpenCourseInfo(QJsonObject)));
				ui->VLayoutMyCourse->addWidget(mcw);
			}
		}
		break;
	case 1://正在更新
	{
		int j = 0;
		for(int i = 0;i < ListCourse.count();i++)
		{
			MyCourseWidget *mcw = new MyCourseWidget(this);
			mcw->setStatus(ListCourse.at(i));
			if(mcw->m_status == "正在更新")
			{
				if(j >= (PageMark-1)*MyCoursePageIndex && j <= PageMark*MyCoursePageIndex-1)
				{
					mcw->SetObj(ListCourse.at(i));
					connect(mcw,SIGNAL(signal_OpenCourseInfo(QJsonObject)),this,SLOT(slot_OpenCourseInfo(QJsonObject)));
					ui->VLayoutMyCourse->addWidget(mcw);
				}
				else
				{
					delete mcw;
					mcw = nullptr;
				}
				j++;
			}
		}
	}
		break;
	case 2://未开始
	{
		int j = 0;
		for(int i = 0;i < ListCourse.count();i++)
		{
			MyCourseWidget *mcw = new MyCourseWidget(this);
			mcw->setStatus(ListCourse.at(i));
			if(mcw->m_status == "未开始")
			{
				if(j >= (PageMark-1)*MyCoursePageIndex && j <= PageMark*MyCoursePageIndex-1)
				{
					mcw->SetObj(ListCourse.at(i));
					connect(mcw,SIGNAL(signal_OpenCourseInfo(QJsonObject)),this,SLOT(slot_OpenCourseInfo(QJsonObject)));
					ui->VLayoutMyCourse->addWidget(mcw);
				}
				else
				{
					delete mcw;
					mcw = nullptr;
				}
				j++;
			}
		}
	}
		break;
	case 3://已完结
	{
		int j = 0;
		for(int i = 0;i < ListCourse.count();i++)
		{
			MyCourseWidget *mcw = new MyCourseWidget(this);
			mcw->setStatus(ListCourse.at(i));
			if(mcw->m_status == "已完结")
			{
				if(j >= (PageMark-1)*MyCoursePageIndex && j <= PageMark*MyCoursePageIndex-1)
				{
					mcw->SetObj(ListCourse.at(i));
					connect(mcw,SIGNAL(signal_OpenCourseInfo(QJsonObject)),this,SLOT(slot_OpenCourseInfo(QJsonObject)));
					ui->VLayoutMyCourse->addWidget(mcw);
				}
				else
				{
					delete mcw;
					mcw = nullptr;
				}
				j++;
			}
		}
	}
		break;
	default:
		break;
	}
	ui->SAWContentsMyCourse->setFixedHeight(ui->VLayoutMyCourse->count()*120);
}

void TeacherDialog::slot_CBoxCourseType_activated(int inx)
{
	switch(inx)
	{
	case 0://全部
		ui->CBoxCourseType->setEnabled(false);
		GetCourseList();
		break;
	case 1://正在更新
	{
		int PageCountTemp = 0;
		for(int i = 0;i < ListCourse.count();i++)
		{
			MyCourseWidget *mcw = new MyCourseWidget(this);
			mcw->setStatus(ListCourse.at(i));
			if(mcw->m_status == "正在更新")
				PageCountTemp++;
			delete mcw;
			mcw = nullptr;
		}
		int PageCount = PageCountTemp / MyCoursePageIndex;
		if(PageCountTemp % MyCoursePageIndex != 0)
			PageCount = PageCount + 1;
		ui->PWidgetCoursePage->InitButtons(PageCount);
	}
		break;
	case 2://未开始
	{
		int PageCountTemp = 0;
		for(int i = 0;i < ListCourse.count();i++)
		{
			MyCourseWidget *mcw = new MyCourseWidget(this);
			mcw->setStatus(ListCourse.at(i));
			if(mcw->m_status == "未开始")
				PageCountTemp++;
			delete mcw;
			mcw = nullptr;
		}
		int PageCount = PageCountTemp / MyCoursePageIndex;
		if(PageCountTemp % MyCoursePageIndex != 0)
			PageCount = PageCount + 1;
		ui->PWidgetCoursePage->InitButtons(PageCount);
	}
		break;
	case 3://已完结
	{
		int PageCountTemp = 0;
		for(int i = 0;i < ListCourse.count();i++)
		{
			MyCourseWidget *mcw = new MyCourseWidget(this);
			mcw->setStatus(ListCourse.at(i));
			if(mcw->m_status == "已完结")
				PageCountTemp++;
			delete mcw;
			mcw = nullptr;
		}
		int PageCount = PageCountTemp / MyCoursePageIndex;
		if(PageCountTemp % MyCoursePageIndex != 0)
			PageCount = PageCount + 1;
		ui->PWidgetCoursePage->InitButtons(PageCount);
	}
		break;
	default:
		break;
	}
}
