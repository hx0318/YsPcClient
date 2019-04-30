#ifndef MYCOURSEWIDGET_H
#define MYCOURSEWIDGET_H

#include <QWidget>
#include "namanager.h"
#include <QLabel>

namespace Ui {
class MyCourseWidget;
}

class ImagePathHx
{
public:
	ImagePathHx();

	QString image_path;
	int label_index;
};

class MyCourseWidget : public QWidget
{
	Q_OBJECT

public:
	explicit MyCourseWidget(QWidget *parent = 0);
	~MyCourseWidget();
	void ClearWidgetMain();
	void SetObj(QJsonObject obj);
	void setStatus(QJsonObject obj);

	QString m_status;

protected:
	bool eventFilter(QObject *watched, QEvent *event);

private:
	Ui::MyCourseWidget *ui;
	QList<QString> ListStringHead;
	int ListIndex;
	QJsonObject m_obj;
	QString m_CoverImage;

	void SetLabelPixmap(QLabel *lab,QPixmap pix);
	QLabel *GetLabel(ImagePathHx *iph);

signals:
	void signal_OpenCourseInfo(QJsonObject obj);

private slots:
	void slot_GetPoster(QPixmap pix,void*);
	void slot_GetVipHead(QPixmap pix,void *da);
	void slot_PButtonLeft_clicked();
	void slot_PButtonRight_clicked();
	void slot_init();
};

#endif // MYCOURSEWIDGET_H
