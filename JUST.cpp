// JUST.cpp

#include "plugins/jni/Common.h"
#include "plugins/jni/JUST.h"
#include "plugins/jni/JniStruct.h"
#include "plugins/jni/JniClass.h"
#include "plugins/jni/Version.h"

#define JUST_JNI_PREFIX_ Java_com_just_sdk_
#define JUST_DISABLE_AUTO_START
#define JUST_NO_UNION
#define JUST_LIBRARY_NOT_EXIST(x) LOG(3, "Library %s not found", x)
#define JUST_FUNCTION_NOT_EXIST(x) LOG(3, "Function %s not found", #x)

#ifdef __GNUC__
#  undef JNIEXPORT
#  define JNIEXPORT __attribute__ ((visibility("default")))
#endif

#ifdef __ANDROID__
#  include <android/log.h>
#  define LOG(level, ...) __android_log_print(level  , "JUST", __VA_ARGS__)
#else
#  define LOG(level, ...) printf(__VA_ARGS__); printf("\n")
#endif

#ifdef WIN32
#  define setenv(n, v, f) SetEnvironmentVariableA(n ,v)
char *getenv(const char *name)
{
    static char v[MAX_PATH];
    v[0] = 0;
    GetEnvironmentVariableA(name, v, sizeof(v));
    return v;
}
#endif

#define _JUST_EXPORT_I_DISPATCH_H_ // disable IDispatch
#define _JUST_EXPORT_I_PLUGIN_H_ // disable IPlugin
#include <just/export/IJustBoostTypes.h>
#include <just/export/IJustRuntime.h>

template <
    typename F
>
char const * JniCallback<F>::sig()
{
    static char str[64] = {0};
    if (str[0] == 0) {
        strncpy(str, "Lcom/just/sdk/JUST$", sizeof(str));
        strncat(str, name_str() + 6, sizeof(str));
        strncat(str, ";", sizeof(str));
    }
    return str;
}

void Just_OnLogDump(char const * log, PP_uint level){
    if(log != NULL){
        LOG(5, "%s", log);
    }
}

bool g_logOn = false;

static void just_redirect_callback(
    PP_context context, 
    PP_handle callback, 
    void * result, 
    va_list args)
{
    __JniCallback * c = (__JniCallback *)callback;
    c->invoke(result, args);
}

static void just_free_callback(
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

    JniClass clsMediaSdk(env, "com/just/sdk/JUST");

    string_holder libPath = clsMediaSdk.static_field_cvalue<JString>("libPath");
    string_holder logPath = clsMediaSdk.static_field_cvalue<JString>("logPath");
    bool logOn = g_logOn = clsMediaSdk.static_field_cvalue<JBoolean>("logOn");

    LOG(3, "libPath = %s", libPath.c_str());
    LOG(3, "logPath = %s", logPath.c_str());
    LOG(3, "logOn = %s", logOn ? "true" : "false");

    char * lib_path_old = getenv("LD_LIBRARY_PATH");
    LOG(3, "LD_LIBRARY_PATH = %s", lib_path_old);
    if (lib_path_old == NULL || strstr(lib_path_old, libPath) == NULL) {
        char str_lib_path[1024] = {0};
        strncat(str_lib_path, libPath, 1024);
        strncat(str_lib_path, ":", 1024);
        if (lib_path_old)
            strncat(str_lib_path, lib_path_old, 1024);
        setenv("LD_LIBRARY_PATH", str_lib_path, 1);
        LOG(3, "LD_LIBRARY_PATH = %s", str_lib_path);
    }

    setenv("TMPDIR", logPath, 1);

    if (JUST_Load() == NULL) {
        char strlib[1024] = {0};
        strncpy(strlib, libPath, sizeof(strlib));
        strncat(strlib, "/", sizeof(strlib));
        strncat(strlib, JUST_LIB_NAME, sizeof(strlib));
        if (JUST_Load(strlib) == NULL) {
            return JNI_ERR;
        }
    }

    LOG(3, "JUST Version: %s", JUST_GetVersion());

    if (logOn) {
        long logLevel = clsMediaSdk.static_field_cvalue<JInt>("logLevel");
        LOG(3, "logLevel = %ld", logLevel);
        JUST_SetLogHook(Just_OnLogDump, logLevel);
    }

    JUST_RedirectCallback(just_redirect_callback, just_free_callback, clsMediaSdk.get_class());

    LOG(3, "[JNI_OnLoad] finish");

    return JNI_VERSION_1_6;
}

extern "C" JNIEXPORT void JNICALL JNI_OnUnload(
    JavaVM *vm, 
    void *reserved)
{
    LOG(3, "[JNI_OnUnload] begin");

    JUST_Unload();

    if (g_logOn) {
        // Just_LogDump(NULL, 0);
    }

    LOG(3, "[JNI_OnUnload] finish");
}
