#ifndef COURSEWAREWIDGET_H
#define COURSEWAREWIDGET_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QFile>
#include "file_convert_client.h"
#include "upload2ui.h"

using namespace sc;

namespace Ui {
class CoursewareWidget;
}

class CoursewareWidget : public QWidget, public fc_observer
{
	Q_OBJECT

public:
	explicit CoursewareWidget(QString filepath,QString name,int CoursewareId,QWidget *parent = 0);
	~CoursewareWidget();
	void fc_progress(int curr, int total, const char *fn);
	void fc_error(int err = 0);
	void fc_complete();
	void fc_upload_progress(uint32_t curr, uint32_t total);

	int m_iTotalPageCount;//教案的总页数
	QString m_strDocFileFullName;//教案的全名
	int m_CoursewareId;

protected:
	bool eventFilter(QObject *watched, QEvent *event);

private:
	Ui::CoursewareWidget *ui;
	QString m_StrStatus;
	QFile *file;
	QNetworkReply *NReply;
	void* m_pFC_client;//office教案转换为png图片页们的客户端对象指针
	QString m_strDocFileBaseName;//教案的文件名(不含路径和后缀)
	QString m_strConvertDstPath;//教案转换为png图片页存放的目录
	QMap<QString, Upload2UI*> m_map_upload2UIs;//当前正在执行上传教案页的对象列表
	int m_iUploadedPageCount;//已成功上传页数

signals:
	void signal_fc_progress(int curr, int total, QString);
	void signal_fc_error(int err);
	void signal_fc_complete();
	void signal_fc_upload_progress(quint32 curr, quint32 total);
	void signal_ShareCur(CoursewareWidget *dw);

private slots:
	void slot_Download();
	void slot_readyRead();
	void slot_finished(QNetworkReply *nr);
	void slot_dump();
	void slot_fc_progress(int curr, int total, QString);
	void slot_fc_error(int err);
	void slot_fc_complete();
	void slot_fc_upload_progress(quint32 curr, quint32 total);
	void slot_onePageUploadComplete(QString,bool);
};

#endif // COURSEWAREWIDGET_H
