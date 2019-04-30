#ifndef RECORDWIDGET_H
#define RECORDWIDGET_H

#include <QWidget>

namespace Ui {
class RecordWidget;
}

class RecordWidget : public QWidget
{
	Q_OBJECT

public:
	explicit RecordWidget(QString PathUrl,QString CoursewareName,QString houzhui,QWidget *parent = 0);
	~RecordWidget();
	void SetWatching(bool Watching);

	QString m_CoursewareName;
	QString m_PathUrl;
	QString m_houzhui;

private:
	Ui::RecordWidget *ui;

signals:
	void signal_WatchClick(RecordWidget *cw);

private slots:
	void slot_PButtonWatch();
};

#endif // RECORDWIDGET_H
