#ifndef LOOKWIDGET_H
#define LOOKWIDGET_H

#include <QWidget>

namespace Ui {
class LookWidget;
}

class LookWidget : public QWidget
{
	Q_OBJECT

public:
	explicit LookWidget(QWidget *parent = 0);
	~LookWidget();
	void setLookText(QString str);

private:
	Ui::LookWidget *ui;
};

#endif // LOOKWIDGET_H
