#ifndef DOWNLOADAPPDIALOG_H
#define DOWNLOADAPPDIALOG_H

#include <QDialog>

namespace Ui {
class DownloadAppDialog;
}

class DownloadAppDialog : public QDialog
{
	Q_OBJECT

public:
	explicit DownloadAppDialog(QWidget *parent = 0);
	~DownloadAppDialog();

protected:
	void closeEvent(QCloseEvent *event);

private:
	Ui::DownloadAppDialog *ui;
};

#endif // DOWNLOADAPPDIALOG_H
