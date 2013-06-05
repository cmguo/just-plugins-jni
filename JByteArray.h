// JniTypes.h

#ifndef _PLUGINS_JNI_J_BYTE_ARRAY_H_
#define _PLUGINS_JNI_J_BYTE_ARRAY_H_

struct JByteArray
{
    typedef jbyteArray jtype_t;

    static const char * sig()
    {
        return "[B";
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

BIND_TYPE(JByteArray, unsigned char const *)

template <>
struct Value<JByteArray, unsigned char const *>
{
    typedef jbyteArray jtype_t;
    typedef unsigned char const * ctype_t;

    Value(
        JNIEnv * env, 
        jtype_t j, 
        j_tag_t * = NULL)
        : env_(env)
        , j_(j)
        , c_(NULL)
    {
        if (j_ == NULL)
            return;
        c_ = (ctype_t)env->GetByteArrayElements(j_, NULL);
    }

    Value(
        JNIEnv * env, 
        ctype_t c, 
        c_tag_t *)
        : env_(env)
        , j_(NULL)
        , c_(NULL)
    {
    }

    ~Value()
    {
        if (c_)
            env_->ReleaseByteArrayElements(j_, (jbyte *)c_, JNI_ABORT);
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
    JNIEnv * env_;
    jtype_t j_;
    ctype_t c_;
};

#endif // _PLUGINS_JNI_J_BYTE_ARRAY_H_
