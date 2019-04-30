#ifndef __MEDIA_TYPE_H__
#define __MEDIA_TYPE_H__

typedef enum {
	NO_MEDIA = 0,
	COMMON_VIDEO = 0x00000001,       /* 摄像头视频数据 */
	COMMON_AUDIO = 0x00000002,       /* 音频数据 */
	SCREEN_DATA  = 0x00000004,       /* 屏幕数据 */
	FILE_VIDEO   = 0x00000008,       /* 文件视频数据 */
	FILE_AUDIO   = 0x00000010,       /* 文件音频数据 */
	BOOTHS_DATA  = 0x00000020,       /* 展台数据 */
} media_type_t;

#define PAYLOAD_TYPE_RED 100
#define PAYLOAD_TYPE_FEC 101

#endif // __MEDIA_TYPE_H__
