// JString.h

#ifndef _PLUGINS_JNI_J_STRING_H_
#define _PLUGINS_JNI_J_STRING_H_

#include "plugins/jni/JniValue.h"

struct JString
{
    typedef jstring jtype_t;

    static const char * sig()
    {
        return "Ljava/lang/String;";
    }

    static jtype_t get_static(
        JNIEnv * env, 
        jclass _class, 
        jfieldID field)
    {
        return static_cast<jtype_t>(env->GetStaticObjectField(_class, field));
    }

    static jtype_t get(
        JNIEnv * env, 
        jobject obj, 
        jfieldID field)
    {
        return static_cast<jtype_t>(env->GetObjectField(obj, field));
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

BIND_TYPE(JString, char const *)

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

    string_holder(
        string_holder const & r)
        : str_(r.str_)
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
        return c_str();
    }

    char const * c_str() const
    {
        return str_ ? (str_ + 1) : NULL;
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
        if (j_ == NULL)
            return;
        jclass cls = env->FindClass("java/lang/String");
        jstring enc = env->NewStringUTF("utf-8");
        jmethodID mtd = env->GetMethodID(cls, "getBytes", "(Ljava/lang/String;)[B");
        jbyteArray arr = (jbyteArray) env->CallObjectMethod(j_, mtd, enc);
        jsize len = env->GetArrayLength(arr);
        jbyte * bytes = env->GetByteArrayElements(arr, NULL);
        char * p = (char *)malloc(len + 2);
        memcpy(p + 1, bytes, len);
        p[len + 1] = 0;
        c_ = p;
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

#endif // _PLUGINS_JNI_J_STRING_H_
