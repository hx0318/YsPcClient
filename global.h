#ifndef GLOBAL_H
#define GLOBAL_H

#include <QString>
#include <QSettings>
#include "noticedialog.h"
#include "logindialog.h"
#include <QDir>
#include <QHostAddress>
#include <QNetworkInterface>
#include <Windows.h>
#include <QProcess>
#include <QTextCodec>

#define AppTarget "YsPcClient"

class Global
{
public:
	Global();
	static void DelDir(QString path);
	static QString getLocalIp();
	static QString getCpuDescribe();
	static QString getMemory();
	static QString GetNetDelayLoss();

	static QString g_AppDataPath;
	static QSettings *g_Settings;
	static NoticeDialog *g_NoticeDialog;
	static LoginDialog *g_LoginDialog;
	static QString g_ServerIp;
	static int g_LoginerId;
	static int g_CurRoomId;
	static QString g_strDCS_IP;
	static int g_iDCS_port;
	static QString g_strFS_IP;
	static int g_iFS_port;
	static QString g_LoginerName;
	static QString g_ImagePath;
	static QString g_Uuid;
	static QString g_LocalIp;
};

#endif // GLOBAL_H
