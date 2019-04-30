#ifndef __K12MSGS_H__
#define __K12MSGS_H__

#include <memory>

#ifdef _MSC_VER
#if defined(LIBMSG_LIBRARY)
#  define LIBMSGSHARED_EXPORT __declspec(dllexport)
#else
#  define LIBMSGSHARED_EXPORT __declspec(dllimport)
#endif
#else
#  define LIBMSGSHARED_EXPORT
#endif

namespace k12msg {

class msg_observer {
public:
	virtual void recieved_msg(const char *msg, const char *fromid,
							  const char *toid) = 0;
	virtual void connect2msgs_state(int state) = 0; // state:0 连接消息系统失败;1 成功
	virtual void disconnect_from_msgs() = 0; // 与消息系统断开连接
};

class LIBMSGSHARED_EXPORT k12msgs {
public:
	virtual ~k12msgs() {}
	static k12msgs *create(msg_observer *obs, uint32_t userid,
						uint32_t roomid, const char *ip, uint16_t port);
	virtual int msgsend(const char *msg, const char *dst_userid) = 0;
};
}

#endif
