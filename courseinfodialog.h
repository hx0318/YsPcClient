#ifndef COURSEINFODIALOG_H
#define COURSEINFODIALOG_H

#include <QDialog>
#include "coursewareoutwidget.h"
#include "namanager.h"
#include "messagewidget.h"
#include "lookwidget.h"

class UrlImg
{
public:
	UrlImg(QString url,QString path);

	QString img_url;
	QString img_path;
};

namespace Ui {
class CourseInfoDialog;
}

class CourseInfoDialog : public QDialog
{
	Q_OBJECT

public:
	explicit CourseInfoDialog(QJsonObject obj,QWidget *parent = 0);
	~CourseInfoDialog();

protected:
	void mousePressEvent(QMouseEvent *);
	void closeEvent(QCloseEvent *event);

private:
	Ui::CourseInfoDialog *ui;
	int m_ciId;
	QString m_string_html;
	QMap<QString,UrlImg *> MapUrlImg;
	LookWidget *m_LookWidget;
	QJsonObject m_obj;

	void queryDetail();

signals:
	void signal_TeacherDialog_close();

private slots:
	void slot_PButtonAbout_clicked(bool IsChecked);
	void slot_PButtonCourse_clicked(bool IsChecked);
	void slot_cow_EnterClassroom(CoursewareOutWidget *cow);
	void slot_joinClass(QJsonObject obj, void *da);
	void slot_PButtonMessage_clicked(bool IsChecked);
	void slot_PButtonPublish_clicked();
	void slot_PButtonPublish_reply(QJsonObject obj, void *);
	void slot_queryDetail(QJsonObject obj, void *);
	void slot_MessageReplySuccess();
	void slot_MessagePraiseSuccess();
	void slot_GetHtmlImage(QPixmap pix,void *da);
	void slot_cow_OpenRecordPlay(CoursewareOutWidget *cow);
	void slot_RecordPlay(QJsonObject obj, void *da);
	void slot_SeeCurriculumSchedule();
	void slot_TeacherDialog_close();
	void slot_init();
	void slot_PButtonClose_clicked();
	void slot_TEditMessage_textChanged();
};

#endif // COURSEINFODIALOG_H
