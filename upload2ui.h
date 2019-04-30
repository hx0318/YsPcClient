#ifndef UPLOAD2UI_H
#define UPLOAD2UI_H

#include <QObject>
#include "file_transfer.h"

class Upload2UI : public QObject, public trans_observer
{
	Q_OBJECT
public:
	explicit Upload2UI(QString strFileName,QObject *parent = nullptr);
	~Upload2UI();
	void progress(uint32_t curr, uint32_t total);
	void trans_error(int err=0);
	void complete();

	k12upload* m_pK12upload;

private:
	QString m_strFileName;//此处为完整的路径文件名

signals:
	void signal_progress(quint32 curr, quint32 total);
	void signal_trans_error(int err=0);
	void signal_complete();
	void signal_UploadComplete(QString, bool);
	void signal_k12upload_null();

private slots:
	void slot_progress(quint32 curr, quint32 total);
	void slot_trans_error(int err=0);
	void slot_complete();
};

#endif // UPLOAD2UI_H
