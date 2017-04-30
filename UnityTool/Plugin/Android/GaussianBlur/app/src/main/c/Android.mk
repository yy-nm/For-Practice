
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
APP_ABI := all

LOCAL_MODULE    := GaussianBlur
LOCAL_CPPFLAGS     := -O2
# LOCAL_CPP_EXTENSION := .cpp
# LOCAL_LDLIBS    := -lm -llog -lz
LOCAL_SHORT_COMMANDS := true
# INC_DIRS = -I$(LOCAL_PATH)/jni
LOCAL_CPPFLAGS += $(INC_DIRS)

LOCAL_SRC_FILES    := GaussianBlur.c GaussianBlurMatrixGen.c

LOCAL_ARM_MODE := arm

# LOCAL_SHARED_LIBRARIES += libandroid_runtime


include $(BUILD_SHARED_LIBRARY)