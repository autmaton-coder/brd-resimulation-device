LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := brdresimulationdevice
LOCAL_SRC_FILES := main.c gleng.c pheng.c mymloc.c
LOCAL_LDLIBS    := -landroid -lEGL -lGLESv2
LOCAL_STATIC_LIBRARIES := android_native_app_glue

include $(BUILD_SHARED_LIBRARY)

$(call import-module,android/native_app_glue)
