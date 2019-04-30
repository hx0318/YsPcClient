#ifndef LIBPDU_H
#define LIBPDU_H

#include "k12pdu_defines.h"

#ifdef _MSC_VER
#if defined(LIBPDU_LIBRARY)
#  define LIBPDUSHARED_EXPORT __declspec(dllexport)
#else
#  define LIBPDUSHARED_EXPORT __declspec(dllimport)
#endif
#else
#  define LIBPDUSHARED_EXPORT
#endif

class k12mcu2ui {
public:
	virtual ~k12mcu2ui() {}
	virtual int msg(k12mcu2ui_type t, void *arg) = 0;

	virtual void ss_start_class(uint32_t uid) = 0;
	virtual void start_class_ack(int code) = 0;
	virtual void ss_stop_class(uint32_t uid) = 0;
	virtual void stop_class_ack(int code) = 0;
	virtual void get_class_state_ack(int state) = 0;
	virtual void ss_application_share(uint32_t uid) = 0;
	virtual void application_share_ack(int code) = 0;
	virtual void ss_cancel_application(uint32_t uid) = 0;
	virtual void cancel_application_ack(int code) = 0;
	virtual void ss_reply_share(uint32_t uid, int reply) = 0;
	virtual void reply_share_ack(uint32_t uid, int reply) = 0;
	virtual void ss_start_share(uint32_t uid) = 0;
	virtual void start_share_ack(int code) = 0;
	virtual void ss_stop_share(uint32_t uid) = 0;
	virtual void stop_share_ack(int code) = 0;
	virtual void get_share_users_ack(int code, int cnt, uint32_t *uid) = 0;
	virtual void ss_start_tag(uint32_t uid) = 0;
	virtual void start_tag_ack(int code) = 0;
	virtual void ss_stop_tag(uint32_t uid) = 0;
	virtual void stop_tag_ack(int code) = 0;
	virtual void get_tag_users_ack(int code, int cnt, uint32_t *uid) = 0;
	/* 取消共享,用户获取共享权限后，在共享前被取消，如果已共享，则停止共享，需要调用stop_share */
	virtual void ss_cancel_share(uint32_t uid) = 0;
	virtual void cancel_share_ack(int code) = 0;
	virtual void p2p_msg(uint32_t src_uid, const char *con, uint32_t con_len) = 0;
	virtual void p2all_msg(uint32_t src_uid, const char *con, uint32_t con_len) = 0;
	virtual void get_classing_user_ack(int code, uint32_t uid) = 0;
	virtual void p2p_msg_ack(uint32_t src_uid, const char *con, uint32_t con_len) = 0;
	virtual void p2all_msg_ack(uint32_t src_uid, const char *con, uint32_t con_len) = 0;
	virtual void user_net_status(uint32_t uid, int state) = 0;  // state 1:异常 0:正常
};

class LIBPDUSHARED_EXPORT Libpdu
{
public:
	static Libpdu *create(k12mcu2ui *back, uint32_t userid, uint32_t roomid,
						  const char *mcuip, uint16_t port, user_role_t role);
	static void destory(Libpdu *pdu);
	virtual ~Libpdu() {}

	virtual int msg(libpdu_type t, void *arg) = 0;

	virtual int start_class() = 0;
	virtual int stop_class(uint32_t uid) = 0;
	virtual int get_class_state() = 0;
	virtual int application_share() = 0;
	virtual int cancel_application() = 0;
	virtual int reply_share(uint32_t uid, int reply) = 0;
	virtual int start_share() = 0;
	virtual int stop_share(uint32_t uid) = 0;
	virtual int get_share_users() = 0;
	virtual int start_tag() = 0;
	virtual int stop_tag(uint32_t uid) = 0;
	virtual int get_tag_users() = 0;
	virtual int cancel_share(uint32_t uid) = 0;
	virtual int send_p2p_msg(uint32_t dst_uid, const char *con, uint32_t con_len) = 0;
	virtual int send_p2all_msg(const char *con, uint32_t con_len) = 0;
	virtual int get_classing_user() = 0;
};

#endif // LIBPDU_H
