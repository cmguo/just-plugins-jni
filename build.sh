#!/bin/sh

javac -d . MediaSDK.java
javah -o MediaSDK.h com.pplive.sdk.MediaSDK
jar cvf ppbox_jni.jar com
rm -rf com
mv ppbox_jni.jar ../../../include/ppbox
