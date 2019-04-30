#pragma once

#include "video_play_info.h"

#if defined(LIBCOURSEWARE_LIBRARY)
#  define LIBCOURSEWARE_SHARED_EXPORT __declspec(dllexport)
#else
#  define LIBCOURSEWARE_SHARED_EXPORT __declspec(dllimport)
#endif

// 录制时间进度，单位：秒
typedef void(*record_progress)(uint32_t, void *);

namespace courseware {
class LIBCOURSEWARE_SHARED_EXPORT courseware_record {
public:
	virtual ~courseware_record() {}
	static courseware_record *create(const char *fn, int w, int h, 
		int fps, record_progress rp, void *userdata);

	virtual void push_picture(const uint8_t *data, int w, int h, int stride) = 0;
};

class LIBCOURSEWARE_SHARED_EXPORT courseware_play {
public:
	virtual ~courseware_play() {}
	static courseware_play *create(const char *fn, video_play *vp, int id);
	static void delete_video_data(const uint8_t *data);

	virtual void play() = 0;
	// p:true 暂停, false 恢复
	virtual void pause(bool p) = 0;
	// p 长度百分比
	virtual void seek(int p) = 0;
	virtual void stop() = 0;
};
}
