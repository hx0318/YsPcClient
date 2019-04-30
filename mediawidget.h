#ifndef MEDIAWIDGET_H
#define MEDIAWIDGET_H

#include <QWidget>
#include "namanager.h"

namespace Ui {
class MediaWidget;
}

class MediaWidget : public QWidget
{
	Q_OBJECT

public:
	explicit MediaWidget(QWidget *parent = 0);
	~MediaWidget();
	void ChangePixmap(QPixmap Pixmap);
	void SetVideoStatus(int st);
	void SetAudioStatus(int st);
	void SetHeadName();
	void ClearAll();
	void ShowPButtonDown();

	int m_id;
	int m_VideoStatus;//0-关，1-开

protected:
	void paintEvent(QPaintEvent *);
	void resizeEvent(QResizeEvent *);
	void enterEvent(QEvent *);
	void leaveEvent(QEvent *);

private:
	Ui::MediaWidget *ui;
	QPixmap m_Pixmap;
	int m_AudioStatus;
	QString m_ImagePath;

signals:
	void signal_VideoOpen(int UserId);
	void signal_VideoClose(int UserId);
	void signal_AudioOpen(int UserId);
	void signal_AudioClose(int UserId);
	void signal_MediaUp(int UserId);
	void signal_MediaDown(int UserId);

private slots:
	void slot_GetHeadPath(QJsonObject,void*);
	void slot_GetHead(QPixmap,void*);
	void slot_PButtonVideo_clicked();
	void slot_PButtonVideo_Enable();
	void slot_PButtonAudio_clicked();
	void slot_PButtonUp_clicked();
	void slot_PButtonDown_clicked();
};

#endif // MEDIAWIDGET_H
