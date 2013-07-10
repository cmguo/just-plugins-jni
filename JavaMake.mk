# MakeJava.mk

# SOURCE_DIRECTORY Java文件所在目录
SOURCE_DIRECTORY	:= $(ROOT_SOURCE_DIRECTORY)$(LOCAL_NAME)

#all: ppbox_jni.jar

FILES_JAVA		:= $(wildcard $(SOURCE_DIRECTORY)/*/*/*/*.java)
FILES_CLASS		:= $(patsubst $(SOURCE_DIRECTORY)/%,%,$(patsubst %.java,%.class,$(FILES_JAVA)))

$(FILES_CLASS): %.class: $(SOURCE_DIRECTORY)/%.java
	javac -d . $<

com/pplive/sdk/Test.class: com/pplive/sdk/PPBOX.class

ppbox_jni.jar: $(FILES_CLASS)
	jar cf ppbox_jni.jar com

