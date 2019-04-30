#ifndef COURSEWIDGET_H
#define COURSEWIDGET_H

#include <QWidget>
#include <QJsonObject>
#include <QTimer>
#include <QDateTime>

namespace Ui {
class CourseWidget;
}

class CourseWidget : public QWidget
{
	Q_OBJECT

public:
	explicit CourseWidget(QJsonObject obj,int LineMark,QWidget *parent = 0);
	~CourseWidget();

	int m_RoomId;
	int m_crcpId;
	QDateTime DTimeClassBegin;
	QDateTime DTimeClassOver;
	QString m_cpiName;

private:
	Ui::CourseWidget *ui;
	QTimer m_timer;
	QDateTime m_DTimeBegin;
	QDateTime m_DTimeEnd;

signals:
	void signal_EnterClassroom(CourseWidget *cw);

private slots:
	void slot_timer_timeout();
	void slot_PButtonEnterclassroom_clicked();
};

#endif // COURSEWIDGET_H
