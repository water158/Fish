LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../../cocos2d)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/external)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/cocos)

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp

LOCAL_SRC_FILES := hellocpp/main.cpp \
                   ../../Classes/AppDelegate.cpp \
                   ../../Classes/BossCoinPrompt.cpp \
                   ../../Classes/Bullet.cpp \
                   ../../Classes/CannonBase.cpp \
                   ../../Classes/Coin.cpp \
                   ../../Classes/CoinPrompt.cpp \
                   ../../Classes/CountdownLayer.cpp \
                   ../../Classes/EnergyCannonCard.cpp \
                   ../../Classes/Fire.cpp \
                   ../../Classes/FishBase.cpp \
                   ../../Classes/FishBezier.cpp \
                   ../../Classes/FishBoom.cpp \
                   ../../Classes/FishGroup.cpp \
                   ../../Classes/FishMedol.cpp \
                   ../../Classes/FishPath.cpp \
                   ../../Classes/FishSpline.cpp \
                   ../../Classes/FiShTV.cpp \
                   ../../Classes/GiftLayer.cpp \
                   ../../Classes/LoadingLayer.cpp \
                   ../../Classes/LogoLayer.cpp \
                   ../../Classes/MainScene.cpp \
                   ../../Classes/Manager.cpp \
                   ../../Classes/ManualCannon.cpp \
                   ../../Classes/MenuLayer.cpp \
                   ../../Classes/MusicData.cpp \
                   ../../Classes/MyActionNew.cpp \
                   ../../Classes/MyButton.cpp \
                   ../../Classes/HttpInstance.cpp \
                   ../../Classes/MyMenu.cpp \
                   ../../Classes/MyTools.cpp \
                   ../../Classes/MyXmlParser.cpp \
                   ../../Classes/NetSprite.cpp \
                   ../../Classes/NextLevel.cpp \
                   ../../Classes/NoseeFish.cpp \
                   ../../Classes/NoticeBoss.cpp \
                   ../../Classes/PixCell.cpp \
                   ../../Classes/ProductType.cpp \
                   ../../Classes/PublicLayer.cpp \
                   ../../Classes/SanYuanSiXi.cpp \
                   ../../Classes/ScopeFish.cpp \
                   ../../Classes/ShiledLayer.cpp \
                   ../../Classes/SmallGameLayer.cpp \
                   ../../Classes/SpellNumber.cpp \
                   ../../Classes/Store.cpp \
                   ../../Classes/TelephoneFareFish.cpp \
                   ../../Classes/TestUIScene.cpp \
                   ../../Classes/tinystr.cpp \
                   ../../Classes/tinyxml.cpp \
                   ../../Classes/tinyxmlerror.cpp \
                   ../../Classes/tinyxmlparser.cpp \
                   ../../Classes/UserData.cpp \
                   ../../Classes/UserDataMission.cpp \
                   ../../Classes/NDKHelper/NDKHelper.cpp \
                   ../../Classes/NDKHelper/NDKCallbackNode.cpp \
                   ../../Classes/NDKHelper/jansson/dump.c \
                   ../../Classes/NDKHelper/jansson/error.c \
                   ../../Classes/NDKHelper/jansson/hashtable.c \
                   ../../Classes/NDKHelper/jansson/load.c \
                   ../../Classes/NDKHelper/jansson/memory.c \
                   ../../Classes/NDKHelper/jansson/pack_unpack.c \
                   ../../Classes/NDKHelper/jansson/strbuffer.c \
                   ../../Classes/NDKHelper/jansson/strconv.c \
                   ../../Classes/NDKHelper/jansson/utf.c \
                   ../../Classes/NDKHelper/jansson/value.c

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes

LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static

# LOCAL_WHOLE_STATIC_LIBRARIES += box2d_static
# LOCAL_WHOLE_STATIC_LIBRARIES += cocosbuilder_static
# LOCAL_WHOLE_STATIC_LIBRARIES += spine_static
 LOCAL_WHOLE_STATIC_LIBRARIES += cocostudio_static
 LOCAL_WHOLE_STATIC_LIBRARIES += cocos_network_static
 LOCAL_WHOLE_STATIC_LIBRARIES += cocos_extension_static


include $(BUILD_SHARED_LIBRARY)

$(call import-module,.)
$(call import-module,audio/android)

# $(call import-module,Box2D)
# $(call import-module,editor-support/cocosbuilder)
# $(call import-module,editor-support/spine)
 $(call import-module,editor-support/cocostudio)
 $(call import-module,network)
 $(call import-module,extensions)
