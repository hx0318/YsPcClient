#ifndef SLIDETEXTWIDGET_H
#define SLIDETEXTWIDGET_H

#include <QWidget>
#include <QTimer>

namespace Ui {
class SlideTextWidget;
}

class SlideTextWidget : public QWidget
{
	Q_OBJECT

public:
	explicit SlideTextWidget(QWidget *parent = 0);
	~SlideTextWidget();
	void setNextSlideText(QString st);
	QList<QString> ListString;

private:
	Ui::SlideTextWidget *ui;
	QTimer *slideTimer;
	int ClearIndex;

private slots:
	void slot_slideTimer_timeout();
};

#endif // SLIDETEXTWIDGET_H
