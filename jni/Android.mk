LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

#OPENCV_CAMERA_MODULES:=on
OPENCV_INSTALL_MODULES:=on
#OPENCV_LIB_TYPE := SHARED -lm -ljnigraphics -lui
include /Users/linqi/SDKDir/OpenCV-android-sdk/sdk/native/jni/OpenCV.mk

#LOCAL_SDK_VERSION := 9
#LOCAL_NDK_STL_VARIANT := gnustl_static

LOCAL_MODULE := libMFDenoise
LOCAL_LDFLAGS := -Wl,--build-id
LOCAL_LDLIBS := -llog -landroid -lGLESv2 -lEGL -lcutils -lui -lutils -lgui
LOCAL_CFLAGS :=  -DEGL_EGLEXT_PROTOTYPES -DGL_GLEXT_PROTOTYPES -DROCKCHIP_GPU_LIB_ENABLE -DHAVE_PTHREADS

LOCAL_C_INCLUDES += \
    $(LOCAL_PATH)/../sdk/native/jni/include \
    frameworks/native/include \
    hardware/libhardware/include \
    system/core/include

LOCAL_SRC_FILES := \
	MutliFrameDenoise.cpp \
	src/format.cpp \
	src/MyThread.cpp \
	src/OrbPatch.cpp \
	src/MutGetHomography.cpp \
	src/GetHomography.cpp \
	src/PerspectiveAdd.cpp\
	src/utils.cpp

LOCAL_C_INCLUDE := $(LOCAL_PATH)/../sdk/native/jni/include
LOCAL_C_INCLUDES += include
LOCAL_C_INCLUDES += ./
#LOCAL_SHARED_LIBRARIES := libopencv_java3
include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
#OPENCV_CAMERA_MODULES:=on
OPENCV_INSTALL_MODULES:=on
#OPENCV_LIB_TYPE := SHARED -lm -ljnigraphics -lui
include /Users/linqi/SDKDir/OpenCV-android-sdk/sdk/native/jni/OpenCV.mk

LOCAL_LDFLAGS := -Wl,--build-id -lskia -llog
LOCAL_CFLAGS :=  -DSK_SUPPORT_LEGACY_SETCONFIG

LOCAL_C_INCLUDES += $(LOCAL_PATH)
LOCAL_C_INCLUDES += \
    $(LOCAL_PATH)/../sdk/native/jni/include

LOCAL_C_INCLUDES += \
    external/skia/include/core \
    external/skia/include/effects \
    external/skia/include/images \
    external/skia/src/ports \
    external/skia/include/utils

LOCAL_SRC_FILES := \
	main.cpp

LOCAL_MODULE:= mfdenoise

LOCAL_SHARED_LIBRARIES := libMFDenoise libopencv_java3 libskia libcutils libutils
include $(BUILD_EXECUTABLE)
