#pragma once

#include <stdint.h>
#include "media_type.h"

enum k12_media_device_state {
	K12_MEDIA_DEV_OK,         // 正常
	K12_MEDIA_DEV_BUSY,       // 设备忙
	K12_MEDIA_DEV_INEXISTENT, // 设备不存在
};

/* 角色定义 */
enum user_role_t {
	TEACHER_ROLE,                    /* 老师 */
	STUDENT_ROLE,                    /* 学生 */
	PATRIARCH_ROLE,                  /* 家长 */
	OTHER_ROLE,                      /* 其他 */
};

/* 共享类型 */
enum share_content_t {
	SHARE_PICTURE,                   /* 共享图片 */
	SHARE_OTHER,                     /* 共享其他 */
};

enum k12mcu2ui_type {
	//K12MCU2UI_HAND_UP,                 /* 学生举手 uint32_t */
	//K12MCU2UI_HAND_DOWN,               /* 学生放手 uint32_t */
	K12MCU2UI_NEW_M_COMING = 2,            /* 有新媒体上来 struct:user_media_t */
	K12MCU2UI_CLOSE_MEDIA,             /* 媒体被关闭 struct:user_media_t */
	K12MCU2UI_USER_ENTER_ROOM,         /* 用户进入房间 user_media_t */
	K12MCU2UI_USER_LEAVE_ROOM,         /* 用户离开房间 uint32_t */
	K12MCU2UI_OPEN_USER_MEDIA_STATE,   /* 打开指定用户媒体返回的状态
										struct:user_media_state_t */
	K12MCU2UI_DISCONNECT_FROM_MCU,     /* 与mcu断开连接 */
	K12MCU2UI_GET_USER_MEDIA_DEV_INFO, /* 返回指定用户媒体 struct:user_media_t */
	K12MCU2UI_GET_USER_LIST,           /* 返回房间内用户列表 struct:user_list_t */
	K12MCU2UI_PLAY_PROGRESS,           /* 媒体播放进度 struct:play_progress_t */
	K12MCU2UI_CONNECT2MCU_STATE,       /* 与MCU连接报告 参数int: 1成功, 0失败 */
	K12MCU2UI_VIDEO_DATA,              /* 视频数据 struct:k12mcu2ui_video_data_t */
	K12MCU2UI_EXIST_MEDIA_INFO,        /* 已存在媒体信息 struct:exist_user_media_t */
	//K12MCU2UI_APPLICATION_VIDEO,       /* 申请第二路视频 struct:user_media_t */
	//K12MCU2UI_CANCEL_APPLICATION_VIDEO,/* 取消申请第二路视频 struct:user_media_t */
	//K12MCU2UI_APPLICATION_VIDEO_ACK,   /* 申请第二路视频的回复
	//									struct:application_video_ack_t */
	K12MCU2UI_RECORD_VOLUME = 17,           /* 录音设备声音能量 uint16_t */
	K12MCU2UI_GET_TAG_DATA,            /* 获取标注的数据 ui2pdu_img_data_t * */
	K12MCU2UI_RECORD_PROGRESS,         /* 录制进度 uint32_t */
	K12MCU2UI_GET_CAMERA_TIMEOUT,      /* 获取摄像头数据超时 */
};

enum libpdu_type {
	//LIBPDU_HAND_UP,                     /* 举手 */
	//LIBPDU_HAND_DOWN,                   /* 放手 */
	LIBPDU_GET_MEDIA_DEV_CNT = 2,           /* 获取媒体设备数量 media_type_t */
	LIBPDU_GET_MEDIA_NAME,              /* 获取媒体设备名称 struct:media_name_t */
	LIBPDU_SET_MEDIA_DEV_INDEX,         /* 设置媒体设备 struct:media_dev_index_t */
	LIBPDU_OPEN_USER_MEDIA,             /* 打开指定用户媒体 struct:user_media_t */
	LIBPDU_CLOSE_USER_MEDIA,            /* 关闭指定用户媒体 struct:user_media_t */
	//LIBPDU_START_RECORD,                /* 开启录制 struct:ui_record_t */
	//LIBPDU_STOP_RECORD,                 /* 停止录制 */
	LIBPDU_GET_USER_LIST = 9,               /* 获取房间内用户列表 */
	//LIBPDU_PLAY_FILE,                   /* 播放视频 char * */
	//LIBPDU_PAUSE_FILE,                  /* 暂停播放 */
	//LIBPDU_STOP_FILE,                   /* 停止播放 */
	LIBPDU_GET_USER_MEDIA_DEV_INFO = 13,     /* 获取指定用户的媒体设备 uint32_t */
	LIBPDU_START_SHARE_SCREEN,          /* 开始屏幕共享 struct:share_t */
	LIBPDU_STOP_SHARE_SCREEN,           /* 结束屏幕共享 */
	LIBPDU_DESTORY_VIDEO_DATA,          /* 销毁视频数据 uint8_t * */
	LIBPDU_GET_VIDEO,                   /* 获取视频 struct:user_media_t */
	LIBPDU_CANCEL_VIDEO,                /* 取消视频 struct:user_media_t */
	//LIBPDU_APPLICATION_VIDEO,           /* 申请第二路视频 media_type_t */
	//LIBPDU_CANCEL_APPLICATION_VIDEO,    /* 取消申请第二路视频 media_type_t */
	//LIBPDU_APPLICATION_VIDEO_ACK,       /* 申请第二路视频的回复
	//									 struct:application_video_ack_t */
	//LIBPDU_START_VOICE_SMS_RECORD,      /* 开启音频短信录制 char * */
	//LIBPDU_STOP_VOICE_SMS_RECORD,       /* 关闭音频短信录制 */
	//LIBPDU_PLAY_VOICE_SMS,              /* 播放音频短信 char * */
	//LIBPDU_PAUSE_VOICE_SMS,             /* 暂停语音信息播放 int 1:暂停, 2:恢复 */
	//LIBPDU_SEEK_VOICE_SMS,              /* 拖动 int 百分比 */
	//LIBPDU_STOP_VOICE_SMS,              /* 停止播放音频短信 */
	LIBPDU_MEDIA_TIME = 22,                  /* 获取媒体时长 char * */
	LIBPDU_GET_PLAYOUT_CNT,             /* 获取媒体设备数量 */
	LIBPDU_GET_PLAYOUT_NAME,            /* 获取媒体设备名称 struct:playout_name_t */
	LIBPDU_SET_PLAYOUT_INDEX,           /* 设置媒体设备 int */
	LIBPDU_GET_RECORD_VOLUME,           /* 获取录音设备声音能量 int  */
	LIBPDU_CANCEL_RECORD_VOLUME,        /* 取消录音设备声音能量 */
	LIBPDU_START_TAGGING,               /* 开始标注 */
	LIBPDU_STOP_TAGGING,                /* 停止标注 */
	LIBPDU_START_GATHER,                /* 开始采集 struct:share_t */
	LIBPDU_STOP_GATHER,                 /* 停止采集 */
	LIBPDU_START_GET_MEDIA,             /* 开始获取媒体数据 */
	LIBPDU_STOP_GET_MEDIA,              /* 停止获取媒体数据 */
};

#pragma pack(1)

/* PDU-->UI */
typedef struct {
	uint32_t userid;
	media_type_t t;
	int flag;   // 0:拒绝,1:同意
} application_video_ack_t;

typedef struct {
	uint32_t userid;
	k12_media_device_state state;
	media_type_t t;
} user_media_state_t;

typedef struct {
	uint32_t userid;
	uint32_t t;
} user_media_t;

typedef struct {
	uint32_t cnt;
	user_media_t *user_media;
} exist_user_media_t;

typedef struct {
	user_media_t *user;
	uint32_t cnt;
} user_list_t;

typedef struct {
	uint32_t curr;
	uint32_t total;
	int id;
} play_progress_t;

typedef struct {
	int w;
	int h;
	uint8_t *data;
	uint32_t userid;
	media_type_t type;
} k12mcu2ui_video_data_t;

typedef struct {
	int w;
	int h;
	uint8_t *data; // RGBA数据, 内存已申请
} ui2pdu_img_data_t;

/* UI-->PDU */
typedef struct {
	media_type_t t;
	uint16_t index;
	char name[128];
} media_name_t;

typedef struct {
	media_type_t t;
	uint16_t index;
} media_dev_index_t;

typedef struct {
	int index;
	char name[128];
} playout_name_t;

typedef struct {
	char *filename;
	void *wnd;
	int w;
	int h;
} ui_record_t;

typedef struct {
	void *arg;
	share_content_t t;
} share_t;

#pragma pack()
