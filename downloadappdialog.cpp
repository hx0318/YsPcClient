#include "downloadappdialog.h"
#include "ui_downloadappdialog.h"
#include <QCloseEvent>

DownloadAppDialog::DownloadAppDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::DownloadAppDialog)
{
	ui->setupUi(this);
	setWindowFlags(windowFlags()|Qt::FramelessWindowHint);
	setWindowOpacity(0.9);

	connect(ui->PButtonClose,SIGNAL(clicked(bool)),this,SLOT(reject()));
}

DownloadAppDialog::~DownloadAppDialog()
{
	delete ui;
}

void DownloadAppDialog::closeEvent(QCloseEvent *event)
{
	event->ignore();
}
