#pragma once

#include <stdint.h>

#ifdef _MSC_VER
#if defined(LIBVOICE_SMS_LIBRARY)
#  define LIBVOICE_SMS_SHARED_EXPORT __declspec(dllexport)
#else
#  define LIBVOICE_SMS_SHARED_EXPORT __declspec(dllimport)
#endif
#else
#  define LIBVOICE_SMS_SHARED_EXPORT
#endif

class notify_progress2ui {
public:
	virtual ~notify_progress2ui() {}
	virtual void progress(uint32_t curr, uint32_t total) = 0;
};

class LIBVOICE_SMS_SHARED_EXPORT libvoice_sms_record
{
public:
	static libvoice_sms_record *create(const char *file_name, uint16_t dev_id);
	virtual ~libvoice_sms_record() {}
};

class LIBVOICE_SMS_SHARED_EXPORT libvoice_sms_play
{
public:
	static libvoice_sms_play *create(notify_progress2ui *back, const char *file_name, int devid);
	virtual ~libvoice_sms_play() {}

	virtual int pause(int flag) = 0; // flag 1:暂停, 2:恢复
	virtual int seek(int per) = 0;
	virtual int stop() = 0;
	virtual int get_time_length() = 0;
};
