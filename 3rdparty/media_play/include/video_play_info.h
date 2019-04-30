#ifndef __VIDEO_PLAY_INFO_H__
#define __VIDEO_PLAY_INFO_H__
#include <stdint.h>

class video_play {
public:
	virtual ~video_play() {}
	// 播放时间进度，单位：秒
	virtual void play_progress(uint32_t, uint32_t) = 0;
	virtual void video_data(const uint8_t *data, int w, int h) = 0;
};
#endif
