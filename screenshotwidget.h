#ifndef SCREENSHOTWIDGET_H
#define SCREENSHOTWIDGET_H

#include <QWidget>
#include "global.h"
#include "namanager.h"
#include <QDir>

namespace Ui {
class ScreenshotWidget;
}

class ScreenshotWidget : public QWidget
{
	Q_OBJECT

public:
    explicit ScreenshotWidget(QString cpdImgAddr,QString cpdCreateTime,QWidget *parent = 0);
	~ScreenshotWidget();

private:
	Ui::ScreenshotWidget *ui;
	QString m_ImagePath;

private slots:
	void slot_GetScreenshot(QPixmap,void*);
	void slot_PButtonSee_clicked();
};

#endif // SCREENSHOTWIDGET_H
