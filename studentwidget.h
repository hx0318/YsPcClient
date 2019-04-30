#ifndef STUDENTWIDGET_H
#define STUDENTWIDGET_H

#include <QWidget>
#include <QJsonObject>

namespace Ui {
class StudentWidget;
}

class StudentWidget : public QWidget
{
	Q_OBJECT

public:
	explicit StudentWidget(int id,int UserType,QWidget *parent = 0);
	~StudentWidget();

	int VideoStatus;//0-关闭，1-打开，2-故障，3-不展示视频（即只听音频）
	int AudioStatus;
	bool HaveVideoStream;
	bool IsTeacherCloseVideo;//教师关闭了该用户的视频
	int m_id;

protected:
	void contextMenuEvent(QContextMenuEvent *event);

private:
	Ui::StudentWidget *ui;
	int m_type;
	QString m_ImagePath;

signals:
	void signal_VideoOpen(int UserId);
	void signal_VideoClose(int UserId);
	void signal_AudioOpen(int UserId);
	void signal_AudioClose(int UserId);

private slots:
	void slot_GetHeadPath(QJsonObject,void*);
	void slot_GetHead(QPixmap,void*);
	void slot_VideoOpen();
	void slot_VideoClose();
	void slot_AudioOpen();
	void slot_AudioClose();
};

#endif // STUDENTWIDGET_H
