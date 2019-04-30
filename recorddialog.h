#ifndef RECORDDIALOG_H
#define RECORDDIALOG_H

#include <QDialog>
#include "namanager.h"

namespace Ui {
class RecordDialog;
}

class RecordDialog : public QDialog
{
	Q_OBJECT

public:
	explicit RecordDialog(QWidget *parent = 0);
	~RecordDialog();

protected:
	 void closeEvent(QCloseEvent *);

private:
	Ui::RecordDialog *ui;
	QList<QString> ListName;

signals:
	void signal_NameAndDescribe(QString,QString);

private slots:
	void btnUpload_clicked();
	void btnCancel_clicked();
	void editCoursewareNote_textChanged();
	void editCoursewareName_textChanged(QString);
	void slot_GetRecordList(QJsonObject,void*);
};

#endif // RECORDDIALOG_H
