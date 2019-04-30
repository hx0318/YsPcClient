#ifndef __FILE__TRANSFER_H__
#define __FILE__TRANSFER_H__

#include <stdint.h>

#ifdef _MSC_VER
#if defined(LIBFILE_TRANSFER_LIBRARY)
#  define LIBFILE_TRANSFER_EXPORT __declspec(dllexport)
#else
#  define LIBFILE_TRANSFER_EXPORT __declspec(dllimport)
#endif
#else
#  define LIBFILE_TRANSFER_EXPORT
#endif

#define SEND_BUFF_LEN 4096

class trans_observer {
public:
	virtual ~trans_observer() {}
	virtual void progress(uint32_t curr, uint32_t total) = 0;
	virtual void trans_error(int err=0) = 0;
	virtual void complete() = 0;
};

class LIBFILE_TRANSFER_EXPORT k12upload {
public:
	static k12upload *create(trans_observer *obj, uint32_t uid, uint32_t rid,
		const char *fn, const char *token, const char *ip, uint16_t port);
	virtual ~k12upload() {}
};

class LIBFILE_TRANSFER_EXPORT k12download {
public:
	static k12download *create(trans_observer *obj, uint32_t uid, uint32_t rid,
		uint32_t dst_uid, const char *fn, const char *token, const char *ip, uint16_t port);
	virtual ~k12download() {}
};

#endif // !__FILE__TRANSFER_H__
