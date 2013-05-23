// JniValue.h

#ifndef _PLUGINS_JNI_JNI_VALUE_H_
#define _PLUGINS_JNI_JNI_VALUE_H_

#include "plugins/jni/JniTypes.h"
#include "plugins/jni/TypeBind.h"

struct j_tag_t {} * j_tag = NULL;
struct c_tag_t {} * c_tag = NULL;

template <
    typename jT, 
    typename cT
>
struct Value
{
    typedef typename jT::jtype_t jtype_t;
    typedef cT ctype_t;

    Value(
        JNIEnv * env, 
        jtype_t j, 
        j_tag_t * = NULL)
        : j_(j)
    {
    }

    Value(
        JNIEnv * env, 
        ctype_t c, 
        c_tag_t *)
        : j_(jtype_t(c))
    {
    }

    jtype_t jvalue() const
    {
        return j_;
    }

    ctype_t cvalue() const
    {
        return (ctype_t)j_;
    }

protected:
    jtype_t j_;
};

struct string_holder
{
public:
    string_holder(
        char const * str = NULL)
        : str_(str)
    {
        if (str_)
            ++*const_cast<char *>(str_);
    }

    ~string_holder()
    {
        if (str_ && --*const_cast<char *>(str_) == 0) {
            delete [] str_;
        }
    }

    string_holder & operator=(
        string_holder const & r)
    {
        string_holder(r).swap(*this);
        return *this;
    }

    operator char const *() const
    {
        return str_ + 1;
    }

    char const * c_str() const
    {
        return str_ + 1;
    }

private:
    void swap(
        string_holder & r)
    {
        char const * tmp = str_;
        str_ = r.str_;
        r.str_ = tmp;
    }

protected:
    char const * str_;
};

template <>
struct Value<JString, char const *>
{
    typedef jstring jtype_t;
    typedef string_holder ctype_t;

    Value(
        JNIEnv * env, 
        jtype_t j, 
        j_tag_t * = NULL)
        : j_(j)
    {
        jclass cls = env->FindClass("java/lang/String");
        jstring enc = env->NewStringUTF("utf-8");
        jmethodID mtd = env->GetMethodID(cls, "getBytes", "(Ljava/lang/String;)[B");
        jbyteArray arr = (jbyteArray) env->CallObjectMethod(j_, mtd, enc);
        jsize len = env->GetArrayLength(arr);
        jbyte * bytes = env->GetByteArrayElements(arr, JNI_FALSE);
        if (len > 0) {
            char * p = (char *)malloc(len + 2);
            memcpy(p + 1, bytes, len);
            p[len + 1] = 0;
            c_ = p;
        }
        env->ReleaseByteArrayElements(arr, bytes, 0);
        env->DeleteLocalRef(cls);
        env->DeleteLocalRef(enc);
        env->DeleteLocalRef(arr);
    }

    Value(
        JNIEnv * env, 
        char const * c, 
        c_tag_t *)
        : j_(NULL)
    {
        jclass cls = env->FindClass("java/lang/String");
        jmethodID ctor = env->GetMethodID(cls, "<init>","([BLjava/lang/String;)V"); 
        jbyteArray arr = env->NewByteArray(strlen(c));
        env->SetByteArrayRegion(arr, 0, strlen(c),(jbyte *)c);
        jstring enc = env->NewStringUTF("utf-8");
        j_ = (jstring)env->NewObject(cls, ctor, arr, enc);
        env->DeleteLocalRef(enc);
        env->DeleteLocalRef(arr);
        env->DeleteLocalRef(cls);
    }

    ~Value()
    {
    }

    jtype_t jvalue() const
    {
        return j_;
    }

    ctype_t cvalue() const
    {
        return c_;
    }

private:
    jtype_t j_;
    ctype_t c_;
};

template <
    typename T
>
struct CValue
{
    typedef Value<typename typec2j<T>::jtype, T> value_t;
};

template <
    typename T
>
struct JValue
{
    typedef Value<T, typename typej2c<T>::ctype> value_t;
};

#endif // _PLUGINS_JNI_JNI_VALUE_H_
