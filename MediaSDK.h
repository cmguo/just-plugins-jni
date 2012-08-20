/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class com_pplive_sdk_MediaSDK */

#ifndef _Included_com_pplive_sdk_MediaSDK
#define _Included_com_pplive_sdk_MediaSDK
#ifdef __cplusplus
extern "C" {
#endif
#undef com_pplive_sdk_MediaSDK_LEVEL_ERROR
#define com_pplive_sdk_MediaSDK_LEVEL_ERROR 0L
#undef com_pplive_sdk_MediaSDK_LEVEL_ALARM
#define com_pplive_sdk_MediaSDK_LEVEL_ALARM 1L
#undef com_pplive_sdk_MediaSDK_LEVEL_EVENT
#define com_pplive_sdk_MediaSDK_LEVEL_EVENT 2L
#undef com_pplive_sdk_MediaSDK_LEVEL_INFOR
#define com_pplive_sdk_MediaSDK_LEVEL_INFOR 3L
#undef com_pplive_sdk_MediaSDK_LEVEL_DEBUG
#define com_pplive_sdk_MediaSDK_LEVEL_DEBUG 4L
#undef com_pplive_sdk_MediaSDK_LEVEL_DEBUG1
#define com_pplive_sdk_MediaSDK_LEVEL_DEBUG1 5L
#undef com_pplive_sdk_MediaSDK_LEVEL_DEBUG2
#define com_pplive_sdk_MediaSDK_LEVEL_DEBUG2 6L
/*
 * Class:     com_pplive_sdk_MediaSDK
 * Method:    startP2PEngine
 * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)J
 */
JNIEXPORT jlong JNICALL Java_com_pplive_sdk_MediaSDK_startP2PEngine
  (JNIEnv *, jclass, jstring, jstring, jstring);

/*
 * Class:     com_pplive_sdk_MediaSDK
 * Method:    stopP2PEngine
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_com_pplive_sdk_MediaSDK_stopP2PEngine
  (JNIEnv *, jclass);

/*
 * Class:     com_pplive_sdk_MediaSDK
 * Method:    fileDownloadOpenItem
 * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)J
 */
JNIEXPORT jlong JNICALL Java_com_pplive_sdk_MediaSDK_fileDownloadOpenItem
  (JNIEnv *, jclass, jstring, jstring, jstring);

/*
 * Class:     com_pplive_sdk_MediaSDK
 * Method:    fileDownloadCloseItem
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_com_pplive_sdk_MediaSDK_fileDownloadCloseItem
  (JNIEnv *, jclass, jlong);

/*
 * Class:     com_pplive_sdk_MediaSDK
 * Method:    getFileDownloadItemInfo
 * Signature: (JLcom/pplive/sdk/MediaSDK/Download_Statistic;)J
 */
JNIEXPORT jlong JNICALL Java_com_pplive_sdk_MediaSDK_getFileDownloadItemInfo
  (JNIEnv *, jclass, jlong, jobject);

#ifdef __cplusplus
}
#endif
#endif
/* Header for class com_pplive_sdk_MediaSDK_Download_Statistic */

#ifndef _Included_com_pplive_sdk_MediaSDK_Download_Statistic
#define _Included_com_pplive_sdk_MediaSDK_Download_Statistic
#ifdef __cplusplus
extern "C" {
#endif
#ifdef __cplusplus
}
#endif
#endif
