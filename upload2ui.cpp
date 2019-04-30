#include "upload2ui.h"
#include "global.h"

Upload2UI::Upload2UI(QString strFileName,QObject *parent) : QObject(parent)
{
	connect(this, SIGNAL(signal_progress(quint32,quint32)), this, SLOT(slot_progress(quint32,quint32)));
	connect(this, SIGNAL(signal_trans_error(int)), this, SLOT(slot_trans_error(int)));
	connect(this, SIGNAL(signal_complete()), this, SLOT(slot_complete()));
	m_strFileName = strFileName;
	m_pK12upload = k12upload::create(this, Global::g_LoginerId, Global::g_CurRoomId,m_strFileName.toLocal8Bit().data(),"",
									 Global::g_strFS_IP.toLatin1().data(),Global::g_iFS_port);
}

Upload2UI::~Upload2UI()
{
	if(m_pK12upload != nullptr)
	{
		delete m_pK12upload;
		m_pK12upload = nullptr;
	}
}

void Upload2UI::progress(uint32_t curr, uint32_t total)
{
	emit signal_progress(curr, total);
}

void Upload2UI::trans_error(int err)
{
	emit signal_trans_error(err);
}

void Upload2UI::complete()
{
	emit signal_complete();
}

void Upload2UI::slot_progress(quint32 curr, quint32 total)
{
	int iProgress = (int)((((float)curr/(float)total))*100);
	Q_UNUSED(iProgress);
}

void Upload2UI::slot_trans_error(int err)
{
	if(err == 1)//0 网络错误；1 服务器文件已存在；2 打开待上传的文件失败；3 读取文件失败；5 要上传的文件不存在
		emit signal_UploadComplete(m_strFileName, true);
	else
		emit signal_UploadComplete(m_strFileName, false);
}

void Upload2UI::slot_complete()
{
	emit signal_UploadComplete(m_strFileName, true);
}
