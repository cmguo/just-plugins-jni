// Common.h

#include <framework/Framework.h>

#ifdef __ANDROID__
#  include <android/log.h>
#endif
#include <jni.h>

#ifdef __linux__
#  undef JNIEXPORT
#  define JNIEXPORT __attribute__ ((visibility("default")))
#endif
