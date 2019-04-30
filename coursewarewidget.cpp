#include "coursewarewidget.h"
#include "ui_coursewarewidget.h"
#include <QMovie>
#include <QTimer>
#include "global.h"
#include <QMouseEvent>
#pragma execution_character_set("utf-8")

CoursewareWidget::CoursewareWidget(QString filepath,QString name,int CoursewareId,QWidget *parent) :
	QWidget(parent),
	ui(new Ui::CoursewareWidget)
{
	ui->setupUi(this);

	ui->RadioName->installEventFilter(this);
	QMovie *me = new QMovie(":/res/room/loading.gif", QByteArray(), this);
	ui->LabelLoading->setScaledContents(true);
	ui->LabelLoading->setMovie(me);
	me->start();
	ui->RadioName->setText(name);

	m_CoursewareId = CoursewareId;
	QString courseLocal = Global::g_AppDataPath + "courseware/" + QString::number(Global::g_CurRoomId) + "/";
	QDir dir(courseLocal);
	if(dir.exists() == false)
		dir.mkpath(courseLocal);
	m_strDocFileFullName = courseLocal + filepath;
	file = nullptr;
	NReply = nullptr;
	m_pFC_client = nullptr;
	m_strDocFileBaseName = QFileInfo(m_strDocFileFullName).completeBaseName();
	m_strConvertDstPath = Global::g_AppDataPath+"courseware_transform/"+QString::number(Global::g_CurRoomId)+"/"+
			m_strDocFileBaseName;
	QDir dir1(m_strConvertDstPath);
	if(dir1.exists() == false)
		dir1.mkpath(m_strConvertDstPath);
	m_map_upload2UIs.clear();
	m_iTotalPageCount = 0;
	m_iUploadedPageCount = 0;

	connect(this,SIGNAL(signal_fc_complete()),this,SLOT(slot_fc_complete()));
	connect(this,SIGNAL(signal_fc_error(int)),this,SLOT(slot_fc_error(int)));
	connect(this,SIGNAL(signal_fc_progress(int,int,QString)),this,SLOT(slot_fc_progress(int,int,QString)));
	connect(this,SIGNAL(signal_fc_upload_progress(quint32,quint32)),this,SLOT(slot_fc_upload_progress(quint32,quint32)));

	m_StrStatus = "课件下载中……";
	QTimer::singleShot(0, this, SLOT(slot_Download()));
}

CoursewareWidget::~CoursewareWidget()
{
	if(m_pFC_client != nullptr)
	{
		destory_fc_client(m_pFC_client);
		m_pFC_client = nullptr;
	}
	QMapIterator<QString, Upload2UI*> i(m_map_upload2UIs);
	while (i.hasNext())//有正在上传教案页工作的话则终止
	{
		i.next();
		Upload2UI* pUpload2UI = i.value();
		delete pUpload2UI;
		pUpload2UI = nullptr;
	}
	if(file)
	{
		file->close();
		delete file;
		file = nullptr;
	}
	NReply->deleteLater();

	delete ui;
}

bool CoursewareWidget::eventFilter(QObject *obj, QEvent *event)
{
	if(obj == ui->RadioName)
	{
		if(event->type() == QEvent::MouseButtonDblClick)
		{
			QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
			if (mouseEvent->button() == Qt::LeftButton)
			{
				if(ui->LabelLoading->isHidden() == false)
				{
					Global::g_NoticeDialog->init(m_StrStatus,"","",2000);
					Global::g_NoticeDialog->exec();
					return false;
				}
				emit signal_ShareCur(this);
			}
		}
	}

	return QObject::eventFilter(obj, event);
}

void CoursewareWidget::slot_Download()
{
	file = new QFile(m_strDocFileFullName);
	if(file->exists())
		file->remove();
	if(file->open(QIODevice::WriteOnly) == false)
	{
		delete file;
		file = nullptr;
		m_StrStatus = "保存课件失败……";
		return ;
	}
	QNetworkRequest NRequest;
	NRequest.setUrl(QUrl(QString("http://%1/uds/upload/%2").arg(Global::g_ServerIp).arg(QFileInfo(m_strDocFileFullName).
																						fileName())));
	NRequest.setHeader(QNetworkRequest::ContentTypeHeader,"application/octet-stream");
	QNetworkAccessManager *NAManager = new QNetworkAccessManager(this);
	NReply = NAManager->get(NRequest);
	connect(NReply,SIGNAL(readyRead()),this,SLOT(slot_readyRead()));
	connect(NAManager,SIGNAL(finished(QNetworkReply*)),this,SLOT(slot_finished(QNetworkReply*)));
}

void CoursewareWidget::slot_readyRead()
{
	file->write(NReply->readAll());
}

void CoursewareWidget::slot_finished(QNetworkReply *nr)
{
	if(nr->error() != QNetworkReply::NoError)
	{
		file->close();
		file->remove();
		delete file;
		file = nullptr;
		m_StrStatus = "下载课件失败……";
	}
	else
	{
		file->close();
		delete file;
		file = nullptr;
		m_StrStatus = "课件转储中……";
		slot_dump();
	}
}

void CoursewareWidget::slot_dump()
{
	m_pFC_client = create_fc_client(Global::g_strDCS_IP.toLatin1().data(),Global::g_iDCS_port,m_strDocFileFullName.toLocal8Bit().
									data(),m_strConvertDstPath.toLocal8Bit().data(),Global::g_LoginerId,Global::g_CurRoomId,this);
	if(m_pFC_client == nullptr)
	{
		m_StrStatus = "无法转储课件……";
		return ;
	}
}

void CoursewareWidget::fc_upload_progress(uint32_t curr, uint32_t total)
{
	emit signal_fc_upload_progress(curr, total);
}

void CoursewareWidget::fc_progress(int curr, int total, const char *fn)
{
	emit signal_fc_progress(curr, total, QString::fromLocal8Bit(fn));
}

void CoursewareWidget::fc_error(int err)
{
	emit signal_fc_error(err);
}

void CoursewareWidget::fc_complete()
{
	emit signal_fc_complete();
}

void CoursewareWidget::slot_fc_upload_progress(quint32 curr, quint32 total)//教案上传到文档转换服务器的进度
{
	int iProgress = (int)(((float)curr/(float)total)*100);
	if(iProgress == 100)
		m_StrStatus = "课件页转换中……";
}

void CoursewareWidget::slot_fc_progress(int curr, int total, QString strPageFileBaseName)//一个教案页转换完成（含下载完成）
{
	if(curr == 1)
		m_iTotalPageCount = total;
	m_StrStatus = QString("课件第%1页转换完成……").arg(curr);

	QString strPageFileBaseName_std = QString("%1_%2.png").arg(m_strDocFileBaseName).arg(curr);//标准格式png文件名
	QString strPageFileFullName_std = m_strConvertDstPath +"/"+ strPageFileBaseName_std;//全名
	if(strPageFileBaseName != strPageFileBaseName_std)//重复上传同一个教案文件
	{
		QFile file1(strPageFileFullName_std);
		if(file1.exists())
			file1.remove();//把老文件删掉
		QString strPageFileFullName = m_strConvertDstPath +"/"+ strPageFileBaseName;
		QFile file2(strPageFileFullName);
		file2.rename(strPageFileFullName_std);//把新文件重命名为标准格式
	}

	QPixmap pixmapPage = QPixmap(strPageFileFullName_std);//减小超大图片----
	if(pixmapPage.isNull())
	{
		qDebug() << "CoursewareWidget::slot_fc_progress:pixmapPage.isNull()";
		return ;
	}
	if(pixmapPage.width() > 1920 || pixmapPage.height() > 1080)
	{
		int iW, iH;
		if(pixmapPage.width() >= pixmapPage.height())//横图
		{
			iW = 1920;
			iH = pixmapPage.height()*1920.0 / pixmapPage.width();
		}
		else//竖图
		{
			iW = 1080;
			iH = pixmapPage.height()*1080.0 / pixmapPage.width();
		}
		pixmapPage.scaled(iW, iH, Qt::IgnoreAspectRatio, Qt::SmoothTransformation)
				.save(strPageFileFullName_std, QFileInfo(strPageFileFullName_std).suffix().toLatin1().data());
	}

	Upload2UI* pUpload2UI = new Upload2UI(strPageFileFullName_std);//上传该页到文件服务器----
	if(pUpload2UI->m_pK12upload == nullptr)
	{
		qDebug() << "CoursewareWidget::slot_fc_progress:pUpload2UI->m_pK12upload == nullptr";
		delete pUpload2UI;
		pUpload2UI = nullptr;
		return ;
	}
	connect(pUpload2UI, SIGNAL(signal_UploadComplete(QString,bool)),this, SLOT(slot_onePageUploadComplete(QString,bool)));
	m_map_upload2UIs.insert(strPageFileFullName_std, pUpload2UI);
}

void CoursewareWidget::slot_fc_error(int err)//2-没连上服务器,3-发送文件出错，4-服务端读取文件出错,5-转换后接收文件出错
{
	m_StrStatus = QString("课件页转换失败，错误码：%1").arg(err);;
}

void CoursewareWidget::slot_fc_complete()
{
}

void CoursewareWidget::slot_onePageUploadComplete(QString strPageFileFullName, bool succ)//一个教案页上传到文件服务器完成
{
	if(succ == false)
	{
		m_StrStatus = "课件页分享失败！";
		return ;
	}

	Upload2UI* pUpload2UI = m_map_upload2UIs.value(strPageFileFullName);
	m_map_upload2UIs.remove(strPageFileFullName);
	delete pUpload2UI;
	pUpload2UI = nullptr;

	++m_iUploadedPageCount;
	if(m_iUploadedPageCount == m_iTotalPageCount)//教案成功上传完成
	{
		m_StrStatus = "课件就绪！";
		ui->LabelLoading->hide();
	}
}
