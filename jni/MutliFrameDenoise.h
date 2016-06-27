#include <jni.h>
#include "string.h"
#include "assert.h"
#include <system/window.h>
#include <hardware/gralloc.h>

#ifndef MY_JNI_MYJNI_H
#define MY_JNI_MYJNI_H

enum OUTPUT_FMT {
	OUTPUT_FMT_CV_MAT,
	OUTPUT_FMT_YUV,
	OUTPUT_FMT_RGBA,
	OUTPUT_NONE
};

struct cv_fimc_buffer {
	void	*start;
	int share_fd;
	size_t	length;
	int stride;
	size_t	bytesused;
	buffer_handle_t handle;
};

class MutliFrameDenoise {
	public:
		bool initialized;
		MutliFrameDenoise();
		~MutliFrameDenoise();
		void initOpenGLES(alloc_device_t *m_alloc_dev, int width, int height);
		void updateImageData(struct cv_fimc_buffer *m_buffers_capture);
		long processing(int* targetAddr, int mode = OUTPUT_NONE);
		int getResult(int targetAddr);
		void destroy();
	private:
		void getImageUnderDir(char *path, char *suffix);

};
#endif //MY_JNI_MYJNI_H