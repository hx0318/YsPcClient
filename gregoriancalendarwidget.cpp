#include "gregoriancalendarwidget.h"
#include "ui_gregoriancalendarwidget.h"
#pragma execution_character_set("utf-8")

GregorianCalendarWidget::GregorianCalendarWidget(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::GregorianCalendarWidget)
{
	ui->setupUi(this);

	for(int i = 0;i < 100;i++)
	{
		ui->CBoxYear->addItem(QString("%1").arg(2019+i));
		if(i < 12)
			ui->CBoxMonth->addItem(QString("%1月").arg(1+i));
	}
	ui->CBoxMonth->setCurrentIndex(QDate::currentDate().month()-1);
	ListDay.append(ui->PButtonDay_1);
	ListDay.append(ui->PButtonDay_2);
	ListDay.append(ui->PButtonDay_3);
	ListDay.append(ui->PButtonDay_4);
	ListDay.append(ui->PButtonDay_5);
	ListDay.append(ui->PButtonDay_6);
	ListDay.append(ui->PButtonDay_7);
	ListDay.append(ui->PButtonDay_8);
	ListDay.append(ui->PButtonDay_9);
	ListDay.append(ui->PButtonDay_10);
	ListDay.append(ui->PButtonDay_11);
	ListDay.append(ui->PButtonDay_12);
	ListDay.append(ui->PButtonDay_13);
	ListDay.append(ui->PButtonDay_14);
	ListDay.append(ui->PButtonDay_15);
	ListDay.append(ui->PButtonDay_16);
	ListDay.append(ui->PButtonDay_17);
	ListDay.append(ui->PButtonDay_18);
	ListDay.append(ui->PButtonDay_19);
	ListDay.append(ui->PButtonDay_20);
	ListDay.append(ui->PButtonDay_21);
	ListDay.append(ui->PButtonDay_22);
	ListDay.append(ui->PButtonDay_23);
	ListDay.append(ui->PButtonDay_24);
	ListDay.append(ui->PButtonDay_25);
	ListDay.append(ui->PButtonDay_26);
	ListDay.append(ui->PButtonDay_27);
	ListDay.append(ui->PButtonDay_28);
	ListDay.append(ui->PButtonDay_29);
	ListDay.append(ui->PButtonDay_30);
	ListDay.append(ui->PButtonDay_31);
	ListDay.append(ui->PButtonDay_32);
	ListDay.append(ui->PButtonDay_33);
	ListDay.append(ui->PButtonDay_34);
	ListDay.append(ui->PButtonDay_35);
	ListDay.append(ui->PButtonDay_36);
	ListDay.append(ui->PButtonDay_37);
	ListDay.append(ui->PButtonDay_38);
	ListDay.append(ui->PButtonDay_39);
	ListDay.append(ui->PButtonDay_40);
	ListDay.append(ui->PButtonDay_41);
	ListDay.append(ui->PButtonDay_42);
	QButtonGroup *bg = new QButtonGroup(this);
	for(int i = 0;i < 42;i++)
	{
		bg->addButton(ListDay.at(i),i);
	}
	m_PButtonChecked = new QPushButton(this);
	m_PButtonChecked->setCheckable(true);
	bg->addButton(m_PButtonChecked,42);
	m_PButtonChecked->hide();
	connect(bg,SIGNAL(buttonClicked(int)),this,SLOT(slot_bg_buttonClicked(int)));
	connect(ui->CBoxYear,SIGNAL(activated(QString)),this,SLOT(slot_CBoxYear_activated(QString)));
	connect(ui->CBoxMonth,SIGNAL(activated(int)),this,SLOT(slot_CBoxMonth_activated(int)));
	connect(ui->PButtonBackToday,SIGNAL(clicked(bool)),this,SLOT(slot_PButtonBackToday_clicked()));
	slot_PButtonBackToday_clicked();
}

GregorianCalendarWidget::~GregorianCalendarWidget()
{
	delete ui;
}

void GregorianCalendarWidget::slot_CBoxYear_activated(QString StringYear)
{
	int iYear = StringYear.toInt();
	int iMonth = ui->CBoxMonth->currentIndex()+1;
	int iow = GetIndexOfWeek(iYear,iMonth,1);
	int nod = GetNumberOfDays(iYear,iMonth);
	SetDaysOfMonth(iow,nod);
}

int GregorianCalendarWidget::GetNumberOfDays(int iYear,int iMonth)
{
	switch(iMonth)
	{
	case 1:
	case 3:
	case 5:
	case 7:
	case 8:
	case 10:
	case 12:
		return 31;
	case 4:
	case 6:
	case 9:
	case 11:
		return 30;
	case 2:
		if((iYear % 4 == 0 && iYear % 100 != 0) || iYear % 400 == 0)
			return 29;
		return 28;
	}
	return 0;
}

int GregorianCalendarWidget::GetIndexOfWeek(int iYear,int iMonth,int iDay)
{
	int m = iMonth;
	if(iMonth <= 2)
	{
		iYear--;
		m = iMonth + 12;
	}
	int y = iYear % 100;
	int c = iYear / 100;
	int w = (y + y / 4 + c / 4 - 2 * c + (13 * (m + 1) / 5) + iDay - 1) % 7;
	if(w <= 0)
		w += 7;
	return w;
}

void GregorianCalendarWidget::SetDaysOfMonth(int iow, int nod)
{
	for(int i = 0;i < 42;i++)
	{
		ListDay.at(i)->setText("");
		ListDay.at(i)->setChecked(false);
		ListDay.at(i)->setEnabled(false);
	}
	for(int i = 0; i < 42;i++)
	{
		if(i >= iow - 1 && i < iow + nod - 1)
		{
			ListDay.at(i)->setText(QString("%1").arg(i - iow + 2));
			ListDay.at(i)->setEnabled(true);
		}
	}
	m_PButtonChecked->setChecked(true);
}

void GregorianCalendarWidget::slot_CBoxMonth_activated(int IntMonthIndex)
{
	int iYear = ui->CBoxYear->currentText().toInt();
	int iMonth = IntMonthIndex+1;
	int iow = GetIndexOfWeek(iYear,iMonth,1);
	int nod = GetNumberOfDays(iYear,iMonth);
	SetDaysOfMonth(iow,nod);
}

void GregorianCalendarWidget::slot_bg_buttonClicked(int inx)
{
	emit signal_DateSelected(QDate(ui->CBoxYear->currentText().toInt(),ui->CBoxMonth->currentIndex()+1,
								   ListDay.at(inx)->text().toInt()));
}

void GregorianCalendarWidget::slot_PButtonBackToday_clicked()
{
	QDate date = QDate::currentDate();
	ui->CBoxYear->setCurrentText(QString("%1").arg(date.year()));
	ui->CBoxMonth->setCurrentIndex(date.month()-1);
	slot_CBoxMonth_activated(ui->CBoxMonth->currentIndex());
	for(int i = 0; i < 42;i++)
	{
		if(ListDay.at(i)->text().toInt() == date.day())
		{
			ListDay.at(i)->setChecked(true);
			slot_bg_buttonClicked(i);
		}
	}
}

void GregorianCalendarWidget::CheckCurrentDate()
{
	QDate date = QDate::currentDate();
	ui->CBoxYear->setCurrentText(QString("%1").arg(date.year()));
	ui->CBoxMonth->setCurrentIndex(date.month()-1);
	slot_CBoxMonth_activated(ui->CBoxMonth->currentIndex());
	for(int i = 0; i < 42;i++)
	{
		if(ListDay.at(i)->text().toInt() == date.day())
			ListDay.at(i)->setChecked(true);
	}
}
