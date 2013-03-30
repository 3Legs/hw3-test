APP1 := net_monitor
APP2 := my_net_app
ROOT:=$(HOME)
NDK_PLATFORM_VER := 14
INSTALL_DIR := /data/tmp

ANDROID_SDK_ROOT:=$(HOME)/bin/android-sdk-linux
ANDROID_NDK_ROOT:=$(HOME)/bin/android-ndk-r8d
ANDROID_NDK_HOST:=linux-x86
ANDROID_TARGET:=i686-linux-android
ANDROID_TARGET_ARCH:=x86
ANDROID_TOOLCHAIN:=x86-4.4.3

BINDIR:=$(ANDROID_NDK_ROOT)/toolchains/$(ANDROID_TOOLCHAIN)/prebuilt/$(ANDROID_NDK_HOST)
LIBDIR:=$(ANDROID_NDK_ROOT)/platforms/android-$(NDK_PLATFORM_VER)/arch-$(ANDROID_TARGET_ARCH)/usr/lib
INCDIR:=$(ANDROID_NDK_ROOT)/platforms/android-$(NDK_PLATFORM_VER)/arch-$(ANDROID_TARGET_ARCH)/usr/include
BIN := $(BINDIR)/bin
 
CPP := $(BIN)/$(ANDROID_TARGET)-g++
CC := $(BIN)/$(ANDROID_TARGET)-gcc
CFLAGS := -I$(INCDIR)
LDFLAGS := -Wl,-rpath-link=$(LIBDIR),-dynamic-linker=/system/bin/linker -L$(LIBDIR) 
LDFLAGS += $(LIBDIR)/crtbegin_dynamic.o $(LIBDIR)/crtend_android.o -nostdlib -lc -disable-multilib -lm
 
 
all: $(APP1) $(APP2)
 
OBJS += $(APP).o
 
$(APP1): $(APP1).o
	$(CC) $(LDFLAGS) -o $@ $^
$(APP2): $(APP2).o
	$(CC) $(LDFLAGS) -o $@ $^
 
%.o: %.c
	$(CC) -c $(INCLUDE) $(CFLAGS) $< -o $@ 
install: $(APP1) $(APP2)
	$(ANDROID_SDK_ROOT)/platform-tools/adb push $(APP1) $(INSTALL_DIR)/$(APP1) 
	$(ANDROID_SDK_ROOT)/platform-tools/adb push $(APP2) $(INSTALL_DIR)/$(APP2) 
	$(ANDROID_SDK_ROOT)/platform-tools/adb shell chmod 777 $(INSTALL_DIR)/$(APP1)
	$(ANDROID_SDK_ROOT)/platform-tools/adb shell chmod 777 $(INSTALL_DIR)/$(APP2)
 
shell:
	$(ANDROID_SDK_ROOT)/platform-tools/adb shell
 
run:
	$(ANDROID_SDK_ROOT)/platform-tools/adb shell $(INSTALL_DIR)/$(APP)
 
r: $(APP)
	$(ANDROID_SDK_ROOT)/platform-tools/adb push $(APP) $(INSTALL_DIR)/$(APP) 
	$(ANDROID_SDK_ROOT)/platform-tools/adb shell chmod 777 $(INSTALL_DIR)/$(APP)
	$(ANDROID_SDK_ROOT)/platform-tools/adb shell $(INSTALL_DIR)/$(APP)
 
clean:
	@rm -f $(APP).o $(APP)

