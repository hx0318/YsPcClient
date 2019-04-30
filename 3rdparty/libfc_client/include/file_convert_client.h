#ifndef __FILE_CONVERT_CLIENT_H__
#define __FILE_CONVERT_CLIENT_H__

#include <stdint.h>

#ifdef _MSC_VER
#ifdef FILE_CONVERT_CLIENT_LIBRARY
#define FCC_API __declspec(dllexport)
#else
#define FCC_API __declspec(dllimport)
#endif
#else
# define LOG_API
#endif

class fc_observer {
public:
	virtual ~fc_observer() {}
	virtual void fc_progress(int curr, int total, const char *fn) = 0;
	virtual void fc_error(int err = 0) = 0;
	virtual void fc_complete() = 0;
	virtual void fc_upload_progress(uint32_t curr, uint32_t total) = 0;
};

namespace sc {
	FCC_API void *create_fc_client(const char *ip, uint16_t port, const char *fn,
		const char *dst_dir, uint32_t uid, uint32_t rid, fc_observer *back);
	FCC_API void destory_fc_client(void *);
}

#endif //__FILE_CONVERT_CLIENT_H__