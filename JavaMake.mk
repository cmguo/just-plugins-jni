# MakeJava.mk

# SOURCE_DIRECTORY Java文件所在目录
SOURCE_DIRECTORY	:= $(ROOT_DIRECTORY)$(LOCAL_NAME)

FILES_JAVA		:= $(wildcard $(SOURCE_DIRECTORY)/*.java)

just_jni.jar: $(FILES_JAVA)
	javac -d . $(FILES_JAVA)
	jar cf just_jni.jar com

