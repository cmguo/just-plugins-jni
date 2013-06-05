# MakeJava.mk

# import vars
# SOURCE_DIRECTORY Java文件所在目录

#all: ppbox_jni.jar

com/pplive/sdk/PPBOX.class: $(ROOT_SOURCE_DIRECTORY)$(LOCAL_NAME)/PPBOX.java
	javac -d . $<

#PPBOX.h: com/pplive/sdk/PPBOX.class
#	javah -o PPBOX.h com.pplive.sdk.PPBOX

ppbox_jni.jar: com/pplive/sdk/PPBOX.class
	jar cf ppbox_jni.jar com

