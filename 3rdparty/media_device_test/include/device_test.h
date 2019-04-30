#pragma once
#include <stdint.h>

#ifdef _MSC_VER
#if defined(LIBMEDIA_DEVICE_TEST_LIBRARY)
#  define LIBMEDIA_DEVICE_TEST_EXPORT __declspec(dllexport)
#else
#  define LIBMEDIA_DEVICE_TEST_EXPORT __declspec(dllimport)
#endif
#else
#  define LIBPDUSHARED_EXPORT
#endif

typedef void(*video_back)(void *private_data, uint8_t *rgb, int w, int h);

class LIBMEDIA_DEVICE_TEST_EXPORT video_test {
public:
	virtual ~video_test() {}
	static video_test *create(video_back back, void *arg);
	virtual int get_device_cnt() = 0;
	virtual int get_device_name(int index, char *buff, size_t buff_len) = 0;
	virtual int set_device_index(int index) = 0;
	virtual int start_test() = 0;
	virtual int stop_test() = 0;
};

typedef void(*audio_back)(void *private_data, uint16_t vol);

class LIBMEDIA_DEVICE_TEST_EXPORT audio_test {
public:
	virtual ~audio_test() {}
	static audio_test *create(audio_back back, void *arg);
	virtual int get_device_cnt() = 0;
	virtual int get_device_name(int index, char *buff, size_t buff_len) = 0;
	virtual int set_device_index(int index) = 0;
	virtual int start_test() = 0;
	virtual int stop_test() = 0;
};

class LIBMEDIA_DEVICE_TEST_EXPORT audio_playout_test {
public:
	virtual ~audio_playout_test() {}
	static audio_playout_test *create();
	virtual int get_device_cnt() = 0;
	virtual int get_device_name(int index, char *buff, size_t buff_len) = 0;
};
