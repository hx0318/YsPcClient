#include "classroomdialog.h"
#include "ui_classroomdialog.h"
#include <QKeyEvent>
#include <QDesktopWidget>
#include "namanager.h"
#include "global.h"
#include <QMovie>
#pragma comment(lib, "ole32.lib")
#pragma execution_character_set("utf-8")

static void record_msg_func(void *private_data, record_msg_t t, void *arg)
{
    ((ClassroomDialog *)private_data)->record_msg(t, arg);
}

ClassroomDialog::ClassroomDialog(int id,QString McuIp,int McuPort,QString ChatMsgServerIP,int ChatMsgServerPort,QString Name,QString dstip,int dstport,int CoursewareId,int crcpId,
                                 QDateTime ClassBegin,QDateTime ClassOver,QWidget *parent):
	QDialog(parent),
	ui(new Ui::ClassroomDialog)
{
	ui->setupUi(this);
	setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint);
	setWindowTitle("教室");
	resize(QApplication::desktop()->availableGeometry().size());
    qRegisterMetaType<user_media_t>("user_media_t");
	QButtonGroup *bg = new QButtonGroup(this);
	bg->addButton(ui->PButtonVip,0);
	bg->addButton(ui->PButtonAuditor,1);
    ui->SWidgetList->setCurrentWidget(ui->SWLPageStudent);
    ui->PButtonVip->setChecked(true);
    ui->SWidgetStudent->setCurrentWidget(ui->PageVip);
	ui->PButtonPrev->hide();
	ui->PButtonPlay->hide();
	ui->PButtonNext->hide();
    ui->LabelClassName->setText(Name);
	QButtonGroup *bg1 = new QButtonGroup(this);
	bg1->addButton(ui->PButtonDevice,0);
	bg1->addButton(ui->PButtonSystem,1);
    ui->PButtonDevice->setChecked(true);
    ui->SWidgetCheck->setCurrentWidget(ui->PageDevice);
	ui->STWidgetBarrage1->hide();
	ui->STWidgetBarrage2->hide();
    ui->STWidgetBarrage3->hide();
	ui->WidgetTool->setCursor(Qt::ArrowCursor);
	ui->SWidgetMedia->setCursor(Qt::ArrowCursor);
    ui->CBoxOutputAudio->installEventFilter(this);
	QButtonGroup *bg2 = new QButtonGroup(this);
	bg2->addButton(ui->PButtonFile,0);
	bg2->addButton(ui->PButtonScreenshotAlbum,1);
	bg2->addButton(ui->PButtonGiftList,2);
	bg2->addButton(ui->PButtonHistoricalNewsList,3);
	bg2->addButton(ui->PButtonStudentList,4);
	bg2->addButton(ui->PButtonDeviceDetection,5);
	ui->PButtonStudentList->setChecked(true);

	connect(this,SIGNAL(signal_K12MCU2UI_CONNECT2MCU_STATE(int)),this,SLOT(slot_K12MCU2UI_CONNECT2MCU_STATE(int)));
	connect(this,SIGNAL(signal_K12MCU2UI_DISCONNECT_FROM_MCU()),this,SLOT(slot_K12MCU2UI_DISCONNECT_FROM_MCU()));
    connect(this,SIGNAL(signal_start_class_ack(int)),this,SLOT(slot_start_class_ack(int)));
    connect(this,SIGNAL(signal_stop_class_ack(int)),this,SLOT(slot_stop_class_ack(int)));
    connect(ui->MWidgetStudent1,SIGNAL(signal_VideoOpen(int)),this,SLOT(slot_VideoOpen(int)));
    connect(ui->MWidgetStudent1,SIGNAL(signal_VideoClose(int)),this,SLOT(slot_VideoClose(int)));
    connect(ui->MWidgetStudent1,SIGNAL(signal_AudioOpen(int)),this,SLOT(slot_AudioOpen(int)));
    connect(ui->MWidgetStudent1,SIGNAL(signal_AudioClose(int)),this,SLOT(slot_AudioClose(int)));
    connect(ui->MWidgetStudent2,SIGNAL(signal_VideoOpen(int)),this,SLOT(slot_VideoOpen(int)));
    connect(ui->MWidgetStudent2,SIGNAL(signal_VideoClose(int)),this,SLOT(slot_VideoClose(int)));
    connect(ui->MWidgetStudent2,SIGNAL(signal_AudioOpen(int)),this,SLOT(slot_AudioOpen(int)));
    connect(ui->MWidgetStudent2,SIGNAL(signal_AudioClose(int)),this,SLOT(slot_AudioClose(int)));
    connect(ui->MWidgetStudent3,SIGNAL(signal_VideoOpen(int)),this,SLOT(slot_VideoOpen(int)));
    connect(ui->MWidgetStudent3,SIGNAL(signal_VideoClose(int)),this,SLOT(slot_VideoClose(int)));
    connect(ui->MWidgetStudent3,SIGNAL(signal_AudioOpen(int)),this,SLOT(slot_AudioOpen(int)));
    connect(ui->MWidgetStudent3,SIGNAL(signal_AudioClose(int)),this,SLOT(slot_AudioClose(int)));
    connect(ui->MWidgetStudent4,SIGNAL(signal_VideoOpen(int)),this,SLOT(slot_VideoOpen(int)));
    connect(ui->MWidgetStudent4,SIGNAL(signal_VideoClose(int)),this,SLOT(slot_VideoClose(int)));
    connect(ui->MWidgetStudent4,SIGNAL(signal_AudioOpen(int)),this,SLOT(slot_AudioOpen(int)));
    connect(ui->MWidgetStudent4,SIGNAL(signal_AudioClose(int)),this,SLOT(slot_AudioClose(int)));
    connect(ui->MWidgetStudent1,SIGNAL(signal_MediaUp(int)),this,SLOT(slot_MediaUp(int)));
    connect(ui->MWidgetStudent2,SIGNAL(signal_MediaUp(int)),this,SLOT(slot_MediaUp(int)));
    connect(ui->MWidgetStudent3,SIGNAL(signal_MediaUp(int)),this,SLOT(slot_MediaUp(int)));
    connect(ui->MWidgetStudent4,SIGNAL(signal_MediaUp(int)),this,SLOT(slot_MediaUp(int)));
    connect(this,SIGNAL(signal_K12MCU2UI_NEW_M_COMING(quint32,int)),this,SLOT(slot_K12MCU2UI_NEW_M_COMING(quint32,int)));
    connect(this,SIGNAL(signal_K12MCU2UI_VIDEO_DATA()),this,SLOT(slot_K12MCU2UI_VIDEO_DATA()));
    connect(this,SIGNAL(signal_K12MCU2UI_CLOSE_MEDIA(quint32,int)),this,SLOT(slot_K12MCU2UI_CLOSE_MEDIA(quint32,int)));
    connect(ui->SWMPSRight,SIGNAL(clicked(bool)),this,SLOT(slot_SWMPSRight_clicked()));
    connect(ui->SWMPSLeft,SIGNAL(clicked(bool)),this,SLOT(slot_SWMPSLeft_clicked()));
    connect(this,SIGNAL(signal_K12MCU2UI_GET_USER_LIST(user_media_t*,quint32)),this,SLOT(slot_K12MCU2UI_GET_USER_LIST(user_media_t*,quint32)));
    connect(this,SIGNAL(signal_K12MCU2UI_USER_ENTER_ROOM(user_media_t)),this,SLOT(slot_K12MCU2UI_USER_ENTER_ROOM(user_media_t)));
    connect(this,SIGNAL(signal_K12MCU2UI_USER_LEAVE_ROOM(int)),this,SLOT(slot_K12MCU2UI_USER_LEAVE_ROOM(int)));
    connect(ui->PButtonStudentList,SIGNAL(clicked(bool)),this,SLOT(slot_PButtonStudentList_clicked()));
    connect(ui->PButtonVip,SIGNAL(clicked(bool)),this,SLOT(slot_PButtonVip_clicked(bool)));
    connect(ui->PButtonAuditor,SIGNAL(clicked(bool)),this,SLOT(slot_PButtonAuditor_clicked(bool)));
    connect(ui->WidgetGraffiti,SIGNAL(signal_SetSizeFinished()),this,SLOT(slot_SetGraffitiSizeFinished()));
    connect(ui->WidgetGraffiti,SIGNAL(signal_send_graffiti(QJsonObject,QString)),this,SLOT(slot_sendMcuMsg(QJsonObject,QString)));
    connect(this,SIGNAL(signal_CursorChanged(int)),ui->WidgetGraffiti,SLOT(slot_CursorChanged(int)));
    connect(ui->PButtonMouseClickEffect,SIGNAL(clicked(bool)),this,SLOT(slot_PButtonMouseClickEffect_clicked()));
    connect(ui->PButtonPencil,SIGNAL(clicked(bool)),this,SLOT(slot_PButtonPencil_clicked()));
    connect(ui->PButtonGlowSticks,SIGNAL(clicked(bool)),this,SLOT(slot_PButtonGlowSticks_clicked()));
    connect(ui->PButtonRevoke,SIGNAL(clicked(bool)),ui->WidgetGraffiti,SLOT(slot_btnRevoke_clicked()));
    connect(ui->PButtonClear,SIGNAL(clicked(bool)),ui->WidgetGraffiti,SLOT(slot_btnBrush_clicked()));
    connect(this,SIGNAL(signal_SetGraffitiPixmapSize(QSize)),ui->WidgetGraffiti,SLOT(slot_SetSize(QSize)));
    connect(ui->PButtonFile,SIGNAL(clicked(bool)),this,SLOT(slot_PButtonFile_clicked()));
    connect(ui->PButtonScreenClear,SIGNAL(clicked(bool)),this,SLOT(slot_PButtonScreenClear_clicked()));
    connect(ui->PButtonNext,SIGNAL(clicked(bool)),this,SLOT(slot_PButtonNext_clicked()));
    connect(ui->PButtonPrev,SIGNAL(clicked(bool)),this,SLOT(slot_PButtonPrev_clicked()));
    connect(ui->PButtonPlay,SIGNAL(clicked(bool)),this,SLOT(slot_PButtonPlay_clicked(bool)));
    connect(ui->PButtonFullScreen,SIGNAL(clicked(bool)),this,SLOT(slot_PButtonFullScreen_clicked(bool)));
    connect(ui->PButtonHistoricalNewsList,SIGNAL(clicked(bool)),this,SLOT(slot_PButtonHistoricalNewsList_clicked()));
    connect(ui->PButtonBarrageClose,SIGNAL(clicked(bool)),this,SLOT(slot_PButtonBarrageClose_clicked()));
    connect(ui->PButtonScreenshotTool,SIGNAL(clicked(bool)),this,SLOT(slot_PButtonScreenshotTool_clicked()));
    connect(ui->PButtonGiftList,SIGNAL(clicked(bool)),this,SLOT(slot_PButtonGiftList_clicked()));
    connect(ui->PButtonScreenshotAlbum,SIGNAL(clicked(bool)),this,SLOT(slot_PButtonScreenshotAlbum_clicked()));
    connect(ui->PButtonDeviceDetection,SIGNAL(clicked(bool)),this,SLOT(slot_PButtonDeviceDetection_clicked()));
    connect(ui->PButtonDevice,SIGNAL(clicked(bool)),this,SLOT(slot_PButtonDevice_clicked(bool)));
    connect(ui->PButtonSystem,SIGNAL(clicked(bool)),this,SLOT(slot_PButtonSystem_clicked(bool)));
    connect(this,SIGNAL(signal_p2p_msg(quint32,const char*,quint32)),this,SLOT(slot_p2p_msg(quint32,const char*,quint32)));
    connect(this,SIGNAL(signal_recieved_msg(QJsonObject)),this,SLOT(slot_recieved_msg(QJsonObject)));
    connect(this,SIGNAL(signal_p2all_msg(quint32,const char*,quint32)),this,SLOT(slot_p2all_msg(quint32,const char*,quint32)));
	connect(ui->PButtonRecordOperate,SIGNAL(clicked(bool)),this,SLOT(slot_PButtonRecordOperate_clicked()));
    connect(this,SIGNAL(signal_record_msg(int,int)),this,SLOT(slot_record_msg(int,int)));
    connect(ui->CBoxChoiceCamera,SIGNAL(activated(int)),this,SLOT(slot_CBoxChoiceCamera_activated(int)));
    connect(ui->CBoxInputAudio,SIGNAL(activated(int)),this,SLOT(slot_CBoxInputAudio_activated(int)));
    connect(ui->CBoxOutputAudio,SIGNAL(activated(int)),this,SLOT(slot_CBoxOutputAudio_activated(int)));
    connect(ui->ButtonPlayTestAudio,SIGNAL(clicked(bool)),this,SLOT(slot_ButtonPlayTestAudio(bool)));
    connect(this,SIGNAL(signal_play_progress(int,int)),this,SLOT(slot_play_progress(int,int)));

    Global::g_CurRoomId = id;
    m_McuIp = McuIp;
    m_McuPort = McuPort;
    pdu = Libpdu::create(this,Global::g_LoginerId,Global::g_CurRoomId,m_McuIp.toLatin1().data(),m_McuPort,TEACHER_ROLE);
	MediaWTeacher = new MediaWidget(ui->WidgetGraffiti);
    MediaWStudentC = new MediaWidget(ui->WidgetGraffiti);
    connect(MediaWStudentC,SIGNAL(signal_MediaDown(int)),this,SLOT(slot_MediaDown(int)));
	connect(MediaWStudentC,SIGNAL(signal_VideoOpen(int)),this,SLOT(slot_VideoOpen(int)));
	connect(MediaWStudentC,SIGNAL(signal_VideoClose(int)),this,SLOT(slot_VideoClose(int)));
	connect(MediaWStudentC,SIGNAL(signal_AudioOpen(int)),this,SLOT(slot_AudioOpen(int)));
	connect(MediaWStudentC,SIGNAL(signal_AudioClose(int)),this,SLOT(slot_AudioClose(int)));
    MediaWStudentC->hide();
	m_PButtonClassClose = nullptr;
    m_crcpId = crcpId;
    m_PduStatus = 0;
    VideoStatus = 0;
    AudioInStatus = 0;
    m_CourWidShare = nullptr;
    m_iCurSharingPageIndex = 0;
    connect(&m_TimerDocumentPlay,SIGNAL(timeout()),this,SLOT(slot_DocumentPlay()));
    m_RectDocument = QRect(0,0,100,100);
    m_SizeGraffiti = QSize(200,150);
    connect(&m_TimerCheckMedia,SIGNAL(timeout()),this,SLOT(slot_CheckMedia()));
    m_AlbumId = -1;
    m_CoursewareId = CoursewareId;
    m_uddw = new UpDownDisposeWidget(this);
    connect(m_uddw,SIGNAL(signal_udw_arrive(UpDownWidget*)),this,SLOT(slot_udw_arrive(UpDownWidget*)));
    m_k12msgs_Chat = k12msgs::create(this, Global::g_LoginerId, Global::g_CurRoomId,ChatMsgServerIP.toLatin1().data(),ChatMsgServerPort);
    connect(&m_TimerClassOver,SIGNAL(timeout()),this,SLOT(slot_TimerClassOver_timeout()));
    m_TimerClassOver.start(1000);
    m_ClassBegin = ClassBegin;
    m_ClassOver = ClassOver;
    m_ClassEnter = QDateTime::currentDateTime();
    ui->PBarRecord->setMaximum(m_ClassEnter.secsTo(m_ClassOver));
    m_LabelClassOver = new QLabel(this);
	m_LabelClassOver->setStyleSheet("QLabel{font-size:18px;background-color: rgb(255, 255, 255);color:#ff3c1a;}");
    m_LabelClassOver->hide();
    m_CenterMedia = nullptr;
    m_dstip = dstip;
    m_dstport = dstport;
    m_iRecSeconds = 0;
    connect(&m_TimerRecord,SIGNAL(timeout()),this,SLOT(slot_TimerRecord_timeout()));
	play = nullptr;
	m_LabelCloseWait = new QLabel(this);
	m_LabelCloseWait->setScaledContents(true);
	QMovie *me = new QMovie(":/res/room/loading.gif", QByteArray(), this);
	m_LabelCloseWait->setMovie(me);
	me->start();
	m_LabelCloseWait->hide();
	m_IsEight = false;

	QTimer::singleShot(0, this, SLOT(slot_init()));
}

ClassroomDialog::~ClassroomDialog()
{
    qDebug() << "ClassroomDialog::~ClassroomDialog";
    CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);//必须加这句，否则只要打开音频，在或者退出教室、或者退出教室再进就会崩溃
	if(m_TimerRecord.isActive())
		m_TimerRecord.stop();
	if(play)
    {
        play->stop();
        delete play;
        play = nullptr;
    }
    if (m_CenterMedia)
    {
        m_CenterMedia->stop_record();
        delete m_CenterMedia;
        m_CenterMedia = nullptr;
	}
    m_CourWidShare = nullptr;
    delete m_k12msgs_Chat;
    m_k12msgs_Chat = nullptr;
    queue_mutex.lock();
    while (video_datas.size() > 0)
    {
        k12mcu2ui_video_data_t fvd = video_datas.front();
        video_datas.pop();
        pdu->msg(LIBPDU_DESTORY_VIDEO_DATA, fvd.data);
    }
    queue_mutex.unlock();
    if(pdu)
        Libpdu::destory(pdu);
    pdu = nullptr;
    Global::DelDir(Global::g_AppDataPath + "screenshot_temp/");

	delete ui;
}

void ClassroomDialog::closeEvent(QCloseEvent *event)
{
	event->ignore();
	slot_PButtonClassClose_clicked();
}

void ClassroomDialog::keyPressEvent(QKeyEvent *event)
{
	switch (event->key())
	{
	case Qt::Key_Escape:
		break;
	default:
		QDialog::keyPressEvent(event);
	}
}

void ClassroomDialog::slot_init()
{
	qDebug() << "ClassroomDialog::slot_init";
	if(pdu == nullptr)
	{
        Global::g_NoticeDialog->init("创建MCU连接失败！","","",5000);
		Global::g_NoticeDialog->exec();
		CloseClassroom();
		return ;
	}
    if(m_k12msgs_Chat == nullptr)
    {
        Global::g_NoticeDialog->init("连接聊天服务器失败,请检查网络后重试！","","",5000);
        Global::g_NoticeDialog->exec();
		CloseClassroom();
        return ;
    }

    ui->SWidgetMedia->setFixedHeight(ui->WidgetTool->height()-ui->PButtonGiftList->y()-80+10);
    ui->SWidgetMedia->setCurrentWidget(ui->SWMPageNone);
    MediaWTeacher->setGeometry(0,0,ui->MWidgetStudent1->width(),ui->MWidgetStudent1->height());
    int iw = (ui->WidgetGraffiti->width()-ui->WidgetTool->width()-100-100-30)/2;
    MediaWStudentC->setGeometry(100+iw+30,100,iw,ui->SWidgetMedia->y()-40-100-100);
    m_uddw->move(width()-m_uddw->width(),height()-m_uddw->height());
    m_uddw->hide();
    m_PButtonClassClose = new QPushButton(this);
    m_PButtonClassClose->setStyleSheet("QPushButton{font: bold 20px \"宋体\";border:none;}");
    m_PButtonClassClose->setText("×");
    connect(m_PButtonClassClose,SIGNAL(clicked(bool)),this,SLOT(slot_PButtonClassClose_clicked()));
    m_PButtonClassClose->setGeometry(width()-30,0,30,30);
	m_PButtonClassClose->show();
	m_LabelCloseWait->setGeometry((width()-60)/2,(height()-60)/2,60,60);

	media_type_t eMediaType = COMMON_VIDEO;
	int iCameraCount = pdu->msg(LIBPDU_GET_MEDIA_DEV_CNT, &eMediaType);
	if(iCameraCount > 0)
	{
		for(int i=0;i<iCameraCount;i++)
		{
			media_name_t sct;
			sct.index=i;
			sct.t=COMMON_VIDEO;
			pdu->msg(LIBPDU_GET_MEDIA_NAME, &sct);
			ui->CBoxChoiceCamera->addItem(QString(sct.name));
		}
		int VideoIndex = Global::g_Settings->value("conf/VideoDevSet").toInt();
		if(VideoIndex < 0 || VideoIndex >= iCameraCount)
			VideoIndex = 0;
		ui->CBoxChoiceCamera->setCurrentIndex(VideoIndex);
		Global::g_Settings->setValue("conf/VideoDevSet",VideoIndex);
		media_dev_index_t mdit;
		mdit.index = VideoIndex;
		mdit.t = COMMON_VIDEO;
		pdu->msg(LIBPDU_SET_MEDIA_DEV_INDEX, &mdit);
	}
	else
	{
		Global::g_NoticeDialog->init("没找到可用的摄像头！","","",2000);
		Global::g_NoticeDialog->exec();
	}
	eMediaType = COMMON_AUDIO;
	int iMikeCount = pdu->msg(LIBPDU_GET_MEDIA_DEV_CNT, &eMediaType);
	if(iMikeCount > 0)
	{
		for(int i=0;i<iMikeCount;i++)
		{
			media_name_t sct;
			sct.index=i;
			sct.t=COMMON_AUDIO;
			pdu->msg(LIBPDU_GET_MEDIA_NAME, &sct);
			ui->CBoxInputAudio->addItem(QString(sct.name));
		}
		int inx = Global::g_Settings->value("conf/AudioInDevSet").toInt();
		if(inx < 0 || inx >= iMikeCount)
			inx = 0;
		ui->CBoxInputAudio->setCurrentIndex(inx);
		Global::g_Settings->setValue("conf/AudioInDevSet", inx);
		media_dev_index_t mdit;
		mdit.index = inx;
		mdit.t = COMMON_AUDIO;
		pdu->msg(LIBPDU_SET_MEDIA_DEV_INDEX, &mdit);
	}
	else
	{
		Global::g_NoticeDialog->init("没找到可用的麦克风！","","",2000);
		Global::g_NoticeDialog->exec();
	}
	int iHeadphoneCount = pdu->msg(LIBPDU_GET_PLAYOUT_CNT, nullptr);
	if(iHeadphoneCount > 0)
	{
		for (int i = 0;i < iHeadphoneCount;i++)
		{
			playout_name_t sct;
			sct.index=i;
			pdu->msg(LIBPDU_GET_PLAYOUT_NAME, &sct);
			ui->CBoxOutputAudio->addItem(QString(sct.name));
		}
		int iSpeakerIndex = Global::g_Settings->value("conf/AudioOutDevSet").toInt();
		if(iSpeakerIndex < 0 || iSpeakerIndex >= iHeadphoneCount)
			iSpeakerIndex = 0;
		ui->CBoxOutputAudio->setCurrentIndex(iSpeakerIndex);
		Global::g_Settings->setValue("conf/AudioOutDevSet", iSpeakerIndex);
		pdu->msg(LIBPDU_SET_PLAYOUT_INDEX, &iSpeakerIndex);
	}
	else
	{
		Global::g_NoticeDialog->init("没找到可用的扬声器！","","",2000);
		Global::g_NoticeDialog->exec();
	}

    ui->LabelSystemTInfo->setText(QString("ID：%1    昵称：%2").arg(Global::g_LoginerId).arg(Global::g_LoginerName));
    ui->LabelSystemCName->setText(QString("电脑名称：%1").arg(QHostInfo::localHostName()));
	ui->LabelSystemLIP->setText(QString("IP地址：%1").arg(Global::g_LocalIp));
    ui->LabelSystemDispose->setText(QString("处理器：%1").arg(Global::getCpuDescribe()));
    ui->LabelSystemOSystem->setText(QString("操作系统：%1").arg(QSysInfo::prettyProductName()));

    NAManager *nam = new NAManager();
    connect(nam,SIGNAL(signal_reply_finished(QJsonObject,void*)),this,SLOT(slot_GetCourseware(QJsonObject,void*)));
    QByteArray ba = QCryptographicHash::hash(QString("ciId=%1&page=1&rows=10000&key=RMS").arg(m_CoursewareId).toUtf8(),QCryptographicHash::Md5);
    QString md5(ba.toHex().toUpper());
    QJsonObject json;
    json.insert("ciId", QString::number(m_CoursewareId));
    json.insert("page", QString::number(1));
    json.insert("rows", QString::number(10000));
    json.insert("sign", md5);
    QString strParam = QString("service=courseware&method=queryList&params=%1").arg(QString(QJsonDocument(json).toJson(QJsonDocument::Compact)));
    nam->SendRequest(QString("http://%1/rms/service/sys/execute").arg(Global::g_ServerIp),strParam);

    NAManager *nam1 = new NAManager();//查询礼物列表
    connect(nam1,SIGNAL(signal_reply_finished(QJsonObject,void*)),this,SLOT(slot_GetGiftList(QJsonObject,void*)));
    QByteArray ba1 = QCryptographicHash::hash(QString("classId=%1&page=1&rows=10000&key=MBS").arg(Global::g_CurRoomId).toUtf8(),QCryptographicHash::Md5);
    QString md51(ba1.toHex().toUpper());
    QJsonObject json1;
    json1.insert("classId", QString::number(Global::g_CurRoomId));
    json1.insert("page", QString::number(1));
    json1.insert("rows", QString::number(10000));
    json1.insert("sign", md51);
    QString strParam1 = QString("service=sysGifts&method=useHistory&params=%1").arg(QString(QJsonDocument(json1).toJson(QJsonDocument::Compact)));
    nam1->SendRequest(QString("http://%1/mbs/service/sys/execute").arg(Global::g_ServerIp),strParam1);

    NAManager *nam2 = new NAManager();//查询弹幕列表
    connect(nam2,SIGNAL(signal_reply_finished(QJsonObject,void*)),this,SLOT(slot_GetBarrageList(QJsonObject,void*)));
    QByteArray ba2 = QCryptographicHash::hash(QString("classId=%1&page=1&rows=10000&key=CS").arg(Global::g_CurRoomId).toUtf8(),QCryptographicHash::Md5);
    QString md52(ba2.toHex().toUpper());
    QJsonObject json2;
    json2.insert("classId", QString::number(Global::g_CurRoomId));
    json2.insert("page", QString::number(1));
    json2.insert("rows", QString::number(10000));
    json2.insert("sign", md52);
    QString strParam2 = QString("service=chatRecords&method=queryList&params=%1").arg(QString(QJsonDocument(json2).toJson(QJsonDocument::Compact)));
    nam2->SendRequest(QString("http://%1/cs/service/sys/execute").arg(Global::g_ServerIp),strParam2);

    NAManager *nam3 = new NAManager();//查询截图相册
    connect(nam3,SIGNAL(signal_reply_finished(QJsonObject,void*)),this,SLOT(slot_GetAlbumList(QJsonObject,void*)));
    QByteArray ba3 = QCryptographicHash::hash(QString("classId=%1&page=1&rows=10000&key=RMS").arg(Global::g_CurRoomId).toUtf8(),QCryptographicHash::Md5);
    QString md53(ba3.toHex().toUpper());
    QJsonObject json3;
    json3.insert("classId", QString::number(Global::g_CurRoomId));
    json3.insert("page", QString::number(1));
    json3.insert("rows", QString::number(10000));
    json3.insert("sign", md53);
    QString strParam3 = QString("service=classPhotos&method=queryList&params=%1").arg(QString(QJsonDocument(json3).toJson(QJsonDocument::Compact)));
    nam3->SendRequest(QString("http://%1/rms/service/sys/execute").arg(Global::g_ServerIp),strParam3);
}

void ClassroomDialog::slot_K12MCU2UI_CONNECT2MCU_STATE(int flag)
{
    qDebug() << "ClassroomDialog::slot_K12MCU2UI_CONNECT2MCU_STATE:flag=" << flag;
	if(flag == 1)
    {
        m_PduStatus = 1;
		pdu->start_class();
    }
	else
		slot_K12MCU2UI_DISCONNECT_FROM_MCU();
}

void ClassroomDialog::slot_K12MCU2UI_DISCONNECT_FROM_MCU()
{
    Global::g_NoticeDialog->init("连接MCU失败！","","",5000);
    Global::g_NoticeDialog->exec();
    m_PduStatus = 0;
	CloseClassroom();
}

void ClassroomDialog::slot_start_class_ack(int code)
{
    qDebug() << "ClassroomDialog::slot_start_class_ack:code=" << code;
    if(code != 0)
    {
        Global::g_NoticeDialog->init("开课失败！","","",5000);
        Global::g_NoticeDialog->exec();
		CloseClassroom();
        return ;
    }

	m_PduStatus = 2;

    slot_CheckMedia();
    m_TimerCheckMedia.start(5000);

    m_SizeGraffiti = QSize(ui->WidgetGraffiti->width(),ui->SWidgetMedia->y()-40);
    emit signal_SetGraffitiPixmapSize(m_SizeGraffiti);

	slot_PButtonRecordOperate_clicked();

    NAManager *nr = new NAManager();
    connect(nr,SIGNAL(signal_reply_finished(QJsonObject,void*)),this,SLOT(slot_send_openClass(QJsonObject,void*)));
    QByteArray ba = QCryptographicHash::hash(QString("crcpId=%1&criId=%2&tiId=%3&key=RMS").arg(m_crcpId).arg(Global::g_CurRoomId).arg(Global::g_LoginerId).toUtf8(),
                                             QCryptographicHash::Md5);
    QString md5(ba.toHex().toUpper());
    QJsonObject json;
    json.insert("crcpId", QString::number(m_crcpId));
    json.insert("criId", QString::number(Global::g_CurRoomId));
    json.insert("tiId", QString::number(Global::g_LoginerId));
    json.insert("sign", md5);
    QString strParam = QString("service=classroom&method=openClass&params=%1").arg(QString(QJsonDocument(json).toJson(QJsonDocument::Compact)));
    nr->SendRequest(QString("http://%1/rms/service/sys/execute").arg(Global::g_ServerIp),strParam);
}

void ClassroomDialog::slot_send_openClass(QJsonObject obj,void*)
{
    qDebug() << "ClassroomDialog::slot_send_openClass:obj=" << obj;
    if(obj.isEmpty())
    {
        Global::g_NoticeDialog->init("网络异常，发送开课消息失败！","","",2000);
        Global::g_NoticeDialog->exec();
        return ;
    }
    if (obj["msg"].toString() != "成功")
    {
        Global::g_NoticeDialog->init(obj["msg"].toString(),"","",2000);
        Global::g_NoticeDialog->exec();
        return ;
    }
}

void ClassroomDialog::slot_stop_class_ack(int code)
{
    qDebug() << "ClassroomDialog::slot_stop_class_ack:code=" << code;
    if(code != 0)
        return ;

    NAManager *nr = new NAManager();
    connect(nr,SIGNAL(signal_reply_finished(QJsonObject,void*)),this,SLOT(slot_send_stopClass(QJsonObject,void*)));
    QByteArray ba = QCryptographicHash::hash(QString("crcpId=%1&criId=%2&tiId=%3&key=RMS").arg(m_crcpId).arg(Global::g_CurRoomId).arg(Global::g_LoginerId).toUtf8(),
                                             QCryptographicHash::Md5);
    QString md5(ba.toHex().toUpper());
    QJsonObject json;
    json.insert("crcpId", QString::number(m_crcpId));
    json.insert("criId", QString::number(Global::g_CurRoomId));
    json.insert("tiId", QString::number(Global::g_LoginerId));
    json.insert("sign", md5);
    QString strParam = QString("service=classroom&method=stopClass&params=%1").arg(QString(QJsonDocument(json).toJson(QJsonDocument::Compact)));
    nr->SendRequest(QString("http://%1/rms/service/sys/execute").arg(Global::g_ServerIp),strParam);
}

void ClassroomDialog::slot_send_stopClass(QJsonObject obj,void*)
{
    qDebug() << "ClassroomDialog::slot_send_stopClass:obj=" << obj;
    if(obj.isEmpty())
    {
        Global::g_NoticeDialog->init("网络异常，发送停课消息失败！","","",2000);
        Global::g_NoticeDialog->exec();
        return ;
    }
    if (obj["msg"].toString() != "成功")
    {
        Global::g_NoticeDialog->init(obj["msg"].toString(),"","",2000);
        Global::g_NoticeDialog->exec();
        return ;
    }

    accept();
}

void ClassroomDialog::CloseClassroom()
{
	qDebug() << "ClassroomDialog::CloseClassroom:m_PduStatus=" << m_PduStatus;
	if(m_TimerClassOver.isActive())
		m_TimerClassOver.stop();
	if(m_TimerCheckMedia.isActive())
		m_TimerCheckMedia.stop();
	if(m_TimerDocumentPlay.isActive())
		m_TimerDocumentPlay.stop();
	setEnabled(false);
	switch(m_PduStatus)
	{
	default:
		reject();
		break;
	case 2:
		if(m_CenterMedia && m_TimerRecord.isActive())
		{
			m_PduStatus = 4;
			slot_PButtonRecordOperate_clicked();
		}
		else
		{
			slot_PButtonScreenClear_clicked();
			pdu->stop_class(Global::g_LoginerId);
			QTimer::singleShot(5000, this, SLOT(reject()));
		}
		break;
	case 3:
		emit signal_BeSqueezedOut();
		reject();
		break;
	case 4:
		break;
	}
}

void ClassroomDialog::slot_PButtonClassClose_clicked()
{
	qDebug() << "ClassroomDialog::slot_PButtonClassClose_clicked";
	Global::g_NoticeDialog->init("是否退出教室？","是","否",5000);
	if(Global::g_NoticeDialog->exec() == QDialog::Rejected)
		return ;
	CloseClassroom();
}

void ClassroomDialog::slot_VideoOpen(int UserId)
{
    qDebug() << "ClassroomDialog::slot_VideoOpen:UserId=" << UserId;
    StudentWidget *sw = GetStudentWidget(UserId);
    if(sw == nullptr && UserId != Global::g_LoginerId)
        return ;
    if(UserId == Global::g_LoginerId && VideoStatus == 2)
        return ;
    if(sw != nullptr)
    {
        if(sw->VideoStatus == 2)
        {
            Global::g_NoticeDialog->init("该学生的摄像头不存在或故障！","","",2000);
            Global::g_NoticeDialog->exec();
            return ;
        }
        else if(sw->VideoStatus == 3)
        {
            Global::g_NoticeDialog->init("该学生已设置只听音频！","","",2000);
            Global::g_NoticeDialog->exec();
            return ;
        }
    }

    user_media_t sct;
    sct.userid = UserId;
    sct.t = COMMON_VIDEO;
    if(pdu->msg(LIBPDU_OPEN_USER_MEDIA, &sct) != 0)
    {
        if(UserId != Global::g_LoginerId)
        {
            Global::g_NoticeDialog->init("打开该学生视频失败！","","",2000);
            Global::g_NoticeDialog->exec();
        }
    }
    else
    {
		if(sw)
		{
			if(sw->IsTeacherCloseVideo == true)
				sw->IsTeacherCloseVideo = false;
		}

        QJsonObject json;
        json.insert("typeId", "306");
        json.insert("content", QString::number(1));
        slot_sendMcuMsg(json,QString::number(UserId));
    }
}

void ClassroomDialog::slot_AudioOpen(int UserId)
{
    qDebug() << "ClassroomDialog::slot_AudioOpen:UserId=" << UserId;
    StudentWidget *sw = GetStudentWidget(UserId);
    if(sw == nullptr && UserId != Global::g_LoginerId)
        return ;
    if(UserId == Global::g_LoginerId && AudioInStatus == 2)
        return ;
    if(sw != nullptr)
    {
        if(sw->AudioStatus == 2)
        {
            Global::g_NoticeDialog->init("该学生的麦克风不存在或故障！","","",2000);
            Global::g_NoticeDialog->exec();
            return ;
        }
    }
	if(GetMeidaAudioOnCount() == 4)
	{
		Global::g_NoticeDialog->init("最多只能打开4路音频！","","",2000);
		Global::g_NoticeDialog->exec();
		return ;
	}

    user_media_t sct;
    sct.userid = UserId;
    sct.t = COMMON_AUDIO;
    if(pdu->msg(LIBPDU_OPEN_USER_MEDIA, &sct) != 0)
    {
        if(UserId != Global::g_LoginerId)
        {
            Global::g_NoticeDialog->init("打开该学生音频失败！","","",2000);
            Global::g_NoticeDialog->exec();
        }
	}
}

void ClassroomDialog::slot_VideoClose(int UserId)
{
    qDebug() << "ClassroomDialog::slot_VideoClose:UserId=" << UserId;
    StudentWidget *sw = GetStudentWidget(UserId);
    if(sw == nullptr && UserId != Global::g_LoginerId)
        return ;

    user_media_t sct;
    sct.userid = UserId;
    sct.t = COMMON_VIDEO;
    if(pdu->msg(LIBPDU_CLOSE_USER_MEDIA, &sct) != 0)
    {
        if(UserId != Global::g_LoginerId)
        {
            Global::g_NoticeDialog->init("关闭该学生视频失败！","","",2000);
            Global::g_NoticeDialog->exec();
        }
    }
    else
    {
		if(sw)
			sw->IsTeacherCloseVideo = true;

        QJsonObject json;
        json.insert("typeId", "306");
        json.insert("content", QString::number(0));
        slot_sendMcuMsg(json,QString::number(UserId));
    }
}

void ClassroomDialog::slot_AudioClose(int UserId)
{
    qDebug() << "ClassroomDialog::slot_AudioClose:UserId=" << UserId;
    StudentWidget *sw = GetStudentWidget(UserId);
    if(sw == nullptr && UserId != Global::g_LoginerId)
        return ;

    user_media_t sct;
    sct.userid = UserId;
    sct.t = COMMON_AUDIO;
    if(pdu->msg(LIBPDU_CLOSE_USER_MEDIA, &sct) != 0)
    {
        if(UserId != Global::g_LoginerId)
        {
            Global::g_NoticeDialog->init("关闭该学生音频失败！","","",2000);
            Global::g_NoticeDialog->exec();
        }
	}
}

void ClassroomDialog::slot_sendMcuMsg(QJsonObject obj, QString strRecvUserId)
{
    qDebug() << "ClassroomDialog::slot_sendMcuMsg:obj=" << obj << ",strRecvUserId=" << strRecvUserId;
    QString strContent = QString(QJsonDocument(obj).toJson(QJsonDocument::Compact));
    QByteArray ba = strContent.toUtf8();
    if(strRecvUserId == "*")
        pdu->send_p2all_msg(ba.data(), ba.length());
    else
        pdu->send_p2p_msg(strRecvUserId.toUInt(), ba.data(), ba.length());
}

void ClassroomDialog::slot_CheckMedia()
{
    ui->CBoxChoiceCamera->clear();
    media_type_t eMediaType = COMMON_VIDEO;
    int iCameraCount = pdu->msg(LIBPDU_GET_MEDIA_DEV_CNT, &eMediaType);
    if(iCameraCount > 0)
    {
        for(int i=0;i<iCameraCount;i++)
        {
            media_name_t sct;
            sct.index=i;
            sct.t=COMMON_VIDEO;
            pdu->msg(LIBPDU_GET_MEDIA_NAME, &sct);
            ui->CBoxChoiceCamera->addItem(QString(sct.name));
        }
        int VideoIndex = Global::g_Settings->value("conf/VideoDevSet").toInt();
        if(VideoIndex >= 0 && VideoIndex < iCameraCount)
        {
            ui->CBoxChoiceCamera->setCurrentIndex(VideoIndex);
            switch(VideoStatus)
            {
            case 0:
                slot_VideoOpen(Global::g_LoginerId);
                break;
            case 2:
                VideoStatus = 0;
                slot_VideoOpen(Global::g_LoginerId);
                break;
            default:
                break;
            }
        }
        else
        {
            VideoStatus = 2;
            slot_VideoClose(Global::g_LoginerId);
        }
    }
    else
    {
        VideoStatus = 2;
        slot_VideoClose(Global::g_LoginerId);
    }
    ui->CBoxInputAudio->clear();
    eMediaType = COMMON_AUDIO;
    int iMikeCount = pdu->msg(LIBPDU_GET_MEDIA_DEV_CNT, &eMediaType);
    if(iMikeCount > 0)
    {
        for(int i=0;i<iMikeCount;i++)
        {
            media_name_t sct;
            sct.index=i;
            sct.t=COMMON_AUDIO;
            pdu->msg(LIBPDU_GET_MEDIA_NAME, &sct);
            ui->CBoxInputAudio->addItem(QString(sct.name));
        }
        int AudioInIndex = Global::g_Settings->value("conf/AudioInDevSet").toInt();
        if(AudioInIndex >= 0 && AudioInIndex < iMikeCount)
        {
            ui->CBoxInputAudio->setCurrentIndex(AudioInIndex);
            switch(AudioInStatus)
            {
            case 0:
                slot_AudioOpen(Global::g_LoginerId);
                break;
            case 2:
                AudioInStatus = 0;
                slot_AudioOpen(Global::g_LoginerId);
                break;
            default:
                break;
            }
        }
        else
        {
            AudioInStatus = 2;
            slot_AudioClose(Global::g_LoginerId);
        }
    }
    else
    {
        AudioInStatus = 2;
        slot_AudioClose(Global::g_LoginerId);
    }

    ui->LabelSystemMemory->setText(QString("内存：%1").arg(Global::getMemory()));
    QStringList sl = Global::GetNetDelayLoss().split(",");
    ui->LabelSystemDelay->setText(QString("网络延时：%1").arg(sl.at(0)));
    ui->LabelSystemLoss->setText(QString("丢包率：%1").arg(sl.at(1)));

	InsertMedia();
}

void ClassroomDialog::slot_K12MCU2UI_NEW_M_COMING(quint32 userid, int mediaType)
{
    qDebug() << "ClassroomDialog::slot_K12MCU2UI_NEW_M_COMING:userid=" << userid << ",mediaType=" << mediaType;
    MediaWidget *mw = GetMediaWidget(userid);
    StudentWidget *sw = GetStudentWidget(userid);
    switch (mediaType)
    {
    case COMMON_VIDEO:
        if(mw)
        {
			if(userid == Global::g_LoginerId)
				VideoStatus = 1;
			else if(sw)
			{
				sw->HaveVideoStream = true;
				sw->VideoStatus = 1;
			}
            user_media_t um;
            um.t = (media_type_t)mediaType;
            um.userid = userid;
            pdu->msg(LIBPDU_GET_VIDEO, &um);
            mw->SetVideoStatus(1);
        }
        else
        {
            if(userid == Global::g_LoginerId)
			{
				VideoStatus = 1;
                user_media_t um;
                um.t = (media_type_t)mediaType;
                um.userid = userid;
                pdu->msg(LIBPDU_GET_VIDEO, &um);
                MediaWTeacher->m_id = userid;
				MediaWTeacher->SetHeadName();
				MediaWTeacher->SetVideoStatus(1);
            }
            else if(sw)
            {
				sw->HaveVideoStream = true;
                MediaWidget *mwn = GetMediaWidgetVipNull();
                if(mwn)
				{
					sw->VideoStatus = 1;
                    user_media_t um;
                    um.t = (media_type_t)mediaType;
                    um.userid = userid;
                    pdu->msg(LIBPDU_GET_VIDEO, &um);
                    mwn->m_id = userid;
                    mwn->SetHeadName();
                    mwn->SetVideoStatus(1);
				}
            }
            else
                MapNeedVideo.insert(userid,mediaType);
        }
        break;
	case COMMON_AUDIO:
		if(userid == Global::g_LoginerId)
			AudioInStatus = 1;
		else if(sw)
			sw->AudioStatus = 1;
        if(mw)
            mw->SetAudioStatus(1);
		else
		{
			if(userid == Global::g_LoginerId)
			{
				MediaWTeacher->m_id = userid;
				MediaWTeacher->SetHeadName();
				MediaWTeacher->SetAudioStatus(1);
			}
			else if(sw)
			{
				MediaWidget *mwn = GetMediaWidgetVipNull();
				if(mwn)
				{
					mwn->m_id = userid;
					mwn->SetHeadName();
					mwn->SetAudioStatus(1);
				}
			}
		}
        break;
    default:
        break;
	}
}

void ClassroomDialog::slot_K12MCU2UI_VIDEO_DATA()
{
    k12mcu2ui_video_data_t fvd;
    queue_mutex.lock();
    if (video_datas.size() <= 0)
    {
        queue_mutex.unlock();
        return;
    }
    fvd = video_datas.front();
    video_datas.pop();
    queue_mutex.unlock();

    switch(fvd.type)
    {
    case COMMON_VIDEO:
    {
        MediaWidget *mw = GetMediaWidget(fvd.userid);
        if(mw)
        {
            QImage image(fvd.data, fvd.w, fvd.h, QImage::Format_RGB888);
            if(image.isNull() == false)
            {
                QPixmap pix = QPixmap::fromImage(image);
                if(pix.isNull() == false)
                    mw->ChangePixmap(pix);
            }
        }
    }
        break;
    default:
        break;
    }

    pdu->msg(LIBPDU_DESTORY_VIDEO_DATA, fvd.data);
}

void ClassroomDialog::slot_K12MCU2UI_CLOSE_MEDIA(quint32 userid, int mediaType)
{
    qDebug() << "ClassroomDialog::slot_K12MCU2UI_CLOSE_MEDIA:userid=" << userid << ",mediaType=" << mediaType;
    StudentWidget *sw = GetStudentWidget(userid);
    MediaWidget *mw = GetMediaWidget(userid);

    switch (mediaType)
    {
    case COMMON_VIDEO:
		if(mw)
		{
			user_media_t um;
			um.t = (media_type_t)mediaType;
			um.userid = userid;
			pdu->msg(LIBPDU_CANCEL_VIDEO, &um);
			mw->SetVideoStatus(0);
			mw->ChangePixmap(QPixmap());
			if(userid == Global::g_LoginerId)
			{
				VideoStatus = 0;
				if(AudioInStatus != 1)
					mw->ClearAll();
			}
			else if(sw)
			{
				sw->HaveVideoStream = false;
				sw->VideoStatus = 0;
				if(sw->AudioStatus != 1)
				{
					mw->ClearAll();
					if(mw == MediaWStudentC)
					{
						MediaWStudentC->hide();
						MediaWTeacher->setGeometry(0,0,ui->MWidgetStudent1->width(),ui->MWidgetStudent1->height());
						if(m_CourWidShare)
							MediaWTeacher->hide();

						QJsonObject json;
						json.insert("typeId", "301");
						json.insert("content", "0");
						json.insert("sendUserId", QString::number(Global::g_LoginerId));
						json.insert("downUserId", QString::number(userid));
						slot_sendMcuMsg(json);
					}
				}
			}
		}
		else
		{
			if(sw)
			{
				sw->HaveVideoStream = false;
				sw->VideoStatus = 0;
			}
		}
        break;
    case COMMON_AUDIO:
		if(mw)
		{
			mw->SetAudioStatus(0);
			if(userid == Global::g_LoginerId)
			{
				AudioInStatus = 0;
				if(VideoStatus != 1)
					mw->ClearAll();
			}
			else
			{
				sw->AudioStatus = 0;
				if(sw->VideoStatus != 1)
				{
					if(mw)
						mw->ClearAll();
					if(mw == MediaWStudentC)
					{
						MediaWStudentC->hide();
						MediaWTeacher->setGeometry(0,0,ui->MWidgetStudent1->width(),ui->MWidgetStudent1->height());
						if(m_CourWidShare)
							MediaWTeacher->hide();

						QJsonObject json;
						json.insert("typeId", "301");
						json.insert("content", "0");
						json.insert("sendUserId", QString::number(Global::g_LoginerId));
						json.insert("downUserId", QString::number(userid));
						slot_sendMcuMsg(json);
					}
				}
			}
		}
		else
		{
			if(sw)
				sw->AudioStatus = 0;
		}
        break;
    default:
        break;
    }
}

MediaWidget *ClassroomDialog::GetMediaWidget(int user_id)
{
    if(user_id == MediaWTeacher->m_id)
        return MediaWTeacher;
    if(user_id == ui->MWidgetStudent1->m_id)
        return ui->MWidgetStudent1;
    if(user_id == ui->MWidgetStudent2->m_id)
        return ui->MWidgetStudent2;
    if(user_id == ui->MWidgetStudent3->m_id)
        return ui->MWidgetStudent3;
    if(user_id == ui->MWidgetStudent4->m_id)
        return ui->MWidgetStudent4;
    if(user_id == MediaWStudentC->m_id)
        return MediaWStudentC;
    return nullptr;
}

MediaWidget *ClassroomDialog::GetMediaWidgetVipNull()
{
    if(-1 == ui->MWidgetStudent1->m_id)
        return ui->MWidgetStudent1;
    if(-1 == ui->MWidgetStudent2->m_id)
        return ui->MWidgetStudent2;
    if(-1 == ui->MWidgetStudent3->m_id)
        return ui->MWidgetStudent3;
    if(-1 == ui->MWidgetStudent4->m_id)
        return ui->MWidgetStudent4;
    return nullptr;
}

StudentWidget *ClassroomDialog::GetStudentWidget(int UserId,int UserType)
{
    if(UserType == 1)
    {
        for(int i = 0;i < ui->VLayoutVip->count();i++)
        {
            StudentWidget *sw = (StudentWidget *)ui->VLayoutVip->itemAt(i)->widget();
            if(sw->m_id == UserId)
                return sw;
        }
        return nullptr;
    }
    else
    {
        for(int i = 0;i < ui->VLayoutAuditor->count();i++)
        {
            StudentWidget *sw = (StudentWidget *)ui->VLayoutAuditor->itemAt(i)->widget();
            if(sw->m_id == UserId)
                return sw;
        }
        return nullptr;
    }
}

int ClassroomDialog::GetMeidaAudioOnCount()
{
    int ret = 0;
    if(AudioInStatus == 1)
        ret++;
    for(int i = 0;i < ui->VLayoutVip->count();i++)
    {
        StudentWidget *sw = (StudentWidget *)ui->VLayoutVip->itemAt(i)->widget();
        if(sw->AudioStatus == 1)
            ret++;
    }
    return ret;
}

void ClassroomDialog::slot_SWMPSRight_clicked()
{
	int MaxInx = 0;
	for(int i = 0;i < ui->VLayoutVip->count();i++)
	{
		StudentWidget *sw = (StudentWidget *)ui->VLayoutVip->itemAt(i)->widget();
		if(sw->m_id == ui->MWidgetStudent1->m_id)
			MaxInx = i;
		else if(sw->m_id == ui->MWidgetStudent2->m_id)
			MaxInx = i;
		else if(sw->m_id == ui->MWidgetStudent3->m_id)
			MaxInx = i;
		else if(sw->m_id == ui->MWidgetStudent4->m_id)
			MaxInx = i;
	}

	QList<int> ListLeftId;
	QList<int> ListRightId;
	for(int i = 0;i < ui->VLayoutVip->count();i++)
	{
		StudentWidget *sw = (StudentWidget *)ui->VLayoutVip->itemAt(i)->widget();
		if(sw->HaveVideoStream == true || sw->AudioStatus == 1)
		{
			if(i > MaxInx)
				ListRightId.append(sw->m_id);
			else
				ListLeftId.append(sw->m_id);
		}
	}

	if(ListLeftId.count() + ListRightId.count() > 4)
    {
        if(ui->MWidgetStudent1->m_id != -1)
		{
			StudentWidget *sw = GetStudentWidget(ui->MWidgetStudent1->m_id);
			if(sw->VideoStatus == 1)
			{
				slot_K12MCU2UI_CLOSE_MEDIA(sw->m_id,COMMON_VIDEO);
				sw->HaveVideoStream = true;
			}
			ui->MWidgetStudent1->ClearAll();
        }
        if(ui->MWidgetStudent2->m_id != -1)
        {
            StudentWidget *sw = GetStudentWidget(ui->MWidgetStudent2->m_id);
            if(sw->VideoStatus == 1)
            {
                slot_K12MCU2UI_CLOSE_MEDIA(sw->m_id,COMMON_VIDEO);
				sw->HaveVideoStream = true;
            }
			ui->MWidgetStudent2->ClearAll();
        }
        if(ui->MWidgetStudent3->m_id != -1)
        {
            StudentWidget *sw = GetStudentWidget(ui->MWidgetStudent3->m_id);
            if(sw->VideoStatus == 1)
            {
                slot_K12MCU2UI_CLOSE_MEDIA(sw->m_id,COMMON_VIDEO);
				sw->HaveVideoStream = true;
            }
			ui->MWidgetStudent3->ClearAll();
        }
        if(ui->MWidgetStudent4->m_id != -1)
        {
            StudentWidget *sw = GetStudentWidget(ui->MWidgetStudent4->m_id);
            if(sw->VideoStatus == 1)
            {
                slot_K12MCU2UI_CLOSE_MEDIA(sw->m_id,COMMON_VIDEO);
				sw->HaveVideoStream = true;
            }
			ui->MWidgetStudent4->ClearAll();
        }

		int OnCount = 0;
		for(int i = 0;i < ListRightId.count();i++)
		{
			StudentWidget *sw = GetStudentWidget(ListRightId.at(i));
			if(sw->HaveVideoStream)
				slot_K12MCU2UI_NEW_M_COMING(ListRightId.at(i),COMMON_VIDEO);
			if(sw->AudioStatus == 1)
				slot_K12MCU2UI_NEW_M_COMING(ListRightId.at(i),COMMON_AUDIO);
			OnCount++;
			if(OnCount >= 4)
				break;
		}
		if(OnCount < 4)
		{
			for(int i = 0;i < ListLeftId.count();i++)
			{
				StudentWidget *sw = GetStudentWidget(ListLeftId.at(i));
				if(sw->HaveVideoStream)
					slot_K12MCU2UI_NEW_M_COMING(ListLeftId.at(i),COMMON_VIDEO);
				if(sw->AudioStatus == 1)
					slot_K12MCU2UI_NEW_M_COMING(ListLeftId.at(i),COMMON_AUDIO);
				OnCount++;
				if(OnCount >= 4)
					break;
			}
		}
	}
}

void ClassroomDialog::slot_SWMPSLeft_clicked()
{
	int MinInx = 0;
	for(int i = ui->VLayoutVip->count() - 1;i >= 0;i--)
	{
		StudentWidget *sw = (StudentWidget *)ui->VLayoutVip->itemAt(i)->widget();
		if(sw->m_id == ui->MWidgetStudent1->m_id)
			MinInx = i;
		else if(sw->m_id == ui->MWidgetStudent2->m_id)
			MinInx = i;
		else if(sw->m_id == ui->MWidgetStudent3->m_id)
			MinInx = i;
		else if(sw->m_id == ui->MWidgetStudent4->m_id)
			MinInx = i;
	}

	QList<int> ListLeftId;
	QList<int> ListRightId;
	for(int i = 0;i < ui->VLayoutVip->count();i++)
	{
		StudentWidget *sw = (StudentWidget *)ui->VLayoutVip->itemAt(i)->widget();
		if(sw->HaveVideoStream == true || sw->AudioStatus == 1)
		{
			if(i < MinInx)
				ListLeftId.append(sw->m_id);
			else
				ListRightId.append(sw->m_id);
		}
	}
	QList<int> ListId;
	for(int i = 0;i < ListRightId.count();i++)
	{
		ListId.append(ListRightId.at(i));
	}
	for(int i = 0;i < ListLeftId.count();i++)
	{
		ListId.append(ListLeftId.at(i));
	}

	if(ListId.count() > 4)
    {
        if(ui->MWidgetStudent1->m_id != -1)
        {
            StudentWidget *sw = GetStudentWidget(ui->MWidgetStudent1->m_id);
            if(sw->VideoStatus == 1)
            {
                slot_K12MCU2UI_CLOSE_MEDIA(sw->m_id,COMMON_VIDEO);
				sw->HaveVideoStream = true;
            }
            ui->MWidgetStudent1->ClearAll();
        }
        if(ui->MWidgetStudent2->m_id != -1)
        {
            StudentWidget *sw = GetStudentWidget(ui->MWidgetStudent2->m_id);
            if(sw->VideoStatus == 1)
            {
                slot_K12MCU2UI_CLOSE_MEDIA(sw->m_id,COMMON_VIDEO);
				sw->HaveVideoStream = true;
            }
            ui->MWidgetStudent2->ClearAll();
        }
        if(ui->MWidgetStudent3->m_id != -1)
        {
            StudentWidget *sw = GetStudentWidget(ui->MWidgetStudent3->m_id);
            if(sw->VideoStatus == 1)
            {
                slot_K12MCU2UI_CLOSE_MEDIA(sw->m_id,COMMON_VIDEO);
				sw->HaveVideoStream = true;
            }
            ui->MWidgetStudent3->ClearAll();
        }
        if(ui->MWidgetStudent4->m_id != -1)
        {
            StudentWidget *sw = GetStudentWidget(ui->MWidgetStudent4->m_id);
            if(sw->VideoStatus == 1)
            {
                slot_K12MCU2UI_CLOSE_MEDIA(sw->m_id,COMMON_VIDEO);
				sw->HaveVideoStream = true;
            }
            ui->MWidgetStudent4->ClearAll();
        }

		for(int i = ListId.count() - 4;i < ListId.count();i++)
		{
			StudentWidget *sw = GetStudentWidget(ListId.at(i));
			if(sw->HaveVideoStream)
				slot_K12MCU2UI_NEW_M_COMING(ListId.at(i),COMMON_VIDEO);
			if(sw->AudioStatus == 1)
				slot_K12MCU2UI_NEW_M_COMING(ListId.at(i),COMMON_AUDIO);
		}
	}
}

void ClassroomDialog::InsertMedia()
{
	if(GetMediaWidgetVipNull() == nullptr)
		return ;

	int CanCount = 0;
	for(int i = 0;i < ui->VLayoutVip->count();i++)
	{
		StudentWidget *sw = (StudentWidget *)ui->VLayoutVip->itemAt(i)->widget();
		if(MediaWStudentC->m_id == sw->m_id)
			continue;
		if((sw->HaveVideoStream == true || sw->AudioStatus == 1))
			CanCount++;
	}
	switch(CanCount)
	{
	case 3:
		if(ui->MWidgetStudent1->m_id != -1 && ui->MWidgetStudent2->m_id != -1 && ui->MWidgetStudent3->m_id != -1)
			return ;
		break;
	case 2:
		if(ui->MWidgetStudent1->m_id != -1 && ui->MWidgetStudent2->m_id != -1)
			return ;
		break;
	case 1:
		if(ui->MWidgetStudent1->m_id != -1)
			return ;
		break;
	case 0:
		return ;
		break;
	default:
		break;
	}

	QList<int> ListMediaId;
	if(ui->MWidgetStudent1->m_id != -1)
		ListMediaId.append(ui->MWidgetStudent1->m_id);
	if(ui->MWidgetStudent2->m_id != -1)
		ListMediaId.append(ui->MWidgetStudent2->m_id);
	if(ui->MWidgetStudent3->m_id != -1)
		ListMediaId.append(ui->MWidgetStudent3->m_id);
	if(ui->MWidgetStudent4->m_id != -1)
		ListMediaId.append(ui->MWidgetStudent4->m_id);

	for(int i = 0;i < ui->VLayoutVip->count();i++)
	{
		StudentWidget *sw = (StudentWidget *)ui->VLayoutVip->itemAt(i)->widget();
		if((sw->HaveVideoStream == true || sw->AudioStatus == 1) && MediaWStudentC->m_id != sw->m_id &&
				ListMediaId.contains(sw->m_id) == false)
			ListMediaId.append(sw->m_id);
	}

	if(ui->MWidgetStudent1->m_id != -1)
	{
		StudentWidget *sw = GetStudentWidget(ui->MWidgetStudent1->m_id);
		if(sw->VideoStatus == 1)
		{
			slot_K12MCU2UI_CLOSE_MEDIA(sw->m_id,COMMON_VIDEO);
			sw->HaveVideoStream = true;
		}
		ui->MWidgetStudent1->ClearAll();
	}
	if(ui->MWidgetStudent2->m_id != -1)
	{
		StudentWidget *sw = GetStudentWidget(ui->MWidgetStudent2->m_id);
		if(sw->VideoStatus == 1)
		{
			slot_K12MCU2UI_CLOSE_MEDIA(sw->m_id,COMMON_VIDEO);
			sw->HaveVideoStream = true;
		}
		ui->MWidgetStudent2->ClearAll();
	}
	if(ui->MWidgetStudent3->m_id != -1)
	{
		StudentWidget *sw = GetStudentWidget(ui->MWidgetStudent3->m_id);
		if(sw->VideoStatus == 1)
		{
			slot_K12MCU2UI_CLOSE_MEDIA(sw->m_id,COMMON_VIDEO);
			sw->HaveVideoStream = true;
		}
		ui->MWidgetStudent3->ClearAll();
	}
	if(ui->MWidgetStudent4->m_id != -1)
	{
		StudentWidget *sw = GetStudentWidget(ui->MWidgetStudent4->m_id);
		if(sw->VideoStatus == 1)
		{
			slot_K12MCU2UI_CLOSE_MEDIA(sw->m_id,COMMON_VIDEO);
			sw->HaveVideoStream = true;
		}
		ui->MWidgetStudent4->ClearAll();
	}

	int OnCount = 0;
	for(int i = 0;i < ListMediaId.count();i++)
	{
		StudentWidget *sw = GetStudentWidget(ListMediaId.at(i));
		if(sw->HaveVideoStream)
			slot_K12MCU2UI_NEW_M_COMING(ListMediaId.at(i),COMMON_VIDEO);
		if(sw->AudioStatus == 1)
			slot_K12MCU2UI_NEW_M_COMING(ListMediaId.at(i),COMMON_AUDIO);
		OnCount++;
		if(OnCount >= 4)
			break;
	}
}

void ClassroomDialog::slot_K12MCU2UI_GET_USER_LIST(user_media_t *user, quint32 cnt)
{
    qDebug() << "ClassroomDialog::slot_K12MCU2UI_GET_USER_LIST:cnt=" << cnt;
    for(quint32 i=0; i<cnt; i++)
    {
        slot_K12MCU2UI_USER_ENTER_ROOM(user[i]);
    }
    delete [] user;
    user = nullptr;
}

void ClassroomDialog::slot_K12MCU2UI_USER_ENTER_ROOM(user_media_t sct)
{
    qDebug() << "ClassroomDialog::slot_K12MCU2UI_USER_ENTER_ROOM:sct.userid=" << sct.userid;
    if(sct.userid > 0 && sct.userid != Global::g_LoginerId)
    {
        NAManager *nam = new NAManager();
        connect(nam,SIGNAL(signal_reply_finished(QJsonObject,void*)),this,SLOT(slot_GetEnterUser(QJsonObject,void*)));
        QByteArray ba = QCryptographicHash::hash(QString("criId=%1&uiId=%2&key=AAS").arg(Global::g_CurRoomId).arg(sct.userid).toUtf8(),QCryptographicHash::Md5);
        QString md5(ba.toHex().toUpper());
        QJsonObject json;
        json.insert("criId", QString::number(Global::g_CurRoomId));
        json.insert("uiId", QString::number(sct.userid));
        json.insert("sign", md5);
        QString strParam = QString("service=user&method=queryDetail&params=%1").arg(QString(QJsonDocument(json).toJson(QJsonDocument::Compact)));
        user_media_t *pUser_media = new user_media_t;
        *pUser_media = sct;
        nam->SendRequest(QString("http://%1/aas/service/sys/execute").arg(Global::g_ServerIp),strParam,Post,(void *)pUser_media);
    }
}

void ClassroomDialog::slot_GetEnterUser(QJsonObject obj, void *pUserData)
{
    qDebug() << "ClassroomDialog::slot_GetEnterUser:obj=" << obj;
    if(obj.isEmpty())
    {
        Global::g_NoticeDialog->init("获取新进入教室用户信息失败！","","",2000);
        Global::g_NoticeDialog->exec();
        return ;
    }
    if (obj["msg"].toString() != "")
    {
        Global::g_NoticeDialog->init(obj["msg"].toString(),"","",2000);
        Global::g_NoticeDialog->exec();
        return ;
    }

	ui->PButtonStudentList->setChecked(true);
    slot_PButtonStudentList_clicked();

    QJsonObject body = obj["body"].toObject();
    if(body.isEmpty())
        return ;
    int UserId = body["id"].toInt();
    int UserType = 1;//1 VIP，2 旁听
    QJsonObject courseJoinInfo_1 = body["courseJoinInfo"].toObject();
    QJsonObject courseJoinInfo_2 = courseJoinInfo_1["courseJoinInfo"].toObject();
    if(courseJoinInfo_2.isEmpty() == false)
        UserType = courseJoinInfo_2["type"].toInt();
    if(GetStudentWidget(UserId) || GetStudentWidget(UserId,2))
        slot_K12MCU2UI_USER_LEAVE_ROOM(UserId);
    StudentWidget *sw = new StudentWidget(UserId,UserType,this);
    connect(sw,SIGNAL(signal_VideoOpen(int)),this,SLOT(slot_VideoOpen(int)));
    connect(sw,SIGNAL(signal_VideoClose(int)),this,SLOT(slot_VideoClose(int)));
    connect(sw,SIGNAL(signal_AudioOpen(int)),this,SLOT(slot_AudioOpen(int)));
    connect(sw,SIGNAL(signal_AudioClose(int)),this,SLOT(slot_AudioClose(int)));
    user_media_t *user_media_sct = (user_media_t *)pUserData;
    uint32_t iDeviceStatus = user_media_sct->t;
    if((iDeviceStatus & COMMON_VIDEO) == false)
        sw->VideoStatus = 2;
    if((iDeviceStatus & COMMON_AUDIO) == false)
        sw->AudioStatus = 2;
    delete pUserData;
    pUserData = nullptr;
    if(UserType == 2)
    {
        ui->VLayoutAuditor->addWidget(sw);
        ui->PButtonAuditor->setChecked(true);
        ui->SWidgetStudent->setCurrentWidget(ui->PageAuditor);
        ui->SAreaVip->verticalScrollBar()->setSliderPosition(0);
    }
    else
    {
        ui->VLayoutVip->addWidget(sw);
        ui->PButtonVip->setChecked(true);
        ui->SWidgetStudent->setCurrentWidget(ui->PageVip);
        ui->SAreaAuditor->verticalScrollBar()->setSliderPosition(0);
        if(MapNeedVideo.contains(UserId))
        {
            slot_K12MCU2UI_NEW_M_COMING(UserId,MapNeedVideo.value(UserId));
            MapNeedVideo.remove(UserId);
        }
        if(MapOnlyAudio.contains(UserId))
        {
            if(MapOnlyAudio.value(UserId) == 1)
                sw->VideoStatus = 3;
            MapOnlyAudio.remove(UserId);
        }
    }
    ui->SAWidgetVip->setFixedHeight(ui->VLayoutVip->count()*100);
    ui->SAWidgetAuditor->setFixedHeight(ui->VLayoutAuditor->count()*100);

	if(ui->VLayoutVip->count() > 0)
		ui->SWidgetMedia->setCurrentWidget(ui->SWMPageSome);
}

void ClassroomDialog::slot_K12MCU2UI_USER_LEAVE_ROOM(int UserId)
{
    qDebug() << "ClassroomDialog::slot_K12MCU2UI_USER_LEAVE_ROOM:UserId=" << UserId;
    for(int i = 0;i < ui->VLayoutVip->count();i++)
    {
        StudentWidget *sw = (StudentWidget *)ui->VLayoutVip->itemAt(i)->widget();
        if(sw->m_id == UserId)
        {
            ui->VLayoutVip->removeWidget(sw);
            delete sw;
            sw = nullptr;
            break;
        }
    }
    for(int i = 0;i < ui->VLayoutAuditor->count();i++)
    {
        StudentWidget *sw = (StudentWidget *)ui->VLayoutAuditor->itemAt(i)->widget();
        if(sw->m_id == UserId)
        {
            ui->VLayoutAuditor->removeWidget(sw);
            delete sw;
            sw = nullptr;
            break;
        }
    }
    ui->SAWidgetVip->setFixedHeight(ui->VLayoutVip->count()*100);
    ui->SAWidgetAuditor->setFixedHeight(ui->VLayoutAuditor->count()*100);

	if(ui->VLayoutVip->count() <= 0)
		ui->SWidgetMedia->setCurrentWidget(ui->SWMPageNone);
}

void ClassroomDialog::slot_PButtonStudentList_clicked()
{
    ui->LabelListTitle->setText("学生列表");
    ui->SWidgetList->setCurrentWidget(ui->SWLPageStudent);
    ui->PButtonVip->setChecked(true);
    ui->SWidgetStudent->setCurrentWidget(ui->PageVip);
}

void ClassroomDialog::slot_PButtonVip_clicked(bool)
{
    ui->SWidgetStudent->setCurrentWidget(ui->PageVip);
}

void ClassroomDialog::slot_PButtonAuditor_clicked(bool)
{
    ui->SWidgetStudent->setCurrentWidget(ui->PageAuditor);
}

void ClassroomDialog::slot_SetGraffitiSizeFinished()
{
    ui->STWidgetBarrage1->move(25,0);
    ui->STWidgetBarrage1->setFixedWidth(MediaWTeacher->width()-25);
    ui->STWidgetBarrage2->move(MediaWTeacher->width()+25,0);
    int lbw = (ui->WidgetGraffiti->width()-MediaWTeacher->width()-75-60)/2;
    ui->STWidgetBarrage2->setFixedWidth(lbw);
    ui->STWidgetBarrage3->move(MediaWTeacher->width()+25+lbw+25,0);
    ui->STWidgetBarrage3->setFixedWidth(lbw);
    if(m_CourWidShare)
    {
        QString documentFilename = QFileInfo(m_CourWidShare->m_strDocFileFullName).completeBaseName();
        QString cfilename = Global::g_AppDataPath + "courseware_transform/" + QString::number(Global::g_CurRoomId) + "/" +
                documentFilename + "/" + documentFilename + QString("_%1.png").arg(m_iCurSharingPageIndex);
        QPixmap pix = QPixmap(cfilename);
        if(pix.isNull())
            return ;
        int rectW = 0;
        int rectH = 0;
        if(pix.width() <= m_SizeGraffiti.width() && pix.height() <= m_SizeGraffiti.height())
        {
            rectW = pix.width();
            rectH = pix.height();
        }
        else
        {
            float imgBILI = (float)pix.width() / (float)pix.height();
            if(m_SizeGraffiti.height() * imgBILI < m_SizeGraffiti.width())
            {
                rectH = m_SizeGraffiti.height();
                rectW = rectH * imgBILI;
            }
            else
            {
                rectW = m_SizeGraffiti.width();
                rectH = rectW / imgBILI;
            }
        }
        pix = pix.scaled(rectW,rectH,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
        m_RectDocument = QRect((m_SizeGraffiti.width()-rectW)/2,(m_SizeGraffiti.height()-rectH)/2,rectW,rectH);
        ui->WidgetGraffiti->ChangePixmap(pix);
    }
}

void ClassroomDialog::slot_PButtonMouseClickEffect_clicked()
{
    emit signal_CursorChanged(0);
}

void ClassroomDialog::slot_PButtonPencil_clicked()
{
    emit signal_CursorChanged(1);
}

void ClassroomDialog::slot_PButtonGlowSticks_clicked()
{
    emit signal_CursorChanged(2);
}

void ClassroomDialog::slot_PButtonFile_clicked()
{
    ui->LabelListTitle->setText("课件");
    ui->SWidgetList->setCurrentWidget(ui->SWLPageDocument);
}

void ClassroomDialog::slot_PButtonScreenClear_clicked()
{
    qDebug() << "ClassroomDialog::slot_PButtonScreenClear_clicked";
    if(m_TimerDocumentPlay.isActive())
        m_TimerDocumentPlay.stop();
    if(m_CourWidShare)
    {
        m_CourWidShare->setStyleSheet("");
        m_CourWidShare = nullptr;
        QJsonObject json;
        json.insert("typeId", "308");
        json.insert("sendUserId", QString::number(Global::g_LoginerId));
        slot_sendMcuMsg(json);
    }
    ui->PButtonPrev->hide();
    ui->PButtonPlay->hide();
    ui->PButtonNext->hide();
    ui->WidgetGraffiti->ChangePixmap(QPixmap());
    if(MediaWStudentC->m_id != -1)
        slot_MediaDown(MediaWStudentC->m_id);
    MediaWTeacher->show();
}

void ClassroomDialog::slot_PButtonNext_clicked()
{
    qDebug() << "ClassroomDialog::slot_PButtonNext_clicked";
    m_iCurSharingPageIndex++;
    if(m_iCurSharingPageIndex > m_CourWidShare->m_iTotalPageCount)
    {
        m_iCurSharingPageIndex = m_CourWidShare->m_iTotalPageCount;
        Global::g_NoticeDialog->init("已经是最后一页！","","",2000);
        Global::g_NoticeDialog->exec();
        return ;
    }

    QJsonObject json;
    json.insert("typeId", "74");
    json.insert("curPage", m_iCurSharingPageIndex);
    QJsonObject rect;
    rect.insert("x", m_RectDocument.x());
    rect.insert("y", 0);
    rect.insert("width", m_RectDocument.width());
    rect.insert("height", m_RectDocument.height());
    json.insert("rect", rect);
    slot_sendMcuMsg(json);

    checkSharingFileExist(QFileInfo(m_CourWidShare->m_strDocFileFullName).completeBaseName(), m_iCurSharingPageIndex);
}

void ClassroomDialog::slot_PButtonPrev_clicked()
{
    qDebug() << "ClassroomDialog::slot_PButtonPrev_clicked";
    m_iCurSharingPageIndex--;
    if(m_iCurSharingPageIndex <= 0)
    {
        m_iCurSharingPageIndex = 1;
        Global::g_NoticeDialog->init("已经是第一页！","","",2000);
        Global::g_NoticeDialog->exec();
        return ;
    }

    QJsonObject json;
    json.insert("typeId", "74");
    json.insert("curPage", m_iCurSharingPageIndex);
    QJsonObject rect;
    rect.insert("x", m_RectDocument.x());
    rect.insert("y", 0);
    rect.insert("width", m_RectDocument.width());
    rect.insert("height", m_RectDocument.height());
    json.insert("rect", rect);
    slot_sendMcuMsg(json);

    checkSharingFileExist(QFileInfo(m_CourWidShare->m_strDocFileFullName).completeBaseName(), m_iCurSharingPageIndex);
}

void ClassroomDialog::slot_PButtonPlay_clicked(bool IsChecked)
{
    if(IsChecked)
    {
        ui->PButtonPrev->hide();
        ui->PButtonNext->hide();
        m_TimerDocumentPlay.start(10000);
    }
    else
    {
        ui->PButtonPrev->show();
        ui->PButtonNext->show();
        if(m_TimerDocumentPlay.isActive())
            m_TimerDocumentPlay.stop();
    }
}

void ClassroomDialog::slot_DocumentPlay()
{
    slot_PButtonNext_clicked();
    if(m_iCurSharingPageIndex == m_CourWidShare->m_iTotalPageCount)
    {
        Global::g_NoticeDialog->init("已经是最后一页！","","",2000);
        Global::g_NoticeDialog->exec();
        m_TimerDocumentPlay.stop();
    }
}

void ClassroomDialog::checkSharingFileExist(QString documentFilename, int pageindex)
{
    QString cfilename = Global::g_AppDataPath + "courseware_transform/" + QString::number(Global::g_CurRoomId) + "/" + documentFilename + "/" + documentFilename +
            QString("_%1.png").arg(pageindex);
    QPixmap pix = QPixmap(cfilename);
    if(pix.isNull())
    {
        Global::g_NoticeDialog->init("课件页读取失败！","","",2000);
        Global::g_NoticeDialog->exec();
        return ;
    }

    int rectW = 0;
    int rectH = 0;
    if(pix.width() <= m_SizeGraffiti.width() && pix.height() <= m_SizeGraffiti.height())
    {
        rectW = pix.width();
        rectH = pix.height();
    }
    else
    {
        float imgBILI = (float)pix.width() / (float)pix.height();
        if(m_SizeGraffiti.height() * imgBILI < m_SizeGraffiti.width())
        {
            rectH = m_SizeGraffiti.height();
            rectW = rectH * imgBILI;
        }
        else
        {
            rectW = m_SizeGraffiti.width();
            rectH = rectW / imgBILI;
        }
    }
    pix = pix.scaled(rectW,rectH,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
    m_RectDocument = QRect((m_SizeGraffiti.width()-rectW)/2,(m_SizeGraffiti.height()-rectH)/2,rectW,rectH);
    ui->WidgetGraffiti->ChangePixmap(pix);
}

void ClassroomDialog::slot_MediaUp(int UserId)
{
    qDebug() << "ClassroomDialog::slot_MediaUp:UserId=" << UserId;
    if(MediaWStudentC->m_id == UserId)
        return ;
    StudentWidget *sw = GetStudentWidget(UserId);
    if(sw == nullptr)
    {
        Global::g_NoticeDialog->init("该用户不是VIP或者不在教室！","","",2000);
        Global::g_NoticeDialog->exec();
        return ;
    }
    if(sw->VideoStatus == 3)
    {
        Global::g_NoticeDialog->init("该用户只听音频，不可以上台！","","",2000);
        Global::g_NoticeDialog->exec();
        return ;
    }

    if(MediaWStudentC->isHidden() == false)
        slot_MediaDown(MediaWStudentC->m_id);

    MediaWidget *mw = GetMediaWidget(UserId);
    if(mw)
        mw->ClearAll();

    MediaWTeacher->setGeometry(100,100,MediaWStudentC->width(),MediaWStudentC->height());
    MediaWTeacher->show();
    MediaWStudentC->ShowPButtonDown();
    MediaWStudentC->show();
    MediaWStudentC->m_id = UserId;
    MediaWStudentC->SetHeadName();
    if(sw->VideoStatus == 1)
        MediaWStudentC->SetVideoStatus(1);
    else
	{
		if(sw->HaveVideoStream == true)
			slot_K12MCU2UI_NEW_M_COMING(UserId,COMMON_VIDEO);
	}
    if(sw->AudioStatus == 1)
        MediaWStudentC->SetAudioStatus(1);
    else
        slot_AudioOpen(UserId);

    QJsonObject json;
    json.insert("typeId", "301");
    json.insert("content", "1");
    json.insert("sendUserId", QString::number(Global::g_LoginerId));
    json.insert("upUserId", QString::number(UserId));
	slot_sendMcuMsg(json);
}

void ClassroomDialog::slot_MediaDown(int UserId)
{
    qDebug() << "ClassroomDialog::slot_MediaDown:UserId=" << UserId;
    StudentWidget *sw = GetStudentWidget(UserId);
    if(sw == nullptr)
    {
        Global::g_NoticeDialog->init("该用户不是VIP或者不在教室！","","",2000);
        Global::g_NoticeDialog->exec();
        return ;
    }

	if(sw->AudioStatus == 1)
		slot_AudioClose(UserId);

    MediaWStudentC->ClearAll();
    MediaWidget *mw = GetMediaWidgetVipNull();
    if(mw)
    {
        mw->m_id = UserId;
        mw->SetHeadName();
        if(sw->VideoStatus == 1)
            mw->SetVideoStatus(1);
    }
    else
    {
		if(sw->VideoStatus == 1)
		{
			slot_K12MCU2UI_CLOSE_MEDIA(UserId,COMMON_VIDEO);
			sw->HaveVideoStream = true;
		}
    }
    MediaWTeacher->setGeometry(0,0,ui->MWidgetStudent1->width(),ui->MWidgetStudent1->height());
    if(m_CourWidShare)
        MediaWTeacher->hide();
    MediaWStudentC->hide();

    QJsonObject json;
    json.insert("typeId", "301");
    json.insert("content", "0");
    json.insert("sendUserId", QString::number(Global::g_LoginerId));
    json.insert("downUserId", QString::number(UserId));
    slot_sendMcuMsg(json);
}

void ClassroomDialog::slot_PButtonFullScreen_clicked(bool IsChecked)
{
    if(IsChecked)
    {
        ui->SWidgetMedia->hide();
        m_SizeGraffiti = ui->WidgetGraffiti->size();
        emit signal_SetGraffitiPixmapSize(m_SizeGraffiti);
    }
    else
    {
        if(ui->SWidgetMedia->isHidden())
        {
            ui->SWidgetMedia->show();
            m_SizeGraffiti = QSize(ui->WidgetGraffiti->width(),ui->SWidgetMedia->y()-40);
            emit signal_SetGraffitiPixmapSize(m_SizeGraffiti);
        }
    }
}

void ClassroomDialog::slot_PButtonHistoricalNewsList_clicked()
{
    ui->LabelListTitle->setText("学员弹幕");
    ui->SWidgetList->setCurrentWidget(ui->SWLPageBarrage);
}

void ClassroomDialog::slot_PButtonBarrageClose_clicked()
{
    if(ui->WidgetBarrage->isHidden())
    {
        ui->WidgetBarrage->show();
        ui->PButtonBarrageClose->setText("关闭弹幕");
    }
    else
    {
        ui->WidgetBarrage->hide();
        ui->PButtonBarrageClose->setText("打开弹幕");
    }
}

void ClassroomDialog::slot_PButtonScreenshotTool_clicked()
{
    ScreenshotDialog *sd = new ScreenshotDialog();
    connect(this,SIGNAL(signal_BeSqueezedOut()),sd,SLOT(reject()));
    connect(sd,SIGNAL(signal_screenshot(QRect)),this,SLOT(slot_screenshot(QRect)));
    sd->setGeometry(rect());
    sd->exec();
    delete sd;
    sd = nullptr;
}

void ClassroomDialog::slot_screenshot(QRect rt)
{
    if(m_AlbumId == -1)
    {
        Global::g_NoticeDialog->init("网络异常，截图相册不存在！","","",2000);
        Global::g_NoticeDialog->exec();
        return ;
    }

    QString pp = Global::g_AppDataPath + "screenshot_temp/";
    QDir dir(pp);
    if(!dir.exists())
        dir.mkpath(pp);
    QString ppp = pp + QDateTime::currentDateTime().toString("yyyyMMdd_hhmmsszzz") + ".png";
    grab(rt).save(ppp);

    NAManager *nam = new NAManager();
    connect(nam,SIGNAL(signal_reply_finished(QJsonObject,void*)),this,SLOT(slot_screenshot_upload(QJsonObject,void*)));
    nam->UploadPng(QString("http://%1/uds/upload").arg(Global::g_ServerIp),ppp);
}

void ClassroomDialog::slot_screenshot_upload(QJsonObject obj, void *)
{
    qDebug() << "ClassroomDialog::slot_screenshot_upload:obj=" << obj;
    if(obj.isEmpty())
    {
        Global::g_NoticeDialog->init("网络异常，上传截图失败！","","",2000);
        Global::g_NoticeDialog->exec();
        return ;
    }

    QString cpdImgName = obj["id"].toString();
    QString cpdImgAddr = obj["filename"].toString();

    NAManager *nam = new NAManager();
    connect(nam,SIGNAL(signal_reply_finished(QJsonObject,void*)),this,SLOT(slot_AlbumAddPicture(QJsonObject,void*)));
    QByteArray ba = QCryptographicHash::hash(QString("cpId=%3&cpdImgAddr=%1&cpdImgName=%2&cpdStatus=1&flag=1&key=RMS").
                                             arg(cpdImgAddr).arg(cpdImgName).arg(m_AlbumId).toUtf8(),QCryptographicHash::Md5);
    QString md5(ba.toHex().toUpper());
    QJsonObject json;
    json.insert("cpId", QString::number(m_AlbumId));
    json.insert("cpdImgAddr", cpdImgAddr);
    json.insert("cpdImgName", cpdImgName);
    json.insert("cpdStatus", QString::number(1));
    json.insert("flag", QString::number(1));
    json.insert("sign", md5);
    QString strParam = QString("service=classPhotos&method=saveOrUpdateDetail&params=%1").
            arg(QString(QJsonDocument(json).toJson(QJsonDocument::Compact)));
    nam->SendRequest(QString("http://%1/rms/service/sys/execute").arg(Global::g_ServerIp),strParam);
}

void ClassroomDialog::slot_PButtonGiftList_clicked()
{
    ui->LabelListTitle->setText("礼物列表");
    ui->SWidgetList->setCurrentWidget(ui->SWLPageGift);
}

void ClassroomDialog::slot_GetAlbumList(QJsonObject obj, void *)
{
    if(obj.isEmpty())
    {
        Global::g_NoticeDialog->init("网络异常，获取截图相册失败！","","",2000);
        Global::g_NoticeDialog->exec();
        return ;
    }
    if (obj["msg"].toString() != "")
    {
        Global::g_NoticeDialog->init(obj["msg"].toString(),"","",2000);
        Global::g_NoticeDialog->exec();
        return ;
    }

    QJsonArray rows = obj["rows"].toArray();
    if(rows.size() == 0)
    {
        NAManager *nam = new NAManager();
        connect(nam,SIGNAL(signal_reply_finished(QJsonObject,void*)),this,SLOT(slot_GetAlbumCreate(QJsonObject,void*)));
        QByteArray ba = QCryptographicHash::hash(QString("cpStatus=1&cpTitle=截图相册&criId=%1&flag=1&key=RMS").
                                                 arg(Global::g_CurRoomId).toUtf8(),QCryptographicHash::Md5);
        QString md5(ba.toHex().toUpper());
        QJsonObject json;
        json.insert("cpStatus", QString::number(1));
        json.insert("cpTitle", "截图相册");
        json.insert("criId", QString::number(Global::g_CurRoomId));
        json.insert("flag", QString::number(1));
        json.insert("sign", md5);
        QString strParam = QString("service=classPhotos&method=saveOrUpdate&params=%1").
                arg(QString(QJsonDocument(json).toJson(QJsonDocument::Compact)));
        nam->SendRequest(QString("http://%1/rms/service/sys/execute").arg(Global::g_ServerIp),strParam);
        return ;
    }
    for(int i = 0;i < rows.size();i++)
    {
        QJsonObject jor = rows.at(i).toObject();
        if(jor["cpTitle"].toString() == "截图相册")
        {
            m_AlbumId = jor["id"].toInt();
            QJsonArray cpdl = jor["classroomPhotosDetailList"].toArray();
            for(int j = 0;j < cpdl.size();j++)
            {
                QJsonObject joc = cpdl.at(j).toObject();
                QString cpdImgAddr = joc["cpdImgAddr"].toString();
                QString cpdCreateTime = joc["cpdCreateTime"].toString();
                ScreenshotWidget *sw = new ScreenshotWidget(cpdImgAddr,cpdCreateTime,this);
                ui->VLayoutAlbum->addWidget(sw);
                ui->SAWidgetAlbum->setFixedHeight(ui->VLayoutAlbum->count()*200);
            }
            break;
        }
    }
}

void ClassroomDialog::slot_GetAlbumCreate(QJsonObject obj, void *)
{
    qDebug() << "ClassroomDialog::slot_GetAlbumCreate:obj=" << obj;
    if(obj.isEmpty())
    {
        Global::g_NoticeDialog->init("网络异常，创建截图相册失败！","","",2000);
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
    m_AlbumId = body["id"].toInt();
}

void ClassroomDialog::slot_AlbumAddPicture(QJsonObject obj, void *)
{
    qDebug() << "ClassroomDialog::slot_AlbumAddPicture:obj=" << obj;
    if(obj.isEmpty())
    {
        Global::g_NoticeDialog->init("网络异常，截图相册新增截图失败！","","",2000);
        Global::g_NoticeDialog->exec();
        return ;
    }
    if (obj["msg"].toString() != "")
    {
        Global::g_NoticeDialog->init(obj["msg"].toString(),"","",2000);
        Global::g_NoticeDialog->exec();
        return ;
    }

    QString cpdImgAddr = obj["cpdImgAddr"].toString();
    QString cpdCreateTime = obj["cpdCreateTime"].toString();
    ScreenshotWidget *sw = new ScreenshotWidget(cpdImgAddr,cpdCreateTime,this);
    ui->VLayoutAlbum->insertWidget(0,sw);
    ui->SAWidgetAlbum->setFixedHeight(ui->VLayoutAlbum->count()*200);
    ui->SAreaAlbum->verticalScrollBar()->setSliderPosition(0);
    slot_PButtonScreenshotAlbum_clicked();
}

void ClassroomDialog::slot_PButtonScreenshotAlbum_clicked()
{
    ui->LabelListTitle->setText("截图相册");
    ui->SWidgetList->setCurrentWidget(ui->SWLPageAlbum);
}

void ClassroomDialog::slot_GetCourseware(QJsonObject obj, void *)
{
    if(obj.isEmpty())
    {
        Global::g_NoticeDialog->init("网络异常，获取历史课件信息失败！","","",2000);
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
    if(body.isEmpty())
        return ;
    QJsonArray rows = body["rows"].toArray();
    for(int i = 0;i < rows.size();i++)
    {
        QJsonObject jor = rows.at(i).toObject();
        QString cwi_filepath = jor["cwi_filepath"].toString();
        QString ci_name = jor["cwi_name"].toString();
        int id = jor["id"].toInt();
        CoursewareWidget* cw = new CoursewareWidget(cwi_filepath,ci_name,id,this);
        connect(cw,SIGNAL(signal_ShareCur(CoursewareWidget*)),this,SLOT(slot_ShareDocumentCur(CoursewareWidget*)));
        ui->VLayoutDocument->addWidget(cw);
    }
    ui->SAWidgetDocument->setFixedHeight(ui->VLayoutDocument->count()*40);
}

void ClassroomDialog::slot_ShareDocumentCur(CoursewareWidget *cw)
{
    qDebug() << "ClassroomDialog::slot_ShareDocumentCur";
    if(m_CourWidShare != nullptr)
        m_CourWidShare->setStyleSheet("");
    m_CourWidShare = cw;
    m_CourWidShare->setStyleSheet("QWidget{background-color: rgb(209, 223, 255);}");

    QJsonObject json;
    json.insert("typeId", "72");
    json.insert("id", m_CourWidShare->m_CoursewareId);
    json.insert("fileType", "3");
    json.insert("fileName", QFileInfo(m_CourWidShare->m_strDocFileFullName).completeBaseName());
    json.insert("fileUploaderUserId", Global::g_LoginerId);
    json.insert("curRotateAngle", 0);
    json.insert("curPage", 1);
    json.insert("totalPageCount", m_CourWidShare->m_iTotalPageCount);
    json.insert("picDispMode", 0);
    slot_sendMcuMsg(json);//广播消息（不含自己）

    MediaWTeacher->hide();
    ui->PButtonPrev->show();
    ui->PButtonPlay->show();
    ui->PButtonNext->show();

    m_iCurSharingPageIndex = 1;
    checkSharingFileExist(QFileInfo(m_CourWidShare->m_strDocFileFullName).completeBaseName(), m_iCurSharingPageIndex);
}

void ClassroomDialog::slot_GetGiftList(QJsonObject obj, void *)
{
    if(obj.isEmpty())
    {
        Global::g_NoticeDialog->init("网络异常，获取礼物列表失败！","","",2000);
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
    for(int j = 0;j < rows.size();j++)
    {
        QJsonParseError jpe;
        QJsonDocument jd = QJsonDocument::fromJson(rows.at(j).toString().toUtf8(), &jpe);
        if (jpe.error != QJsonParseError::NoError)
            continue;
        if (jd.isObject() == false)
            continue;
        QJsonObject jojo = jd.object();
        QJsonObject sendUser = jojo["sendUser"].toObject();
        QString SenderIcon = sendUser["imagePath"].toString();
        if(SenderIcon.contains("/") == false)
            SenderIcon = "/" + SenderIcon;
        QString nickName = sendUser["nickName"].toString();
        QJsonObject giftSendDetail = jojo["giftSendDetail"].toObject();
        double payMoney = giftSendDetail["payMoney"].toDouble();
        GiftWidget *gw = new GiftWidget(SenderIcon,nickName,payMoney,this);
        ui->VLayoutGift->addWidget(gw);
        ui->SAWidgetGift->setFixedHeight(ui->VLayoutGift->count()*70);
    }
}

void ClassroomDialog::slot_GetBarrageList(QJsonObject obj, void *)
{
    if(obj.isEmpty())
    {
        Global::g_NoticeDialog->init("网络异常，获取弹幕列表失败！","","",2000);
        Global::g_NoticeDialog->exec();
        return ;
    }
    if (obj["msg"].toString() != "")
    {
        Global::g_NoticeDialog->init(obj["msg"].toString(),"","",2000);
        Global::g_NoticeDialog->exec();
        return ;
    }

    QJsonArray arr = obj["rows"].toArray();
    for(int j = 0;j < arr.size();j++)
    {
        QJsonObject jobj = arr.at(j).toObject();
        int sendUserId = jobj["uiId"].toInt();
        QString sendUsername = jobj["uiNickname"].toString();
        QString msgSendTime = jobj["cpCreateTime"].toString();
        QString content = jobj["ccrContent"].toString();
        BarrageWidget *bw = new BarrageWidget(sendUserId,sendUsername,msgSendTime,content,this);
        ui->VLayoutBarrage->addWidget(bw);
        ui->SAWidgetBarrage->setFixedHeight(ui->VLayoutBarrage->count()*110);
    }
}

void ClassroomDialog::ChangeGraffitiBarrage()
{
	QLayoutItem *item = ui->VLayoutBarrage->itemAt(0);
	BarrageWidget *bw =(BarrageWidget *)item->widget();
	if(MediaWTeacher->isHidden())
	{
		if(ui->STWidgetBarrage3->ListString.count() <= ui->STWidgetBarrage2->ListString.count() &&
				ui->STWidgetBarrage3->ListString.count() <= ui->STWidgetBarrage1->ListString.count())
			ui->STWidgetBarrage3->setNextSlideText(bw->GetContent());
		else if(ui->STWidgetBarrage2->ListString.count() <= ui->STWidgetBarrage1->ListString.count())
			ui->STWidgetBarrage2->setNextSlideText(bw->GetContent());
		else
			ui->STWidgetBarrage1->setNextSlideText(bw->GetContent());
	}
	else
	{
		if(ui->STWidgetBarrage3->ListString.count() <= ui->STWidgetBarrage2->ListString.count())
			ui->STWidgetBarrage3->setNextSlideText(bw->GetContent());
		else
			ui->STWidgetBarrage2->setNextSlideText(bw->GetContent());
	}
}

void ClassroomDialog::slot_TeacherDialog_close()
{
    qDebug() << "ClassroomDialog::slot_TeacherDialog_close";
    m_PduStatus = 3;
	CloseClassroom();
}

void ClassroomDialog::slot_PButtonDeviceDetection_clicked()
{
    ui->LabelListTitle->setText("设置");
    ui->SWidgetList->setCurrentWidget(ui->SWLPageDevice);
}

void ClassroomDialog::slot_PButtonDevice_clicked(bool isChecked)
{
    if(isChecked)
    {
        ui->PButtonDevice->setChecked(true);
        ui->PButtonSystem->setChecked(false);
        ui->SWidgetCheck->setCurrentWidget(ui->PageDevice);
    }
}

void ClassroomDialog::slot_PButtonSystem_clicked(bool isChecked)
{
    if(isChecked)
    {
        ui->PButtonDevice->setChecked(false);
        ui->PButtonSystem->setChecked(true);
        ui->SWidgetCheck->setCurrentWidget(ui->PageSystem);
    }
}

void ClassroomDialog::slot_p2p_msg(quint32, const char *msg, quint32)
{
    QJsonParseError json_error;
    QJsonDocument parse_doucment = QJsonDocument::fromJson(QString(msg).toUtf8(), &json_error);
    delete msg;
    msg = nullptr;
    if(json_error.error != QJsonParseError::NoError)
    {
        qDebug() << "ClassroomDialog::slot_p2p_msg:json_error.error=" << json_error.error;
        return;
    }
    if(parse_doucment.isObject())
        slot_McuMsgArrived(parse_doucment.object());
    else
        qDebug() << "ClassroomDialog::slot_p2p_msg:parse_doucment.isObject()=false";
}

void ClassroomDialog::slot_p2all_msg(quint32, const char *msg, quint32)
{
    QJsonParseError json_error;
    QJsonDocument parse_doucment = QJsonDocument::fromJson(QString(msg).toUtf8(), &json_error);
    delete msg;
    msg = nullptr;
    if(json_error.error != QJsonParseError::NoError)
    {
        qDebug() << "ClassroomDialog::slot_p2p_msg:json_error.error=" << json_error.error;
        return;
    }
    if(parse_doucment.isObject())
        slot_McuMsgArrived(parse_doucment.object());
    else
        qDebug() << "ClassroomDialog::slot_p2p_msg:parse_doucment.isObject()=false";
}

void ClassroomDialog::slot_recieved_msg(QJsonObject obj)
{
    qDebug() << "ClassroomDialog::slot_recieved_msg:obj=" << obj;
    int typeId = obj["typeId"].toString().toInt();
    switch(typeId)
    {
	case 0://新增弹幕
    {
        int sendUserId = -1;
        if(obj["sendUserId"].isString())
            sendUserId = obj["sendUserId"].toString().toInt();
        else
            sendUserId = obj["sendUserId"].toInt();
        QString sendUsername = obj["sendUsername"].toString();
        QString msgSendTime = obj["msgSendTime"].toString();
        QString content = obj["content"].toString();
        BarrageWidget *bw = new BarrageWidget(sendUserId,sendUsername,msgSendTime,content,this);
        ui->VLayoutBarrage->insertWidget(0,bw);
        ui->SAWidgetBarrage->setFixedHeight(ui->VLayoutBarrage->count()*110);
        ui->SAreaBarrage->verticalScrollBar()->setSliderPosition(0);
		ui->PButtonHistoricalNewsList->setChecked(true);
        slot_PButtonHistoricalNewsList_clicked();
        ChangeGraffitiBarrage();
    }
        break;
	case 50://新增礼物
    {
        QString sendUserImg = obj["sendUserImg"].toString();
        if(sendUserImg.contains("/") == false)
            sendUserImg = "/" + sendUserImg;
        QString sendUsername = obj["sendUsername"].toString();
        double payMoney = obj["giftName"].toString().toDouble();
        GiftWidget *gw = new GiftWidget(sendUserImg,sendUsername,payMoney,this);
        ui->VLayoutGift->insertWidget(0,gw);
        ui->SAWidgetGift->setFixedHeight(ui->VLayoutGift->count()*70);
        ui->scrollArea->verticalScrollBar()->setSliderPosition(0);
		ui->PButtonGiftList->setChecked(true);
        slot_PButtonGiftList_clicked();
    }
        break;
    default:
        break;
    }
}

void ClassroomDialog::slot_McuMsgArrived(QJsonObject obj)
{
    qDebug() << "ClassroomDialog::slot_McuMsgArrived:obj=" << obj;
    if(obj.isEmpty())
        return ;

    int iMsgType = -1;
    if(obj["typeId"].isString())
        iMsgType = obj["typeId"].toString().toInt();
    else
        iMsgType = obj["typeId"].toInt();
    int iSendUserId = -1;
    if(obj["sendUserId"].isString())
        iSendUserId = obj["sendUserId"].toString().toInt();
    else
        iSendUserId = obj["sendUserId"].toInt();
    switch(iMsgType)
    {
    case 30://学生请求上下台
    {
        int content = obj["content"].toString().toInt();
        if(content == 1 && MediaWStudentC->m_id == iSendUserId){}
        else
            m_uddw->add_udw(iSendUserId,obj["sendUsername"].toString(),content);
    }
        break;
    case 71://学生请求历史教案记录
        if(m_CourWidShare)
        {
            QJsonObject json;
            json.insert("typeId", "72");
            json.insert("id", m_CourWidShare->m_CoursewareId);
            json.insert("fileType", "3");
            json.insert("fileName", QFileInfo(m_CourWidShare->m_strDocFileFullName).completeBaseName());
            json.insert("fileUploaderUserId", Global::g_LoginerId);
            json.insert("curRotateAngle", 0);
            json.insert("curPage", m_iCurSharingPageIndex);
            json.insert("totalPageCount", m_CourWidShare->m_iTotalPageCount);
            json.insert("picDispMode", 0);
            slot_sendMcuMsg(json,QString::number(iSendUserId));
        }
        break;
    case 108://学生请求历史板书记录
        if(ui->WidgetGraffiti->ListShapePoints.count() > 0)
        {
            QJsonObject pen;
            pen.insert("typeId", 111);
            pen.insert("sendUserId", Global::g_LoginerId);
			pen.insert("penShapeType", 1);
            QJsonObject penColor;
            penColor.insert("r", ui->WidgetGraffiti->m_Pen.color().red());
            penColor.insert("g", ui->WidgetGraffiti->m_Pen.color().green());
            penColor.insert("b", ui->WidgetGraffiti->m_Pen.color().blue());
            penColor.insert("a", ui->WidgetGraffiti->m_Pen.color().alpha());
            pen.insert("penColor", penColor);
            pen.insert("penWidth", ui->WidgetGraffiti->m_Pen.width());
            slot_sendMcuMsg(pen, QString::number(iSendUserId));
            QJsonArray arrShapes = ui->WidgetGraffiti->getCurShapes();
            if(arrShapes.size() == 0)
            {
                QJsonObject json;
                json.insert("typeId", 109);
                json.insert("sendUserId", Global::g_LoginerId);
                json.insert("content", "");
                slot_sendMcuMsg(json, QString::number(iSendUserId));
            }
            else
            {
                for(int i=0; i < arrShapes.size(); i++)
                {
                    QJsonObject one;
                    one.insert("typeId", 109);
                    one.insert("sendUserId", Global::g_LoginerId);
                    one.insert("content", arrShapes.at(i));
                    slot_sendMcuMsg(one, QString::number(iSendUserId));
                }
            }
        }
        else
        {
            QJsonObject json;
            json.insert("typeId", 109);
            json.insert("sendUserId", Global::g_LoginerId);
            json.insert("content", "-1");
            slot_sendMcuMsg(json, QString::number(iSendUserId));
        }
        break;
    case 303://学生发出从旁听升级到VIP的消息
    {
        StudentWidget *sw = GetStudentWidget(iSendUserId,2);
        if(sw)
        {
            StudentWidget *sw_temp = new StudentWidget(iSendUserId,1,this);
            ui->VLayoutVip->addWidget(sw_temp);
            connect(sw_temp,SIGNAL(signal_VideoOpen(int)),this,SLOT(slot_VideoOpen(int)));
            connect(sw_temp,SIGNAL(signal_VideoClose(int)),this,SLOT(slot_VideoClose(int)));
            connect(sw_temp,SIGNAL(signal_AudioOpen(int)),this,SLOT(slot_AudioOpen(int)));
            connect(sw_temp,SIGNAL(signal_AudioClose(int)),this,SLOT(slot_AudioClose(int)));
            sw_temp->VideoStatus = sw->VideoStatus;
            sw_temp->AudioStatus = sw->AudioStatus;
            for(int i = 0;i < ui->VLayoutAuditor->count();i++)
            {
                StudentWidget *sw_1 = (StudentWidget *)ui->VLayoutAuditor->itemAt(i)->widget();
                if(sw_1->m_id == iSendUserId)
                {
                    ui->VLayoutAuditor->removeWidget(sw_1);
                    delete sw_1;
                    sw_1 = nullptr;
                    break;
                }
            }
            ui->SAWidgetVip->setFixedHeight(ui->VLayoutVip->count()*100);
            ui->SAWidgetAuditor->setFixedHeight(ui->VLayoutAuditor->count()*100);
            if(MapNeedVideo.contains(iSendUserId))
            {
                slot_K12MCU2UI_NEW_M_COMING(iSendUserId,MapNeedVideo.value(iSendUserId));
                MapNeedVideo.remove(iSendUserId);
            }
			if(ui->VLayoutVip->count() > 0)
				ui->SWidgetMedia->setCurrentWidget(ui->SWMPageSome);
        }
    }
        break;
    case 304://学生请求当前正上台学生情况
        if(MediaWStudentC->m_id != -1)
        {
            QJsonObject json;
            json.insert("typeId", "305");
            json.insert("sendUserId", QString::number(Global::g_LoginerId));
            json.insert("MediaCenterUserId", QString::number(MediaWStudentC->m_id));
            slot_sendMcuMsg(json,QString::number(iSendUserId));
        }
        break;
    case 307://学生发过来只听音频请求
    {
        int content = obj["content"].toString().toInt();
        StudentWidget *sw = GetStudentWidget(iSendUserId);
        if(sw)
        {
            if(content == 1)//只听音频
				sw->VideoStatus = 3;
			else//取消只听音频
			{
				if(sw->IsTeacherCloseVideo == true)
					sw->VideoStatus = 0;
			}
        }
        else
            MapOnlyAudio.insert(iSendUserId,content);
    }
        break;
    default:
        break;
    }
}

void ClassroomDialog::slot_udw_arrive(UpDownWidget *udw)
{
    if(udw->m_UpOrDown == 1)
        slot_MediaUp(udw->m_senderId);
    else
        slot_MediaDown(udw->m_senderId);
    m_uddw->remove_udw(udw);
}

void ClassroomDialog::slot_TimerClassOver_timeout()
{
    int TimeCount = QDateTime::currentDateTime().secsTo(m_ClassBegin);
    if(TimeCount > 0)
    {
        if(TimeCount/60 > 0)
            ui->LabelClassState->setText("等待上课：" + QString::number(TimeCount/60) + "分" + QString::number(TimeCount%60) + "秒");
        else
            ui->LabelClassState->setText("等待上课：" + QString::number(TimeCount%60) + "秒");
    }
    else
        ui->LabelClassState->setText("正在上课……");

    TimeCount = QDateTime::currentDateTime().secsTo(m_ClassOver);
    if(TimeCount > 0 && TimeCount < 600)
    {
        m_LabelClassOver->setText("下课倒计时:" + QString::number(TimeCount) + "秒");
        m_LabelClassOver->adjustSize();
        m_LabelClassOver->move((width()-m_LabelClassOver->width())/2,0);
        m_LabelClassOver->show();
    }
    else if(TimeCount <= 0)
	{
		m_LabelClassOver->setText("上课结束");
		m_TimerClassOver.stop();
		CloseClassroom();
	}
}

void ClassroomDialog::slot_PButtonRecordOperate_clicked()
{
	qDebug() << "ClassroomDialog::slot_PButtonRecordOperate_clicked";
	if(m_CenterMedia == nullptr)
	{
		m_CenterMedia = center_media::create(Global::g_CurRoomId, Global::g_LoginerId,m_McuIp.toLatin1().data(), m_McuPort,V_F_1080P,true,m_dstip.toLatin1().data(),m_dstport,
											 record_msg_func, this);
		ui->PButtonRecordOperate->setEnabled(false);
		ui->LabelRecordTime->setText(QString("00:00:00/%1:%2:%3").arg(ui->PBarRecord->maximum()/3600,2,10,QLatin1Char('0')).
									 arg(ui->PBarRecord->maximum()%3600/60,2,10,QLatin1Char('0')).arg(ui->PBarRecord->maximum()%60,2,10,QLatin1Char('0')));
		m_iRecSeconds = 0;
		ui->PBarRecord->setValue(m_iRecSeconds);
	}
    else
	{
		m_TimerRecord.stop();
		m_CenterMedia->stop_record();
		ui->PButtonRecordOperate->setEnabled(false);
		m_LabelCloseWait->show();
		QString strName = QString("%1%2").arg(ui->LabelClassName->text()).arg(QDateTime::currentDateTime().toTime_t());
		QString strDescribe = "课件要点";
		QString str = QString("crcpId=%1,criId=%2,tiId=%3,crvName=%4,crvDesc=%5").arg(m_crcpId).arg(Global::g_CurRoomId).
				arg(Global::g_LoginerId).arg(strName.replace(",","，")).arg(strDescribe.replace(",","，"));
		std::string s = str.toStdString();
		m_CenterMedia->record_info(s.c_str(), s.size());
    }
}

void ClassroomDialog::record_msg(record_msg_t t, void *arg)
{
    int code = -1;
    if (arg)
        code = *((int *)arg);
    emit signal_record_msg(t, code);
}

void ClassroomDialog::slot_record_msg(int t,int arg)
{
	qDebug() << "ClassroomDialog::slot_record_msg:t=" << t << ",arg=" << arg;
	m_LabelCloseWait->hide();
	if(m_CenterMedia == nullptr)
		return ;
    switch (t)
    {
    case RECORD_CONNECTED:
        m_CenterMedia->start_record();
        m_TimerRecord.start(1000);
        break;
    case RECORD_UPDATED:
    case RECORD_UNCONNECTED:
    case RECORD_CONNECT_BREAK:
	default:
		delete m_CenterMedia;
		m_CenterMedia = nullptr;
		if(t == RECORD_UPDATED)
		{
			if(arg != 0)
				Global::g_NoticeDialog->init("录像上传失败！","","",2000);
			else
				Global::g_NoticeDialog->init("录像上传成功！","","",2000);
		}
		else if(t == RECORD_UNCONNECTED)
			Global::g_NoticeDialog->init("网络异常，连接录制服务器失败！","","",2000);
		else if(t == RECORD_CONNECT_BREAK)
			Global::g_NoticeDialog->init("网络异常，录像上传失败！","","",2000);
		Global::g_NoticeDialog->exec();
		if(m_PduStatus == 4)
		{
			m_PduStatus = 2;
			CloseClassroom();
		}
		return ;
    }
}

void ClassroomDialog::slot_TimerRecord_timeout()
{
	if(m_IsEight == false)
	{
		if(m_ClassEnter.secsTo(QDateTime::currentDateTime()) >= m_ClassEnter.secsTo(m_ClassOver)*80/100)
		{
			ui->PButtonRecordOperate->setEnabled(true);
			m_IsEight = true;
		}
	}
    m_iRecSeconds = m_iRecSeconds + 1;
    ui->PBarRecord->setValue(m_iRecSeconds);
    ui->LabelRecordTime->setText(QString("%1:%2:%3/%4:%5:%6").arg(m_iRecSeconds/3600,2,10,QLatin1Char('0'))
                                 .arg(m_iRecSeconds%3600/60,2,10,QLatin1Char('0')).arg(m_iRecSeconds%60,2,10,QLatin1Char('0'))
                                 .arg(ui->PBarRecord->maximum()/3600,2,10,QLatin1Char('0')).arg(ui->PBarRecord->maximum()%3600/60,2,10,QLatin1Char('0')).
                                 arg(ui->PBarRecord->maximum()%60,2,10,QLatin1Char('0')));
    QString ppp = Global::g_AppDataPath + "push_picture.jpg";
	ui->WidgetGraffiti->m_Pixmap.save(ppp);
//	ui->WidgetGraffiti->grab().save(ppp);
	QFile FileRecord(ppp);
	if(FileRecord.open(QIODevice::ReadOnly))
	{
		QByteArray ba = FileRecord.readAll();
		pdu->send_p2p_msg(0, ba.data(), ba.length());
		FileRecord.close();
	}
}

void ClassroomDialog::slot_CBoxChoiceCamera_activated(int inx)
{
    Global::g_Settings->setValue("conf/VideoDevSet",inx);

    slot_VideoClose(Global::g_LoginerId);

    media_dev_index_t mdit;
    mdit.index = inx;
    mdit.t = COMMON_VIDEO;
    pdu->msg(LIBPDU_SET_MEDIA_DEV_INDEX, &mdit);
}

void ClassroomDialog::slot_CBoxInputAudio_activated(int inx)
{
    Global::g_Settings->setValue("conf/AudioInDevSet",inx);

    slot_AudioClose(Global::g_LoginerId);

    media_dev_index_t mdit;
    mdit.index = inx;
    mdit.t = COMMON_AUDIO;
    pdu->msg(LIBPDU_SET_MEDIA_DEV_INDEX, &mdit);
}

void ClassroomDialog::slot_CBoxOutputAudio_activated(int inx)
{
    ui->ButtonPlayTestAudio->setChecked(false);
    slot_ButtonPlayTestAudio(false);
    Global::g_Settings->setValue("conf/AudioOutDevSet",inx);
    pdu->msg(LIBPDU_SET_PLAYOUT_INDEX, &inx);
}

bool ClassroomDialog::eventFilter(QObject *obj, QEvent *event)
{
    if(obj == ui->CBoxOutputAudio)
    {
        if(event->type() == QEvent::MouseButtonPress)
        {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
            if (mouseEvent->button() == Qt::LeftButton)
            {
                ui->CBoxOutputAudio->clear();
                int iSpeakerIndex = Global::g_Settings->value("conf/AudioOutDevSet").toInt();
                int iHeadphoneCount = pdu->msg(LIBPDU_GET_PLAYOUT_CNT, nullptr);
                if(iHeadphoneCount > 0)
                {
                    for (int i = 0;i < iHeadphoneCount;i++)
                    {
                        playout_name_t sct;
                        sct.index=i;
                        pdu->msg(LIBPDU_GET_PLAYOUT_NAME, &sct);
                        ui->CBoxOutputAudio->addItem(QString(sct.name));
                    }
                    if(iSpeakerIndex >= 0 || iSpeakerIndex < iHeadphoneCount)
                        ui->CBoxOutputAudio->setCurrentIndex(iSpeakerIndex);
                }
            }
        }
    }

    return QObject::eventFilter(obj, event);
}

void ClassroomDialog::slot_ButtonPlayTestAudio(bool bl)
{
    if(bl)
    {
        QString Mp3Path = "./testMusic.mp3";
        play = media_player::create(Mp3Path.toStdString().c_str(), this,Global::g_Settings->value("conf/AudioOutDevSet").toInt());
        if(play)
            play->play();
        else
        {
            Global::g_NoticeDialog->init("启动播放失败！","","",2000);
            Global::g_NoticeDialog->exec();
        }
    }
    else
    {
        if(play)
        {
            play->stop();
            delete play;
            play = nullptr;
        }
    }
}

void ClassroomDialog::slot_play_progress(int bytesSent, int bytesTotal)
{
    if(bytesSent >= bytesTotal)
    {
		if(play)
		{
			delete play;
			play = nullptr;
			ui->ButtonPlayTestAudio->setChecked(false);
		}
    }
}

int ClassroomDialog::msg(k12mcu2ui_type t, void *arg)
{
    switch(t)
    {
    case K12MCU2UI_CONNECT2MCU_STATE:
        emit signal_K12MCU2UI_CONNECT2MCU_STATE(*((int *)arg));
        break;
    case K12MCU2UI_DISCONNECT_FROM_MCU:
        emit signal_K12MCU2UI_DISCONNECT_FROM_MCU();
        break;
    case K12MCU2UI_NEW_M_COMING:
        emit signal_K12MCU2UI_NEW_M_COMING(((user_media_t *)arg)->userid, ((user_media_t *)arg)->t);
        break;
    case K12MCU2UI_VIDEO_DATA:
    {
        k12mcu2ui_video_data_t *sct = (k12mcu2ui_video_data_t *)arg;
        k12mcu2ui_video_data_t sctdata = *sct;
        queue_mutex.lock();
        video_datas.push(sctdata);
        if (video_datas.size() > 10)
        {
            k12mcu2ui_video_data_t fvd = video_datas.front();
            video_datas.pop();
            pdu->msg(LIBPDU_DESTORY_VIDEO_DATA, fvd.data);
        }
        queue_mutex.unlock();
        emit signal_K12MCU2UI_VIDEO_DATA();
    }
        break;
    case K12MCU2UI_CLOSE_MEDIA:
        emit signal_K12MCU2UI_CLOSE_MEDIA(((user_media_t *)arg)->userid, ((user_media_t *)arg)->t);
        break;
    case K12MCU2UI_GET_USER_LIST:
    {
        user_list_t* sct = (user_list_t *)arg;
        if (sct->cnt > 0)
        {
            user_media_t* pUserArray = new user_media_t[sct->cnt];
            memcpy(pUserArray, sct->user, sizeof(user_media_t)*sct->cnt);
            emit signal_K12MCU2UI_GET_USER_LIST(pUserArray, sct->cnt);
        }
    }
        break;
    case K12MCU2UI_USER_ENTER_ROOM:
        emit signal_K12MCU2UI_USER_ENTER_ROOM(*((user_media_t *)arg));
        break;
    case K12MCU2UI_USER_LEAVE_ROOM:
        emit signal_K12MCU2UI_USER_LEAVE_ROOM(*((uint32_t *)arg));
        break;
    default:
        break;
    }

    return 0;
}

void ClassroomDialog::ss_start_class(uint32_t)
{
}

void ClassroomDialog::start_class_ack(int code)
{
	emit signal_start_class_ack(code);
}

void ClassroomDialog::ss_stop_class(uint32_t)
{
}

void ClassroomDialog::stop_class_ack(int code)
{
	emit signal_stop_class_ack(code);
}

void ClassroomDialog::get_class_state_ack(int)
{
}

void ClassroomDialog::ss_application_share(uint32_t)
{
}

void ClassroomDialog::application_share_ack(int)
{
}

void ClassroomDialog::ss_cancel_application(uint32_t)
{
}

void ClassroomDialog::cancel_application_ack(int)
{
}

void ClassroomDialog::ss_reply_share(uint32_t, int)
{
}

void ClassroomDialog::reply_share_ack(uint32_t, int)
{
}

void ClassroomDialog::ss_start_share(uint32_t)
{
}

void ClassroomDialog::start_share_ack(int)
{
}

void ClassroomDialog::ss_stop_share(uint32_t)
{
}

void ClassroomDialog::stop_share_ack(int)
{
}

void ClassroomDialog::get_share_users_ack(int, int, uint32_t *)
{
}

void ClassroomDialog::ss_start_tag(uint32_t)
{
}

void ClassroomDialog::start_tag_ack(int)
{
}

void ClassroomDialog::ss_stop_tag(uint32_t)
{
}

void ClassroomDialog::stop_tag_ack(int)
{
}

void ClassroomDialog::get_tag_users_ack(int, int, uint32_t *)
{
}

void ClassroomDialog::ss_cancel_share(uint32_t)
{
}

void ClassroomDialog::cancel_share_ack(int)
{
}

void ClassroomDialog::p2p_msg(uint32_t src_uid, const char *con, uint32_t con_len)
{
    char *buf = new char[con_len+1];
    buf[con_len]=0;
    memcpy(buf, con, con_len);
    emit signal_p2p_msg(src_uid, buf, con_len);
}

void ClassroomDialog::p2all_msg(uint32_t src_uid, const char *con, uint32_t con_len)
{
    char *buf = new char[con_len+1];
    buf[con_len]=0;
    memcpy(buf, con, con_len);
    emit signal_p2all_msg(src_uid, buf, con_len);
}

void ClassroomDialog::get_classing_user_ack(int, uint32_t)
{
}

void ClassroomDialog::p2p_msg_ack(uint32_t, const char *, uint32_t)
{
}

void ClassroomDialog::p2all_msg_ack(uint32_t, const char *, uint32_t)
{
}

void ClassroomDialog::user_net_status(uint32_t,int)
{
}

void ClassroomDialog::recieved_msg(const char *recieved_msg, const char *, const char *)
{
    if(QString(recieved_msg) == "keepAlive")
        return ;
    QJsonParseError json_error;
    QJsonDocument parse_doucment = QJsonDocument::fromJson(QString(recieved_msg).toUtf8(), &json_error);
    if (json_error.error != QJsonParseError::NoError)
    {
        qDebug() << "ClassroomDialog::recieved_msg:json_error.error=" << json_error.error;
        return;
    }
    if (parse_doucment.isObject() == false)
    {
        qDebug() << "ClassroomDialog::recieved_msg:parse_doucment.isObject()=false";
        return;
    }
    emit signal_recieved_msg(parse_doucment.object());
}

void ClassroomDialog::connect2msgs_state(int)
{
}

void ClassroomDialog::disconnect_from_msgs()
{
}

void ClassroomDialog::play_progress(uint32_t bytesSent, uint32_t bytesTotal)
{
    emit signal_play_progress((int)bytesSent,(int)bytesTotal);
}

void ClassroomDialog::video_data(const uint8_t *,int,int)
{
}
