// MediaSDK.cpp

#include "plugins/jni/Common.h"
#include "plugins/jni/Version.h"

#include <ppbox/ppbox/IDemuxer.h>
#include <ppbox/ppbox/IDownloader.h>
#include <ppbox/ppbox/Name.h>

#undef JNIEXPORT
#define JNIEXPORT __attribute__ ((visibility("default")))

#ifdef __ANDROID__
#  include <android/log.h>
#endif
#include <jni.h>
#include <dlfcn.h>

#include "plugins/jni/MediaSDK.h"

extern "C" {

typedef void (* JNI_PPBOX_OnLogDump)( char const *, long );
typedef void (* JNI_PPBOX_LogDump)(JNI_PPBOX_OnLogDump callback, long );

typedef int (* JNI_PPBOX_StartP2PEngine)(char const *, char const *, char const *);
typedef int (* JNI_PPBOX_StopP2PEngine)();

typedef int (* JNI_PPBOX_DownloadOpen)(char const *, char const *, char const *, PPBOX_Download_Callback);
typedef void (* JNI_PPBOX_DownloadClose)(PPBOX_Download_Handle );
typedef int (* JNI_PPBOX_GetDownloadInfo)(PPBOX_Download_Handle, PPBOX_DownloadStatistic *);

void * handle = NULL;

JNI_PPBOX_LogDump J_PPBOX_LogDump = NULL;

JNI_PPBOX_StartP2PEngine J_PPBOX_StartP2PEngine = NULL;
JNI_PPBOX_StopP2PEngine J_PPBOX_StopP2PEngine = NULL;

JNI_PPBOX_DownloadOpen J_PPBOX_DownloadOpen = NULL;
JNI_PPBOX_DownloadClose J_PPBOX_DownloadClose = NULL;
JNI_PPBOX_GetDownloadInfo J_PPBOX_GetDownloadInfo = NULL;


//log日志

#ifdef __ANDROID__
#  define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG  , "P2PENGINE_TEST",__VA_ARGS__)
#else
#  define LOGD(...) printf(__VA_ARGS__)
#endif


    bool canStop = false;
    bool init_done = false;
 
    void Ppbox_OnLogDump( char const * log, long level){
        if(log != NULL){
            LOGD("%s", log);
        }
    }

    char* js2c(JNIEnv* env, jstring jstr)
    {
        char* rtn = NULL;
        jclass clsstring = env->FindClass("java/lang/String");
        jstring strencode = env->NewStringUTF("utf-8");
        jmethodID mid = env->GetMethodID(clsstring, "getBytes",
            "(Ljava/lang/String;)[B");
        jbyteArray barr = (jbyteArray) env->CallObjectMethod(jstr, mid, strencode);
        jsize alen = env->GetArrayLength(barr);
        jbyte* ba = env->GetByteArrayElements(barr, JNI_FALSE);
        if (alen > 0) {
            rtn = (char*) malloc(alen + 1);
            memcpy(rtn, ba, alen);
            rtn[alen] = 0;
        }
        env->ReleaseByteArrayElements(barr, ba, 0);

        env->DeleteLocalRef(clsstring);
        env->DeleteLocalRef(strencode);
        env->DeleteLocalRef(barr);
        return rtn;
    }

#define PPBOX_LIBNAME ppbox::name_string() 

    JNIEXPORT jint JNICALL JNI_OnLoad(
        JavaVM *vm, void *reserved)
    {
        LOGD("JNI_OnLoad");

        JNIEnv* env = NULL; 
        if (vm->GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK) { 
            LOGD("ERROR: GetEnv failed\n"); 
            return JNI_ERR;
        } 

        jfieldID lib_path = env->GetStaticFieldID(thiz, "libPath", "Ljava/lang/String;");
        jfieldID log_path = env->GetStaticFieldID(thiz, "logPath", "Ljava/lang/String;");
        jfieldID logopen = env->GetStaticFieldID(thiz, "logOn", "Z");
        jfieldID loglevel = env->GetStaticFieldID(thiz, "logLevel", "I");
        
        jstring libpa = (jstring)(env->GetStaticObjectField(thiz, lib_path));
        jstring logpa = (jstring)(env->GetStaticObjectField(thiz, log_path));
        bool log_on = env->GetStaticBooleanField(thiz, logopen);
        int log_level = env->GetStaticIntField(thiz, loglevel);
        
        char strEnv[1024] = {0};
        char *logpath = js2c(env,logpa);
        LOGD("TMPDIR=%s", strEnv);
        setenv("TMPDIR", logpath, 1);

        char * pwd = getenv("PWD");
        LOGD("PWD=%s", pwd);
        char* libPath = js2c(env,libpa);
        char * libraryPath = getenv("LD_LIBRARY_PATH");
        if (libraryPath == NULL || strstr(libraryPath,libPath) == NULL){
            char strlibpathEnv[1024] = {0};
            strncat(strlibpathEnv,libPath,1024);
            strncat(strlibpathEnv,":",1024);
            if (libraryPath)
                strncat(strlibpathEnv,libraryPath,1024);
            LOGD("LD_LIBRARY_PATH=%s", strlibpathEnv);
            setenv("LD_LIBRARY_PATH", strlibpathEnv, 1);
        } else {
            LOGD("LD_LIBRARY_PATH=%s", libraryPath);
        }

        handle = dlopen(PPBOX_LIBNAME ,RTLD_LAZY);
        if (handle == NULL) {
            LOGD("dlopen: %s", dlerror());
            char strlib[1024] = {0};
            strncat(strlib, libPath, 1024);
            strncat(strlib, "/", 1024);
            strncat(strlib, PPBOX_LIBNAME, 1024);
            handle = dlopen(strlib, RTLD_LAZY);
            if (handle == NULL){
                LOGD("dlopen: %s", dlerror());
            }
        }

        J_PPBOX_LogDump = (JNI_PPBOX_LogDump)dlsym(handle ,"PPBOX_LogDump");
        
        J_PPBOX_StartP2PEngine = (JNI_PPBOX_StartP2PEngine)dlsym(handle ,"PPBOX_StartP2PEngine");
        if (NULL == J_PPBOX_StartP2PEngine) {
            LOGD("PPBOX_StartP2PEngine not exist!");
        }
        J_PPBOX_StopP2PEngine = (JNI_PPBOX_StopP2PEngine)dlsym(handle ,"PPBOX_StopP2PEngine");
        if (NULL == J_PPBOX_StopP2PEngine) {
            LOGD("PPBOX_StopP2PEngine not exist!");
        }
        J_PPBOX_DownloadOpen = (JNI_PPBOX_DownloadOpen)dlsym(handle, "PPBOX_DownloadOpen");
        if (NULL == J_PPBOX_DownloadOpen) {
            LOGD("PPBOX_DownloadOpen not exist!");
        }
        J_PPBOX_DownloadClose = (JNI_PPBOX_DownloadClose)dlsym(handle, "PPBOX_DownloadClose");
        if (NULL == J_PPBOX_DownloadClose) {
            LOGD("PPBOX_DownloadClose not exist!");
        }
        J_PPBOX_GetDownloadInfo = (JNI_PPBOX_GetDownloadInfo)dlsym(handle, "PPBOX_GetDownloadInfo");
        if (NULL == J_PPBOX_GetDownloadInfo) {
            LOGD("PPBOX_GetDownloadInfo not exist!");
        }
        
        if (J_PPBOX_LogDump && log_on) {
            J_PPBOX_LogDump(&Ppbox_OnLogDump, log_level);
        }

        if (libPath != NULL){
            free(libPath);
        }
        if (logpath != NULL){
            free(logpath);
        }

        LOGD("JNI_OnLoad finish!");

        return JNI_VERSION_1_4;
    }

    JNIEXPORT void JNICALL JNI_OnUnload(
        JavaVM *vm, void *reserved)
    {
        LOGD("JNI_OnUnload");

        if (J_PPBOX_LogDump != NULL){
            J_PPBOX_LogDump(NULL, 0);
        }

        if (handle)
            dlclose(handle);

        LOGD("JNI_OnLoad finish!");
    }

    JNIEXPORT jlong JNICALL Java_com_pplive_sdk_MediaSDK_startP2PEngine (
        JNIEnv *env, jclass thiz,jstring gid,jstring pid,jstring auth)
    {
        MediaSDK_init(env, thiz);

        if (J_PPBOX_StartP2PEngine == NULL){
            return -1;
        }

        char* pgid = js2c(env,gid);
        //LOGD(pgid);
        char* ppid = js2c(env,pid);
        //LOGD(ppid);
        char* pauth = js2c(env,auth);
        //LOGD(auth);
        LOGD("PPBOX_StartP2PEngine !");
        long ret = J_PPBOX_StartP2PEngine(pgid,ppid,pauth);
        if (pgid != NULL){
            free(pgid);
        }
        if (ppid != NULL){
            free(ppid);
        }
        if (pauth != NULL){
            free(pauth);
        }
        LOGD("PPBOX_StartP2PEngine ! finish %ld", ret);
        canStop = true;
        return ret;
    }

    JNIEXPORT jlong JNICALL Java_com_pplive_sdk_MediaSDK_stopP2PEngine(
        JNIEnv *env, jclass thiz)
    {
        if (!canStop){
            return 0;
        }
        LOGD("PPBOX_stopP2PEngine !");
        if (NULL == J_PPBOX_StopP2PEngine) {
            return -1;
        }
        J_PPBOX_StopP2PEngine();
        LOGD("PPBOX_stopP2PEngine ! finish");
        return 0;
    }

    //需返回当前打开的下载用例的handle
    JNIEXPORT jlong JNICALL Java_com_pplive_sdk_MediaSDK_fileDownloadOpenItem(
        JNIEnv *env, jclass thiz, jstring gid, jstring pid, jstring auth)
    {
        if (NULL == J_PPBOX_DownloadOpen) {
            return -1;
        }
        LOGD("MediaSDK_DownloadOpen !");
        char * pgid = js2c(env, gid);
        char * ppid = js2c(env, pid);
        char * pauth = js2c(env, auth);
        PPBOX_Download_Handle handle;
        
        handle =(PPBOX_Download_Handle) J_PPBOX_DownloadOpen(pgid, ppid, pauth, NULL);

        if (pgid != NULL){
            free(pgid);
        }
        if (ppid != NULL){
            free(ppid);
        }
        if (pauth != NULL){
            free(pauth);
        }

        return (long)(handle);
    }

    //根据handle关闭特定下载用例
    JNIEXPORT void JNICALL Java_com_pplive_sdk_MediaSDK_fileDownloadCloseItem(
        JNIEnv *env, jclass thiz, jlong handle)
    {
        if (NULL == J_PPBOX_DownloadClose) {
            return ;
        }
        LOGD("MediaSDK_DownloadClose !");
        PPBOX_Download_Handle hdle = (PPBOX_Download_Handle)handle;
        J_PPBOX_DownloadClose(hdle);
    }

    //根据handle获取特定下载用例的info
    JNIEXPORT jlong JNICALL Java_com_pplive_sdk_MediaSDK_getFileDownloadItemInfo(
        JNIEnv *env, jclass thiz, jlong handle, jobject stat)
    {
        if (NULL== J_PPBOX_GetDownloadInfo) {
            return -1;
        }
        LOGD("MediaSDK_GetDownloadInfo !");
        PPBOX_DownloadStatistic dload_stat;
        PPBOX_Download_Handle hdle = (PPBOX_Download_Handle)handle;
        long ret = J_PPBOX_GetDownloadInfo(hdle, &dload_stat);
        //查找内部类需要用到$符号表示
        jclass clss = env->GetObjectClass(stat);
        jfieldID total_id = env->GetFieldID(clss, "total_size", "J");
        jfieldID finish_id = env->GetFieldID(clss, "finish_size", "J");
        jfieldID speed_id = env->GetFieldID(clss, "speed", "I");

        long total_size = dload_stat.total_size;
        env->SetLongField(stat, total_id, total_size);
        long finish_size = dload_stat.finish_size;
        env->SetLongField(stat, finish_id, finish_size);
        int speed = dload_stat.speed;
        env->SetIntField(stat, speed_id, speed);

        return ret;
    }

}
