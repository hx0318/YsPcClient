#include "recordwidget.h"
#include "ui_recordwidget.h"

RecordWidget::RecordWidget(QString PathUrl,QString CoursewareName,QString houzhui,QWidget *parent) :
	QWidget(parent),
	ui(new Ui::RecordWidget)
{
	ui->setupUi(this);

	m_PathUrl = PathUrl.left(PathUrl.indexOf('?'));
	m_CoursewareName = CoursewareName;
	ui->LabelName->setText(m_CoursewareName);
	ui->LabelName->adjustSize();
	m_houzhui = houzhui;

	connect(ui->PButtonWatch,SIGNAL(clicked(bool)),this,SLOT(slot_PButtonWatch()));
}

RecordWidget::~RecordWidget()
{
	delete ui;
}

void RecordWidget::SetWatching(bool Watching)
{
	ui->PButtonWatch->setChecked(Watching);
}

void RecordWidget::slot_PButtonWatch()
{
	emit signal_WatchClick(this);
}
