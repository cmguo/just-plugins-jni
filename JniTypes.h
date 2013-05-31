// JniTypes.h

#ifndef _PLUGINS_JNI_JNI_TYPES_H_
#define _PLUGINS_JNI_JNI_TYPES_H_

#include <jni.h>

struct JVoid
{
    typedef void jtype_t;

    static const char * sig()
    {
        return "V";
    }

    static void call_static(
        JNIEnv * env, 
        jclass _class, 
        jmethodID method, 
        va_list args)
    {
        env->CallStaticVoidMethodV(_class, method, args);
    }

    static void call(
        JNIEnv * env, 
        jobject obj, 
        jmethodID method, 
        va_list args)
    {
        env->CallVoidMethodV(obj, method, args);
    }
};

struct JBoolean
{
    typedef jboolean jtype_t;

    static const char * sig()
    {
        return "Z";
    }

    static jtype_t call_static(
        JNIEnv * env, 
        jclass _class, 
        jmethodID method, 
        va_list args)
    {
        return env->CallStaticBooleanMethodV(_class, method, args);
    }

    static jtype_t call(
        JNIEnv * env, 
        jobject obj, 
        jmethodID method, 
        va_list args)
    {
        return env->CallBooleanMethodV(obj, method, args);
    }

    static jtype_t get_static(
        JNIEnv * env, 
        jclass _class, 
        jfieldID field)
    {
        return env->GetStaticBooleanField(_class, field);
    }

    static jtype_t get(
        JNIEnv * env, 
        jobject obj, 
        jfieldID field)
    {
        return env->GetBooleanField(obj, field);
    }

    static void set(
        JNIEnv * env, 
        jobject obj, 
        jfieldID field, 
        jtype_t value)
    {
        return env->SetBooleanField(obj, field, value);
    }
};

struct JByte
{
    typedef jbyte jtype_t;

    static const char * sig()
    {
        return "B";
    }

    static jtype_t get_static(
        JNIEnv * env, 
        jclass _class, 
        jfieldID field)
    {
        return env->GetStaticByteField(_class, field);
    }

    static jtype_t get(
        JNIEnv * env, 
        jobject obj, 
        jfieldID field)
    {
        return env->GetByteField(obj, field);
    }

    static void set(
        JNIEnv * env, 
        jobject obj, 
        jfieldID field, 
        jtype_t value)
    {
        return env->SetByteField(obj, field, value);
    }
};

struct JChar
{
    typedef jchar jtype_t;

    static const char * sig()
    {
        return "C";
    }

    static jtype_t get_static(
        JNIEnv * env, 
        jclass _class, 
        jfieldID field)
    {
        return env->GetStaticCharField(_class, field);
    }

    static jtype_t get(
        JNIEnv * env, 
        jobject obj, 
        jfieldID field)
    {
        return env->GetCharField(obj, field);
    }

    static void set(
        JNIEnv * env, 
        jobject obj, 
        jfieldID field, 
        jtype_t value)
    {
        return env->SetCharField(obj, field, value);
    }
};

struct JDouble
{
    typedef jdouble jtype_t;

    static const char * sig()
    {
        return "D";
    }

    static jtype_t get_static(
        JNIEnv * env, 
        jclass _class, 
        jfieldID field)
    {
        return env->GetStaticDoubleField(_class, field);
    }

    static jtype_t get(
        JNIEnv * env, 
        jobject obj, 
        jfieldID field)
    {
        return env->GetDoubleField(obj, field);
    }

    static void set(
        JNIEnv * env, 
        jobject obj, 
        jfieldID field, 
        jtype_t value)
    {
        return env->SetDoubleField(obj, field, value);
    }
};

struct JFloat
{
    typedef jfloat jtype_t;

    static const char * sig()
    {
        return "F";
    }

    static jtype_t get_static(
        JNIEnv * env, 
        jclass _class, 
        jfieldID field)
    {
        return env->GetStaticFloatField(_class, field);
    }

    static jtype_t get(
        JNIEnv * env, 
        jobject obj, 
        jfieldID field)
    {
        return env->GetFloatField(obj, field);
    }

    static void set(
        JNIEnv * env, 
        jobject obj, 
        jfieldID field, 
        jtype_t value)
    {
        return env->SetFloatField(obj, field, value);
    }
};

struct JShort
{
    typedef jshort jtype_t;

    static const char * sig()
    {
        return "I";
    }

    static jtype_t get_static(
        JNIEnv * env, 
        jclass _class, 
        jfieldID field)
    {
        return env->GetStaticShortField(_class, field);
    }

    static jtype_t get(
        JNIEnv * env, 
        jobject obj, 
        jfieldID field)
    {
        return env->GetShortField(obj, field);
    }

    static void set(
        JNIEnv * env, 
        jobject obj, 
        jfieldID field, 
        jtype_t value)
    {
        return env->SetShortField(obj, field, value);
    }
};

struct JInt
{
    typedef jint jtype_t;

    static const char * sig()
    {
        return "I";
    }

    static jtype_t get_static(
        JNIEnv * env, 
        jclass _class, 
        jfieldID field)
    {
        return env->GetStaticIntField(_class, field);
    }

    static jtype_t get(
        JNIEnv * env, 
        jobject obj, 
        jfieldID field)
    {
        return env->GetIntField(obj, field);
    }

    static void set(
        JNIEnv * env, 
        jobject obj, 
        jfieldID field, 
        jtype_t value)
    {
        return env->SetIntField(obj, field, value);
    }
};

struct JLong
{
    typedef jlong jtype_t;

    static const char * sig()
    {
        return "J";
    }

    static jtype_t get_static(
        JNIEnv * env, 
        jclass _class, 
        jfieldID field)
    {
        return env->GetStaticLongField(_class, field);
    }

    static jtype_t get(
        JNIEnv * env, 
        jobject obj, 
        jfieldID field)
    {
        return env->GetLongField(obj, field);
    }

    static void set(
        JNIEnv * env, 
        jobject obj, 
        jfieldID field, 
        jtype_t value)
    {
        return env->SetLongField(obj, field, value);
    }
};

struct JObject
{
    typedef jobject jtype_t;

    static const char * sig()
    {
        return "L";
    }

    static jtype_t get_static(
        JNIEnv * env, 
        jclass _class, 
        jfieldID field)
    {
        return env->GetStaticObjectField(_class, field);
    }

    static jtype_t get(
        JNIEnv * env, 
        jobject obj, 
        jfieldID field)
    {
        return env->GetObjectField(obj, field);
    }

    static void set(
        JNIEnv * env, 
        jobject obj, 
        jfieldID field, 
        jtype_t value)
    {
        return env->SetObjectField(obj, field, value);
    }
};

template <typename T>
struct JUnsigned
    : T
{
};

template <typename T>
struct JPointer
    : JLong
{
};

typedef JUnsigned<JByte> JUByte;
typedef JUnsigned<JShort> JUShort;
typedef JUnsigned<JInt> JUInt;
typedef JUnsigned<JLong> JULong;


#endif // _PLUGINS_JNI_JNI_TYPES_H_
