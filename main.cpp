#include <QApplication>
#include <Windows.h>
#include <DbgHelp.h>
#include <QDir>
#include <shlobj.h>
#include <QTextStream>
#include <QDateTime>
#include <direct.h>
#include "global.h"
#include <QMessageBox>
#include "logindialog.h"
#pragma comment(lib,"DbgHelp.lib")
#pragma comment(lib,"winmm.lib ")
#pragma execution_character_set("utf-8")

bool AppIsOnly()
{
	HANDLE hMutex = CreateMutex(NULL, FALSE,  L"YsPcClientOnly" );

	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		CloseHandle(hMutex);
		hMutex = NULL;
		return false;
	}

	return true;
}

static void CreateDumpFile(LPCWSTR lpstrDumpFilePathName, EXCEPTION_POINTERS *pException)
{
	HANDLE hDumpFile = CreateFile(lpstrDumpFilePathName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	MINIDUMP_EXCEPTION_INFORMATION dumpInfo;
	dumpInfo.ExceptionPointers = pException;
	dumpInfo.ThreadId = GetCurrentThreadId();
	dumpInfo.ClientPointers = TRUE;

	MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hDumpFile, MiniDumpNormal, &dumpInfo, NULL, NULL);

	CloseHandle(hDumpFile);
}

static long ExceptionFilter(LPEXCEPTION_POINTERS lpExceptionInfo)
{
	if (IsDebuggerPresent())
		return EXCEPTION_CONTINUE_SEARCH;

	char igo_base[MAX_PATH];
	memset(igo_base, 0, sizeof igo_base);
	if (FAILED(SHGetFolderPathA(NULL, CSIDL_LOCAL_APPDATA, NULL, 0, igo_base)))
		return -1;

	QString sp = QString("\\%1\\").arg(AppTarget);
	_snprintf(igo_base + strlen(igo_base), sizeof igo_base, sp.toLatin1().data());
	_mkdir(igo_base);

	char filepath[MAX_PATH] = "";
	_snprintf(filepath, sizeof filepath, "%s%u.dmp", igo_base, timeGetTime());

	WCHAR wszClassName[MAX_PATH];
	memset(wszClassName, 0, sizeof(wszClassName));
	MultiByteToWideChar(CP_ACP, 0, filepath, strlen(filepath) + 1, wszClassName, sizeof(wszClassName) / sizeof(wszClassName[0]));
	CreateDumpFile(wszClassName, lpExceptionInfo);

	return EXCEPTION_EXECUTE_HANDLER;
}

void MsgOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
	QString msg_type = "";
	switch(type)
	{
	case QtDebugMsg:
		msg_type = QString("Debug:");
		break;
	case QtInfoMsg:
		msg_type = QString("Info:");
		break;
	case QtWarningMsg:
		msg_type = QString("Warning:");
		break;
	case QtCriticalMsg:
		msg_type = QString("Critical:");
		break;
	case QtFatalMsg:
		msg_type = QString("Fatal:");
		break;
	default:
		break;
	}

	QString file_line = QString("File:%1 Line:%2").arg(QString(context.file)).arg(context.line);
	QString current_date_time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss ddd");
	QString msg_output = QString("%1 %2 %3 %4").arg(msg_type).arg(file_line).arg(msg).arg(current_date_time);

	QFile file(QDir::homePath() + "/AppData/Local/" + AppTarget + "/MsgOutput.txt");
	if(file.open(QIODevice::WriteOnly | QIODevice::Append))
	{
		QTextStream text_stream(&file);
		text_stream << msg_output << "\r\n";
		file.flush();
		file.close();
	}
}

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	Global::g_NoticeDialog = new NoticeDialog();
	if(false == AppIsOnly())
	{
		Global::g_NoticeDialog->init("程序已运行，不能重复运行！","","",-1);
		Global::g_NoticeDialog->exec();
		delete Global::g_NoticeDialog;
		Global::g_NoticeDialog = nullptr;
		return 0;
	}
	SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)ExceptionFilter);
	qInstallMessageHandler(MsgOutput);

	Global::g_LoginDialog = new LoginDialog();
	Global::g_LoginDialog->show();

	return a.exec();
}
