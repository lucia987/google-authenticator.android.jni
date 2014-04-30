LOCAL_PATH := $(call my-dir)


# Prebuilt libcrypto
include $(CLEAR_VARS)
LOCAL_MODULE := crypto
LOCAL_SRC_FILES := prebuilt/openssl/libcrypto.so
include $(PREBUILT_SHARED_LIBRARY)

# Prebuilt libssl
include $(CLEAR_VARS)
LOCAL_MODULE := ssl
LOCAL_SRC_FILES := prebuilt/openssl/libssl.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := GoogleAuthenticator
LOCAL_SRC_FILES := com_google_android_apps_authenticator_TotpCounter.c
LOCAL_SRC_FILES += com_google_android_apps_authenticator_AccountDb.c
LOCAL_C_INCLUDES += $(LOCAL_PATH)/includes
LOCAL_SHARED_LIBRARIES = crypto ssl
LOCAL_C_FLAGS := -g
LOCAL_LDLIBS := -llog
include $(BUILD_SHARED_LIBRARY)