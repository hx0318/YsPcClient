#ifndef TEACHERDIALOG_H
#define TEACHERDIALOG_H

#include <QDialog>
#include <QJsonObject>
#include "coursewidget.h"
#include "mycoursewidget.h"
#include "courseinfodialog.h"
#include "foronelogin.h"
#include "k12msgs.h"

namespace Ui {
class TeacherDialog;
}

class TeacherDialog : public QDialog
{
	Q_OBJECT

public:
	explicit TeacherDialog(QString ticketId,QString chat_server,QWidget *parent = 0);
	~TeacherDialog();

protected:
	void closeEvent(QCloseEvent *event);
	void keyPressEvent(QKeyEvent *event);

private:
	Ui::TeacherDialog *ui;
	int MyCoursePageIndex;
	QString chat_server_ip;
	unsigned short chat_server_port;
	ForOneLogin oneLogin;
	k12msg::k12msgs *m_onelogin;
	QList<QJsonObject> ListCourse;
	QString m_ImagePath;
	QString m_ticketId;

	void queryMyCourseByDate(QDate de);
	void GetCourseList();
	void PageOfListCourse(int PageMark);

signals:
	void signal_TeacherDialog_close();

private slots:
	void slot_PButtonLogout_clicked();
	void slot_init_1();
	void slot_init_2();
	void slot_queryMyCourseByDate_reply(QJsonObject obj, void *);
	void slot_cw_EnterClassroom(CourseWidget *cw);
	void slot_joinClass(QJsonObject obj, void *da);
	void slot_GCWidgetDate_DateSelected(QDate date);
	void slot_GetNameConcem_reply(QJsonObject obj, void *);
	void slot_GetHeadimg(QPixmap pix,void*);
	void slot_RButtonCreateCourse_clicked();
	void slot_RButtonUploadCourseware_clicked();
	void slot_RButtonCourseSignUpRecord_clicked();
	void slot_GetCourseList_reply(QJsonObject obj, void *);
	void slot_PWidgetCoursePageSendPageMark(uint PageMark);
	void slot_OpenCourseInfo(QJsonObject obj);
	void selflogin(bool IfSelf);
	void disconnect_from_oneLogin();
	void __connect2msgs_state(int);
	void slot_CBoxCourseType_activated(int inx);
	void slot_Onelogin_reply(QJsonObject,void*);
};

#endif // TEACHERDIALOG_H
