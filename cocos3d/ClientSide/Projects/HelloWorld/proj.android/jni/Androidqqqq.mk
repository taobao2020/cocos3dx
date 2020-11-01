LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := game_shared

LOCAL_MODULE_FILENAME := libgame

#����ĳ��Ŀ¼������cpp�ļ�����
define all-cpp-files-under
$(patsubst $(LOCAL_PATH)/%, %, $(shell find $(1) -maxdepth 3 -name "*.cpp" -and -not -name ".*") )
endef

LOCAL_SRC_FILES := hellocpp/main.cpp + $(call all-cpp-files-under, $(LOCAL_PATH)/../../Classes)
                   
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes                   

LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static \
	cocosdenshion_static \
	cocos_extension_static \
	mmUtil_static \
	zlib_static \
	cocoslib_static \
	hawkutil_static \
	tinyxml_static
            
#�������ļ�����				
#LOCAL_LDLIBS := 
#LOCAL_C_FLAGS += -fuse-ld=gold 
#LOCAL_CPP_FLAGS += -fuse-ld=gold 
#LOCAL_LDLIBS += -fuse-ld=gold

include $(BUILD_SHARED_LIBRARY)

$(call import-module,CocosDenshion/android) \
$(call import-module,cocos2dx) \
$(call import-module,extensions)
$(call import-module,mmUtil)
$(call import-module,cocoslib)
$(call import-module,hawkutil)
$(call import-module,tinyxml)