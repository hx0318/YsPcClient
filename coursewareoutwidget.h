#ifndef COURSEWAREOUTWIDGET_H
#define COURSEWAREOUTWIDGET_H

#include <QWidget>
#include <QDateTime>

namespace Ui {
class CoursewareOutWidget;
}

class CoursewareOutWidget : public QWidget
{
	Q_OBJECT

public:
	explicit CoursewareOutWidget(QWidget *parent = 0);
	~CoursewareOutWidget();
	void Init_enter_reply(QJsonObject obj,int LineMark);
	void Init_see(QString str_des,int LineMark);

	int m_RoomId;
	int m_crcpId;
	QDateTime DTimeClassBegin;
	QDateTime DTimeClassOver;
	QString m_cpiName;

private:
	Ui::CoursewareOutWidget *ui;

signals:
	void signal_EnterClassroom(CoursewareOutWidget *cow);
	void signal_OpenRecordPlay(CoursewareOutWidget *cow);
	void signal_SeeCurriculumSchedule();

private slots:
	void slot_PButtonEnterClassroom_clicked();
	void slot_PButtonReplay_clicked();
	void slot_PButtonSee_clicked();
};

#endif // COURSEWAREOUTWIDGET_H
