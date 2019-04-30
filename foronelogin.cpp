#include "foronelogin.h"
#include <QJsonParseError>
#include <QJsonObject>
#include "global.h"
#include <QHostInfo>

ForOneLogin::ForOneLogin(QObject *parent) : QObject(parent)
{
}

void ForOneLogin::recieved_msg(const char *msg,const char *,const char *)
{
	QJsonParseError json_error;
	QJsonDocument parse_doucment = QJsonDocument::fromJson(QString(msg).toUtf8(), &json_error);
	if(json_error.error != QJsonParseError::NoError)
		return ;
	if(parse_doucment.isObject() == false)
		return ;

	QJsonObject obj = parse_doucment.object();
	if(obj["typeString"].toString() == "dddl")
	{
		if(obj["Uuid"].toString() != Global::g_Uuid)
			emit signal_SelfLogin(true);
		else
			emit signal_SelfLogin(false);
	}
}

void ForOneLogin::connect2msgs_state(int state)//state:0 连接消息系统失败;1 成功
{
	emit _connect2msgs_state(state);
}

void ForOneLogin::disconnect_from_msgs()//与消息系统断开连接
{
	emit signal_disconnect_from_oneLogin();
}
