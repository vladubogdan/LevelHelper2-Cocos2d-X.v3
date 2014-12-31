#APP_STL := c++_static
APP_STL := gnustl_static
NDK_TOOLCHAIN_VERSION=clang

#use chipmunk
#APP_CPPFLAGS := -frtti -DCC_ENABLE_CHIPMUNK_INTEGRATION=1 -std=c++11 -fsigned-char
#use box2d
APP_CPPFLAGS := -frtti -DLH_USE_BOX2D=1 -std=c++11 -fsigned-char

APP_LDFLAGS := -latomic

#compile for genymotion
APP_ABI := x86

#compile for device
#APP_ABI :=armeabi armeabi-v7a

APP_DEBUG := $(strip $(NDK_DEBUG))
ifeq ($(APP_DEBUG),1)
  APP_CPPFLAGS += -DCOCOS2D_DEBUG=1
  APP_OPTIM := debug
else
  APP_CPPFLAGS += -DNDEBUG
  APP_OPTIM := release
endif