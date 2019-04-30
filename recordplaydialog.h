#ifndef RECORDPLAYDIALOG_H
#define RECORDPLAYDIALOG_H

#include <QDialog>
#include "namanager.h"
#include <QFile>
#include "media_player.h"
#include "recordwidget.h"

namespace Ui {
class RecordPlayDialog;
}

class RecordPlayDialog : public QDialog , public video_play
{
	Q_OBJECT

public:
	explicit RecordPlayDialog(QJsonArray rows,int crcpId,int RoomId,QWidget *parent = 0);
	~RecordPlayDialog();
	void play_progress(uint32_t, uint32_t);
	void video_data(const uint8_t *data, int w, int h);

protected:
	bool eventFilter(QObject *, QEvent *);
	void keyPressEvent(QKeyEvent *event);

private:
	Ui::RecordPlayDialog *ui;
	QFile *file;
	QNetworkReply *NReply;
	QString m_Mp4Menu;
	QString m_Mp4Path;
	media_player *play;
	RecordWidget *m_RW;

	void SetDisplayPos(int inx);
	void LoadingMp4(RecordWidget *rw);

signals:
	void signal_play_progress(int bytesSent, int bytesTotal);

private slots:
	void slot_init();
	void slot_WatchCourseware(RecordWidget *rw);
	void readContent();
	void replyFinished(QNetworkReply*);
	void loadProgress(qint64,qint64);
	void slot_PButtonPlayPause();
	void slot_PButtonReplay();
	void slot_play_progress(int bytesSent, int bytesTotal);
	void slot_VRWidgetMiddleLeft();
	void slot_Max_Common();
};

#endif // RECORDPLAYDIALOG_H
