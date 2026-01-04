LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := dvtrga
LOCAL_SRC_FILES := dvtrga_android.c
LOCAL_LDLIBS    := -llog -landroid

include $(BUILD_SHARED_LIBRARY)
