// PPBOX.cpp

#include "plugins/jni/Common.h"
#include "plugins/jni/PPBOX.h"
#include "plugins/jni/JniStruct.h"
#include "plugins/jni/JniClass.h"

#define PPBOX_JNI_PREFIX_ Java_com_pplive_sdk_
#define PPBOX_DISABLE_AUTO_START
#define PPBOX_NO_UNION

#ifdef __ANDROID__
#  include <android/log.h>
#  define LOG(level, ...) __android_log_print(level  , "PPBOX", __VA_ARGS__)
#else
#  define LOG(level, ...) printf(__VA_ARGS__); printf("\n")
#endif

#ifdef WIN32
#  define setenv(n, v, f) SetEnvironmentVariableA(n ,v)
#  define getenv(n, v) (GetEnvironmentVariableA(n ,v, sizeof(v)), v)
#else
#  define getenv(n, v) (getenv)(n)
#endif

#include "plugins/ppbox/ppbox_runtime.h"

void Ppbox_OnLogDump(char const * log, PP_uint level){
    if(log != NULL){
        LOG(level, "%s", log);
    }
}

bool g_logOn = false;

static void ppbox_redirect_callback(
    PP_context context, 
    PP_handle callback, 
    void * result, 
    va_list args)
{
    __JniCallback * c = (__JniCallback *)callback;
    c->invoke((jclass)context, result, args);
}

static void ppbox_free_callback(
       PP_handle callback)
{
    __JniCallback * c = (__JniCallback *)callback;
    delete c;
}

extern "C" JNIEXPORT jint JNICALL JNI_OnLoad(
    JavaVM *vm, 
    void *reserved)
{
    LOG(3, "[JNI_OnLoad] begin");

    JNIEnv* env = NULL; 
    if (vm->GetEnv((void**) &env, JNI_VERSION_1_6) != JNI_OK) { 
        LOG(1, "[JNI_OnLoad] GetEnv failed\n"); 
        return JNI_ERR;
    }

    JniClass clsMediaSdk(env, "com/pplive/sdk/PPBOX");

    string_holder libPath = clsMediaSdk.static_field_cvalue<JString>("libPath");
    LOG(3, "libPath = %s", libPath.c_str());
    char strlib[1024] = {0};
    strncpy(strlib, libPath, sizeof(strlib));
    strncat(strlib, "/", sizeof(strlib));
    strncat(strlib, PPBOX_LIB_NAME, sizeof(strlib));
    PPBOX_Load(strlib);

    string_holder logPath = clsMediaSdk.static_field_cvalue<JString>("logPath");
    LOG(3, "logPath = %s", logPath.c_str());
    setenv("TMPDIR", libPath, 1);

    bool logOn = g_logOn = clsMediaSdk.static_field_cvalue<JBoolean>("logOn");
    if (logOn) {
        LOG(3, "logOn = %s", logOn ? "true" : "false");
        long logLevel = clsMediaSdk.static_field_cvalue<JInt>("logLevel");
        LOG(3, "logLevel = %d", logLevel);
        PPBOX_LogDump(Ppbox_OnLogDump, logLevel);
    }

    PPBOX_RedirectCallback(ppbox_redirect_callback, ppbox_free_callback, clsMediaSdk.get_class());

    LOG(3, "[JNI_OnLoad] finish");

    return JNI_VERSION_1_6;
}

extern "C" JNIEXPORT void JNICALL JNI_OnUnload(
    JavaVM *vm, 
    void *reserved)
{
    LOG(3, "[JNI_OnUnload] begin");

    PPBOX_Unload();

    if (g_logOn) {
        // Ppbox_LogDump(NULL, 0);
    }

    LOG(3, "[JNI_OnUnload] finish");
}
