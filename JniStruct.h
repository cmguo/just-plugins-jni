// JniStruct.h

#ifndef _PLUGINS_JNI_JNI_STRUCT_H_
#define _PLUGINS_JNI_JNI_STRUCT_H_

#include "plugins/jni/JniValue.h"

template<typename T>
struct JStruct
    : JObject
{
};

template<typename T>
struct Value<JStruct<T *>, T *>
{
    typedef jobject jtype_t;
    typedef T * ctype_t;

    Value(
        JNIEnv * env, 
        jobject obj, 
        j_tag_t * = NULL)
        : env_(env)
        , obj_(obj)
    {
        memset(&struct_, 0, sizeof(T));
    }

    Value(
        JNIEnv * env, 
        T * c, 
        c_tag_t *)
        : env_(env)
        , obj_(NULL)
    {
        memset(&struct_, 0, sizeof(T));
    }

    ~Value()
    {
        to_jni();
    }

    jtype_t jvalue()
    {
        return obj_;
    }

    ctype_t cvalue()
    {
        return &struct_;
    }

private:
    void to_jni();

private:
    JNIEnv * env_;
    jobject obj_;
    T struct_;
};

template<typename T>
struct Value<JStruct<T const *>, T const *>
{
    typedef jobject jtype_t;
    typedef T const * ctype_t;

    Value(
        JNIEnv * env, 
        jobject obj)
        : env_(env)
        , obj_(obj)
    {
        memset(&struct_, 0, sizeof(T));
        from_jni();
    }

    Value(
        JNIEnv * env, 
        T * c, 
        c_tag_t *)
        : env_(env)
        , obj_(NULL)
    {
        memset(&struct_, 0, sizeof(T));
    }

    ctype_t cvalue() const
    {
        return &struct_;
    }

private:
    void from_jni();

private:
    JNIEnv * env_;
    jobject obj_;
    T struct_;
};


#define BIND_STRUCT_TYPE(Struct) BIND_TYPE(JStruct<Struct>, Struct)

#endif // _PLUGINS_JNI_JNI_STRUCT_H_
