#include "global.h"
#pragma execution_character_set("utf-8")

QString Global::g_AppDataPath = "";
QSettings *Global::g_Settings = nullptr;
NoticeDialog *Global::g_NoticeDialog = nullptr;
LoginDialog *Global::g_LoginDialog = nullptr;
QString Global::g_ServerIp = "";
int Global::g_LoginerId = -1;
int Global::g_CurRoomId = -1;
QString Global::g_strDCS_IP = "";
int Global::g_iDCS_port = -1;
QString Global::g_strFS_IP = "";
int Global::g_iFS_port = -1;
QString Global::g_LoginerName = "";
QString Global::g_ImagePath = "";
QString Global::g_Uuid = "";
QString Global::g_LocalIp = "";

Global::Global()
{
}

void Global::DelDir(QString path)
{
	QDir dir(path);
	if(!dir.exists())
		return ;

	dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);
	QFileInfoList fileList = dir.entryInfoList();
	foreach (QFileInfo file, fileList)
	{
		if (file.isFile())
			file.dir().remove(file.fileName());
		else
			DelDir(file.absoluteFilePath());
	}

	dir.rmpath(dir.absolutePath());
}

QString Global::getLocalIp()
{
	QProcess exc;
	exc.start("netstat -n");
	exc.waitForFinished(3000);
	QTextCodec *codec = QTextCodec::codecForName("GBK");
	QStringList SLNetstat = codec->toUnicode(exc.readAll()).split("\r\n");

	QString retstr;
	for(int i = 0;i < SLNetstat.count();i++)
	{
		QString str = SLNetstat.at(i);
		if(str.contains("47.93.15.90") && str.contains("127.0.0.1") == false)
		{
			QStringList SLstr = str.split(" ");
			for(int j = 0;j < SLstr.count();j++)
			{
				QString str_t = SLstr.at(j);
				if(str_t.contains(":"))
				{
					retstr = str_t.left(str_t.indexOf(":"));
					break;
				}
			}
			break;
		}
	}

	if(retstr.isEmpty())
	{
		for(int i = 0;i < SLNetstat.count();i++)
		{
			QString str = SLNetstat.at(i);
			if(str.contains("TCP") && str.contains("127.0.0.1") == false)
			{
				QStringList SLstr = str.split(" ");
				for(int j = 0;j < SLstr.count();j++)
				{
					QString str_t = SLstr.at(j);
					if(str_t.contains(":"))
					{
						retstr = str_t.left(str_t.indexOf(":"));
						break;
					}
				}
				break;
			}
		}
	}

	if(retstr.isEmpty())
	{
		QList<QHostAddress> ListHostAddress = QNetworkInterface::allAddresses();
		for (int i = 0; i < ListHostAddress.size(); ++i)
		{
			if (ListHostAddress.at(i) != QHostAddress::LocalHost &&  ListHostAddress.at(i).toIPv4Address())
			{
				retstr = ListHostAddress.at(i).toString();
				break;
			}
		}
	}

	if(retstr.isEmpty())
		retstr = QHostAddress(QHostAddress::LocalHost).toString();

	return retstr;
}

QString Global::getCpuDescribe()
{
	QSettings *sc = new QSettings("HKEY_LOCAL_MACHINE\\HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0",
								  QSettings::NativeFormat);
	QString cpuDescribe = sc->value("ProcessorNameString").toString();
	delete sc;
	sc = nullptr;
	return cpuDescribe;
}

QString Global::getMemory()
{
	MEMORYSTATUSEX statex;
	statex.dwLength = sizeof (statex);
	GlobalMemoryStatusEx(&statex);
	float ap = statex.ullAvailPhys*1.0/(1024*1024*1024);
	float tp = statex.ullTotalPhys*1.0/(1024*1024*1024);
	return QString("可用%1GB/共%2GB").arg(QString::asprintf("%.2f",ap)).arg(QString::asprintf("%.2f",tp));
}

QString Global::GetNetDelayLoss()
{
	QString retstr = "请求超时,";

	QProcess exc;
	QString cmdstr = QString("ping %1 -n 1 -w 1000").arg(g_ServerIp);//Windows指令 "ping IP -n 1 -w 超时(ms)"
	exc.start(cmdstr);
	exc.waitForFinished(1000);

	QTextCodec *codec = QTextCodec::codecForName("GBK");
	QString outstr = codec->toUnicode(exc.readAll()).replace("\r\n","");
	if(outstr.contains("往返行程的估计时间"))
	{
		int ttl_inx = outstr.indexOf(" TTL");
		int less_inx = outstr.lastIndexOf("<",ttl_inx);
		if(less_inx >= 0)
			retstr = outstr.mid(less_inx+1,ttl_inx-less_inx-1)+",";
		else
		{
			int equal_inx = outstr.lastIndexOf("=",ttl_inx);
			retstr = outstr.mid(equal_inx+1,ttl_inx-equal_inx-1)+",";
		}
	}
	int loss_inx = outstr.indexOf(" 丢失");
	int left_parenthesis_inx = outstr.lastIndexOf("(",loss_inx);
	retstr = retstr + outstr.mid(left_parenthesis_inx+1,loss_inx-left_parenthesis_inx-1);

	return retstr;
}
