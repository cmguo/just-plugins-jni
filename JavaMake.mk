# MakeJava.mk

# import vars
# SOURCE_DIRECTORY Java文件所在目录

all: MediaSDK.h ppbox_jni.jar

com/pplive/sdk/MediaSDK.class: $(SOURCE_DIRECTORY)/MediaSDK.java
	javac -d . $<

MediaSDK.h: com/pplive/sdk/MediaSDK.class
	javah -o MediaSDK.h com.pplive.sdk.MediaSDK

ppbox_jni.jar: com/pplive/sdk/MediaSDK.class
	jar cf ppbox_jni.jar com

