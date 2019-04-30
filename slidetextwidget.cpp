#include "slidetextwidget.h"
#include "ui_slidetextwidget.h"

SlideTextWidget::SlideTextWidget(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::SlideTextWidget)
{
	ui->setupUi(this);

	ListString.clear();
	ClearIndex = 0;
	slideTimer = new QTimer();
	connect(slideTimer,SIGNAL(timeout()),this,SLOT(slot_slideTimer_timeout()));
	slideTimer->start(50);
}

SlideTextWidget::~SlideTextWidget()
{
	ListString.clear();
	slideTimer->stop();
	delete slideTimer;
	slideTimer = nullptr;

	delete ui;
}

void SlideTextWidget::setNextSlideText(QString st)
{
	ListString.append(st.replace("\n",""));
	if(ui->LabelText->text() == "")
		ClearIndex = 0;
}

void SlideTextWidget::slot_slideTimer_timeout()
{
	if(ClearIndex == 0)
	{
		if(ListString.count() > 0)
		{
			ui->LabelText->setText(ListString.at(0));
			ui->LabelText->adjustSize();
			ui->LabelText->move(90,ui->LabelText->y());
			ListString.removeAt(0);
			show();
		}
		else
			hide();
	}
	ClearIndex++;
	if(ClearIndex >= 80)
		ClearIndex = 0;

	if(ui->LabelText->width() <= width())
		ui->LabelText->move((width()-ui->LabelText->width())/2,ui->LabelText->y());
	else
	{
		if(ui->LabelText->x() + ui->LabelText->width() <= 0)
			ui->LabelText->move(90,ui->LabelText->y());
		else
			ui->LabelText->move(ui->LabelText->x()-6,ui->LabelText->y());
	}
}
