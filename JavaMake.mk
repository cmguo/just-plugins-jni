# MakeJava.mk

# import vars
# SOURCE_DIRECTORY Java文件所在目录

#all: ppbox_jni.jar

com/pplive/sdk/%.class: $(ROOT_SOURCE_DIRECTORY)$(LOCAL_NAME)/%.java
	javac -d . $<

#PPBOX.h: com/pplive/sdk/PPBOX.class
#	javah -o PPBOX.h com.pplive.sdk.PPBOX

com/pplive/sdk/Test.class: com/pplive/sdk/PPBOX.class

ppbox_jni.jar: com/pplive/sdk/PPBOX.class com/pplive/sdk/Test.class
	jar cf ppbox_jni.jar com

