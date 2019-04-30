#ifndef FORONELOGIN_H
#define FORONELOGIN_H

#include <QObject>
#include "k12msgs.h"

class ForOneLogin : public QObject,public k12msg::msg_observer
{
	Q_OBJECT
public:
	explicit ForOneLogin(QObject *parent = nullptr);
	void recieved_msg(const char *msg, const char *, const char *);
	void connect2msgs_state(int state); // state:0 连接消息系统失败;1 成功
	void disconnect_from_msgs(); // 与消息系统断开连接

signals:
	void signal_SelfLogin(bool IfSelf);
	void signal_disconnect_from_oneLogin();
	void _connect2msgs_state(int);
};

#endif // FORONELOGIN_H
