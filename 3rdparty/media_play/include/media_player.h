#pragma once

#include "video_play_info.h"

#if defined(LIBMEDIAPLAYER_LIBRARY)
#  define LIBMEDIAPLAYER_SHARED_EXPORT __declspec(dllexport)
#else
#  define LIBMEDIAPLAYER_SHARED_EXPORT __declspec(dllimport)
#endif

class LIBMEDIAPLAYER_SHARED_EXPORT media_player {
public:
	virtual ~media_player() {}
	static media_player *create(const char *fn, video_play *vp, int id);

	virtual void play() = 0;
	// p:true 暂停, false 恢复
	virtual void pause(bool p) = 0;
	// p 长度百分比
	virtual void seek(int p) = 0;
	virtual void stop() = 0;
};
