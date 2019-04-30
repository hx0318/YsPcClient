#include "recordplaydialog.h"
#include "ui_recordplaydialog.h"
#include <QMovie>
#include <QDesktopWidget>
#include <QTimer>
#include "global.h"
#include <QMouseEvent>
#pragma execution_character_set("utf-8")

RecordPlayDialog::RecordPlayDialog(QJsonArray rows,int crcpId,int RoomId,QWidget *parent) :
	QDialog(parent),
	ui(new Ui::RecordPlayDialog)
{
	ui->setupUi(this);
	setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint);
	resize(QApplication::desktop()->availableGeometry().size());
	setWindowTitle("播放器");
	connect(ui->PButtonClose,SIGNAL(clicked(bool)),this,SLOT(reject()));
	connect(ui->PButtonPlayPause,SIGNAL(clicked(bool)),this,SLOT(slot_PButtonPlayPause()));
	ui->SliderTime->installEventFilter(this);
	connect(ui->PButtonReplay,SIGNAL(clicked(bool)),this,SLOT(slot_PButtonReplay()));
	connect(this,SIGNAL(signal_play_progress(int,int)),this,SLOT(slot_play_progress(int,int)));
	connect(ui->VRWidgetMiddleLeft,SIGNAL(signal_DoubleClick()),this,SLOT(slot_VRWidgetMiddleLeft()));

	QMovie *me = new QMovie(":/res/room/loading.gif", QByteArray(), this);
	ui->LabelLoading->setScaledContents(true);
	ui->LabelLoading->setMovie(me);
	me->start();

	m_Mp4Menu = QString("%1video/%2/%3/").arg(Global::g_AppDataPath).arg(crcpId).arg(RoomId);
	QDir dir(m_Mp4Menu);
	if(!dir.exists())
		dir.mkpath(m_Mp4Menu);

	m_RW = nullptr;
	play = nullptr;
	file = nullptr;
	NReply = nullptr;

	for(int i = 0;i < rows.size();i++)
	{
		QJsonObject one = rows.at(i).toObject();
		QString PlayURL = "";
		QString houzhui = "";
		QJsonDocument jd = QJsonDocument::fromJson(one["crvPath"].toString().toLocal8Bit().data());
		if(!jd.isNull())
		{
			QJsonObject PlayInfoList = jd.object()["PlayInfoList"].toObject();
			if(!PlayInfoList.isEmpty())
			{
				QString Definition = "";
				QJsonArray PlayInfo = PlayInfoList["PlayInfo"].toArray();
				for(int n = 0;n < PlayInfo.size();n++)
				{
					QJsonObject jo_one = PlayInfo.at(n).toObject();
					if(jo_one["Format"].toString() == "mp4")
					{
						houzhui = "mp4";
						if(jo_one["Definition"].toString() == "OD")
						{
							PlayURL = jo_one["PlayURL"].toString();
							break;
						}
						else if(jo_one["Definition"].toString() == "LD")
						{
							PlayURL = jo_one["PlayURL"].toString();
							Definition = "LD";
						}
						else if(jo_one["Definition"].toString() == "FD")
						{
							if(Definition != "LD")
							{
								PlayURL = jo_one["PlayURL"].toString();
								Definition = "FD";
							}
						}
					}
					else if(jo_one["Format"].toString() == "m3u8")
					{
						if(houzhui != "mp4")
						{
							houzhui = "m3u8";
							if(jo_one["Definition"].toString() == "OD")
								PlayURL = jo_one["PlayURL"].toString();
							else if(jo_one["Definition"].toString() == "LD")
							{
								if(Definition != "OD")
								{
									PlayURL = jo_one["PlayURL"].toString();
									Definition = "LD";
								}
							}
							else if(jo_one["Definition"].toString() == "FD")
							{
								if(Definition != "LD" && Definition != "OD")
								{
									PlayURL = jo_one["PlayURL"].toString();
									Definition = "FD";
								}
							}
						}
					}
				}
			}
		}
		QString CoursewareName = one["crvName"].toString();
		RecordWidget *rw = new RecordWidget(PlayURL,CoursewareName,houzhui,this);
		connect(rw,SIGNAL(signal_WatchClick(RecordWidget*)),this,SLOT(slot_WatchCourseware(RecordWidget*)));
		if(i == 0)
		{
			ui->LabelCoursewareName->setText("课件：" + CoursewareName);
			rw->SetWatching(true);
			m_RW = rw;
		}
		ui->VLayoutCourseware->addWidget(rw);
	}
	ui->SAWContents->setFixedHeight(45 * ui->VLayoutCourseware->count());

	QTimer::singleShot(0,this,SLOT(slot_init()));
}

RecordPlayDialog::~RecordPlayDialog()
{
	m_RW = nullptr;
	if(play)
	{
		delete play;
		play = nullptr;
	}
	if(file)
	{
		file->close();
		file->remove();
		delete file;
		file = nullptr;
	}
	if(NReply)
	{
		NReply->deleteLater();
		NReply = nullptr;
	}

	delete ui;
}

void RecordPlayDialog::SetDisplayPos(int inx)
{
	switch(inx)
	{
	case 0://加载
		ui->SliderTime->setEnabled(false);
		ui->VRWidgetMiddleLeft->clearWnd();
		ui->SliderTime->setValue(0);
		ui->LabelNothing->hide();
		ui->PButtonReplay->hide();
		ui->LabelLoading->show();
		ui->PBarLoading->setValue(0);
		ui->PBarLoading->show();
		break;
	case 1://视频
		ui->SliderTime->setEnabled(true);
		ui->LabelNothing->hide();
		ui->PButtonReplay->hide();
		ui->LabelLoading->hide();
		ui->PBarLoading->hide();
		break;
	case 2://重播
		ui->SliderTime->setEnabled(false);
		ui->VRWidgetMiddleLeft->clearWnd();
		ui->LabelNothing->hide();
		ui->PButtonReplay->show();
		ui->LabelLoading->hide();
		ui->PBarLoading->hide();
		break;
	case 3://下载失败
		ui->SliderTime->setEnabled(false);
		ui->VRWidgetMiddleLeft->clearWnd();
		ui->LabelNothing->show();
		ui->PButtonReplay->hide();
		ui->LabelLoading->hide();
		ui->PBarLoading->hide();
		break;
	default:
		break;
	}
}

void RecordPlayDialog::slot_init()
{
	SetDisplayPos(0);

	if(m_RW != nullptr)
		LoadingMp4(m_RW);
	else
	{
		ui->LabelNothing->setText("没有可以播放的课件！");
		SetDisplayPos(3);
	}
}

void RecordPlayDialog::slot_WatchCourseware(RecordWidget *rw)
{
	for(int i = 0;i < ui->VLayoutCourseware->count();i++)
	{
		RecordWidget *rw_temp = (RecordWidget *)ui->VLayoutCourseware->itemAt(i)->widget();
		if(rw_temp == rw)
		{
			rw_temp->SetWatching(true);
			ui->LabelCoursewareName->setText("课件：" + rw->m_CoursewareName);
			m_RW = rw;
			LoadingMp4(m_RW);
		}
		else
			rw_temp->SetWatching(false);
	}
}

void RecordPlayDialog::LoadingMp4(RecordWidget *rw)
{
	if(play)
	{
		delete play;
		play = nullptr;
	}
	if(file)
	{
		file->close();
		file->remove();
		delete file;
		file = nullptr;
	}
	if(NReply)
	{
		NReply->deleteLater();
		NReply = nullptr;
	}
	SetDisplayPos(0);

	if(rw->m_houzhui == "mp4")
	{
		m_Mp4Path = QString("%1%2.%3").arg(m_Mp4Menu).arg(rw->m_CoursewareName).arg(rw->m_houzhui);
		file = new QFile(m_Mp4Path);
		if(file->exists())
		{
			ui->PBarLoading->setValue(ui->PBarLoading->maximum());
			delete file;
			file = nullptr;
			play = media_player::create(m_Mp4Path.toStdString().c_str(), this,
										Global::g_Settings->value("conf/AudioOutDevSet").toInt());
			if(play)
			{
				ui->PButtonPlayPause->setChecked(true);
				SetDisplayPos(1);
				play->play();
			}
			else
			{
				ui->LabelNothing->setText("启动播放失败！");
				SetDisplayPos(3);
			}
			return ;
		}

		if(file->open(QIODevice::WriteOnly) == false)
		{
			ui->LabelNothing->setText("不具备写权限！");
			SetDisplayPos(3);
			delete file;
			file = nullptr;
			return ;
		}

		QNetworkRequest NRequest(rw->m_PathUrl);
		QSslConfiguration conf = NRequest.sslConfiguration();
		conf.setPeerVerifyMode(QSslSocket::VerifyNone);
		NRequest.setSslConfiguration(conf);
		NRequest.setHeader(QNetworkRequest::ContentTypeHeader,"application/octet-stream");
		QNetworkAccessManager *NAManager = new QNetworkAccessManager(this);
		NReply = NAManager->get(NRequest);
		connect(NReply,SIGNAL(readyRead()),this,SLOT(readContent()));
		connect(NAManager,SIGNAL(finished(QNetworkReply*)),this,SLOT(replyFinished(QNetworkReply*)));
		connect(NReply,SIGNAL(downloadProgress(qint64,qint64)),SLOT(loadProgress(qint64,qint64)));
	}
	else if(rw->m_houzhui == "m3u8")
	{
		play = media_player::create(rw->m_PathUrl.toStdString().c_str(), this,
									Global::g_Settings->value("conf/AudioOutDevSet").toInt());
		if(play)
		{
			ui->PButtonPlayPause->setChecked(true);
			SetDisplayPos(1);
			play->play();
		}
		else
		{
			ui->LabelNothing->setText("启动播放失败！");
			SetDisplayPos(3);
		}
	}
}

void RecordPlayDialog::readContent()
{
	file->write(NReply->readAll());
}

void RecordPlayDialog::replyFinished(QNetworkReply *)
{
	if(NReply->error() != QNetworkReply::NoError)
	{
		NReply->deleteLater();
		NReply = nullptr;
		ui->LabelNothing->setText("课件下载失败！");
		SetDisplayPos(3);
		file->close();
		file->remove();
		delete file;
		file = nullptr;
	}
	else
	{
		NReply->deleteLater();
		NReply = nullptr;
		file->close();
		delete file;
		file = nullptr;
		play = media_player::create(m_Mp4Path.toStdString().c_str(), this,
									Global::g_Settings->value("conf/AudioOutDevSet").toInt());
		if(play)
		{
			ui->PButtonPlayPause->setChecked(true);
			SetDisplayPos(1);
			play->play();
		}
		else
		{
			ui->LabelNothing->setText("启动播放失败！");
			SetDisplayPos(3);
		}
	}
}

void RecordPlayDialog::loadProgress(qint64 bytesSent, qint64 bytesTotal)
{
	ui->PBarLoading->setMaximum(bytesTotal);
	ui->PBarLoading->setValue(bytesSent);
}

void RecordPlayDialog::play_progress(uint32_t bytesSent, uint32_t bytesTotal)
{
	emit signal_play_progress((int)bytesSent,(int)bytesTotal);
}

void RecordPlayDialog::video_data(const uint8_t *data, int w, int h)
{
	if(play)
	{
		QImage ImageTemp((uchar *)data, w, h, QImage::Format_RGB888);
		if(ImageTemp.isNull() == false)
		{
			QPixmap PixmapTemp = QPixmap::fromImage(ImageTemp.scaled(ui->VRWidgetMiddleLeft->size()));
			if(PixmapTemp.isNull() == false)
				ui->VRWidgetMiddleLeft->SetBackground(PixmapTemp);
		}
	}
}

void RecordPlayDialog::slot_PButtonPlayPause()
{
	if(ui->PButtonPlayPause->isChecked())
	{
		if(play)
			play->pause(false);
	}
	else
	{
		if(play)
			play->pause(true);
	}
}

bool RecordPlayDialog::eventFilter(QObject *obj, QEvent *event)
{
	if(obj == ui->SliderTime)
	{
		if (event->type()==QEvent::MouseButtonRelease)
		{
			QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
			if (mouseEvent->button() == Qt::LeftButton)
			{
				int pos_x = mouseEvent->x();
				if(pos_x < 0)
					pos_x = 0;
				else if(pos_x > ui->SliderTime->width())
					pos_x = ui->SliderTime->width();
				int pos = ui->SliderTime->minimum() + (ui->SliderTime->maximum() - ui->SliderTime->minimum()) *
						((double)pos_x / ui->SliderTime->width());
				if(play)
					play->seek(pos*100/ui->SliderTime->maximum());
			}
		}
	}

	return QDialog::eventFilter(obj,event);
}

void RecordPlayDialog::slot_PButtonReplay()
{
	LoadingMp4(m_RW);
}

void RecordPlayDialog::slot_play_progress(int bytesSent, int bytesTotal)
{
	if(bytesSent >= bytesTotal)
	{
		delete play;
		play = nullptr;
		SetDisplayPos(2);
	}
	if(play)
	{
		ui->SliderTime->setMaximum(bytesTotal);
		ui->LabelTimeTotal->setText(QString("%1:%2:%3").arg(bytesTotal/3600,2,10,QLatin1Char('0'))
									.arg(bytesTotal%3600/60,2,10,QLatin1Char('0')).arg(bytesTotal%60,2,10,QLatin1Char('0')));
		ui->SliderTime->setValue(bytesSent);
		ui->LabelTimeCurrent->setText(QString("%1:%2:%3").arg(bytesSent/3600,2,10,QLatin1Char('0'))
									  .arg(bytesSent%3600/60,2,10,QLatin1Char('0')).arg(bytesSent%60,2,10,QLatin1Char('0')));
	}
}

void RecordPlayDialog::slot_VRWidgetMiddleLeft()
{
	if(play)
		play->pause(true);
	resize(QApplication::desktop()->size());
	ui->WidgetTop->hide();
	ui->WidgetRight->hide();
	ui->WidgetBottomLeft->hide();
	if(play)
		QTimer::singleShot(0, this, SLOT(slot_Max_Common()));
}

void RecordPlayDialog::keyPressEvent(QKeyEvent *event)
{
	switch (event->key())
	{
	case Qt::Key_Escape:
		if(play)
			play->pause(true);
		ui->WidgetTop->show();
		ui->WidgetRight->show();
		ui->WidgetBottomLeft->show();
		resize(QApplication::desktop()->availableGeometry().size());
		if(play)
			QTimer::singleShot(0, this, SLOT(slot_Max_Common()));
		break;
	default:
		QDialog::keyPressEvent(event);
	}
}

void RecordPlayDialog::slot_Max_Common()
{
	if(play)
		play->pause(false);
}
