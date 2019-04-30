#ifndef GREGORIANCALENDARWIDGET_H
#define GREGORIANCALENDARWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QDate>

namespace Ui {
class GregorianCalendarWidget;
}

class GregorianCalendarWidget : public QWidget
{
	Q_OBJECT

public:
	explicit GregorianCalendarWidget(QWidget *parent = 0);
	~GregorianCalendarWidget();

	void CheckCurrentDate();

private:
	Ui::GregorianCalendarWidget *ui;
	QList<QPushButton *> ListDay;
	QPushButton *m_PButtonChecked;

	int GetNumberOfDays(int iYear,int iMonth);
	int GetIndexOfWeek(int iYear,int iMonth,int iDay);
	void SetDaysOfMonth(int iow,int nod);

signals:
	void signal_DateSelected(QDate date);

private slots:
	void slot_CBoxYear_activated(QString StringYear);
	void slot_CBoxMonth_activated(int IntMonthIndex);
	void slot_bg_buttonClicked(int inx);
	void slot_PButtonBackToday_clicked();
};

#endif // GREGORIANCALENDARWIDGET_H
