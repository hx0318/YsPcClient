#pragma once

#include <stdint.h>

#ifdef _MSC_VER
#if defined(LIBCENTER_MEDIA_LIBRARY)
#  define LIBCENTER_MEDIA_EXPORT __declspec(dllexport)
#else
#  define LIBCENTER_MEDIA_EXPORT __declspec(dllimport)
#endif
#else
#  define LIBPDUSHARED_EXPORT
#endif

typedef enum {
	V_F_720P,
	V_F_1080P,
} video_format_t;

typedef struct {
	uint32_t uid;             // 教室ID
	uint8_t alive;            // 是否有视频
	char work[256];           // 职业
	char nickname[256];       // 昵称
} user_info_t;

typedef enum {
	RECORD_CONNECTED,
	RECORD_UNCONNECTED,
	RECORD_CONNECT_BREAK,
	RECORD_UPDATED,            // 文件上传完成
} record_msg_t;

typedef void(*record_back)(void *, record_msg_t, void *);

class LIBCENTER_MEDIA_EXPORT center_media {
public:
	virtual ~center_media() = default;
	static center_media *create(uint32_t rid, uint32_t uid, const char *mcuip,
								uint16_t mcu_port, video_format_t vf, bool only_data,
								const char *dstip, uint16_t dstport,
								record_back back, void *arg);
	virtual int start_record() = 0;
	virtual int stop_record() = 0;
	virtual int start_webcast(const char *url) = 0;
	virtual int stop_webcast() = 0;
	//virtual void push_picture(const char *fn) = 0;
	virtual void video_order_changed(user_info_t *info, int cnt = 9) = 0;
	virtual void record_info(const char *info, size_t len) = 0;
};
