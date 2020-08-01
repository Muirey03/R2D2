TARGET := iphone:clang:latest:7.0
INSTALL_TARGET_PROCESSES = R2D2
ARCHS = arm64

#DEBUG
GO_EASY_ON_ME = 1

include $(THEOS)/makefiles/common.mk

APPLICATION_NAME = R2D2
R2D2_FILES = $(wildcard *.m *.mm)
R2D2_FRAMEWORKS = UIKit CoreGraphics
R2D2_CFLAGS = -fobjc-arc -Iinclude/libr
R2D2_LDFLAGS = -Llib
R2D2_LIBRARIES = r_core

include $(THEOS_MAKE_PATH)/application.mk
