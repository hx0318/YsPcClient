#include "lookwidget.h"
#include "ui_lookwidget.h"

LookWidget::LookWidget(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::LookWidget)
{
	ui->setupUi(this);
}

LookWidget::~LookWidget()
{
	delete ui;
}

void LookWidget::setLookText(QString str)
{
	ui->TEditContent->setPlainText(str);
}
