// JniCallback.h

#ifndef _PLUGINS_JNI_JNI_CALLBACK_H_
#define _PLUGINS_JNI_JNI_CALLBACK_H_

#include "plugins/jni/JniValue.h"

struct __JniCallback
    : JObject
{
    __JniCallback(
        JNIEnv * env, 
        jobject obj, 
        char const * sig)
        : env_(env)
        , obj_(env->NewGlobalRef(obj))
    {
        env_->GetJavaVM(&vm_);
        jclass cls = env->GetObjectClass(obj);
        method_ = env->GetMethodID(cls, "invoke", sig);
    }

    virtual ~__JniCallback()
    {
        env_->DeleteGlobalRef(obj_);
    }

    virtual void invoke(
        void * result, 
        va_list args) const = 0;

    template <typename cT>
    void invoke2(
        cT * result, 
        ...) const
    {
        va_list args;
        va_start(args, result);
        typedef typename typec2j<cT>::jtype jT;
        JNIEnv * env = NULL;
        vm_->AttachCurrentThread(&env, NULL);
        typename jT::jtype_t r = 
            jT::call(env, obj_, method_, args);
        *result = typename JValue<jT>::value_t(env, r).cvalue();
        va_end(args);
    }

    void invoke2(
        void * result, 
        ...) const
    {
        va_list args;
        va_start(args, result);
        JNIEnv * env = NULL;
        vm_->AttachCurrentThread(&env, NULL);
        JVoid::call(env, obj_, method_, args);
        va_end(args);
    }

protected:
    JavaVM * vm_;
    JNIEnv * env_;
    jobject obj_;
    jmethodID method_;
};

template <
    typename F
>
struct JniCallback
    : __JniCallback
{
public:
    JniCallback(
         JNIEnv * env, 
         jobject method)
         : __JniCallback(env, method, invoke_sig())
    {
    }

    static const char * name_str();
    static const char * invoke_sig();

    static const char * sig();

    virtual void invoke(
        void * result, 
        va_list args) const;
};

template <
    typename F
>
struct Value<JniCallback<F>, F>
{
    typedef jobject jtype_t;
    typedef F ctype_t;

    Value(
        JNIEnv * env, 
        jobject j, 
        j_tag_t * = NULL)
        : callback_(NULL)
    {
        if (j)
            callback_ = new JniCallback<F>(env, j);
    }

    Value(
        JNIEnv * env, 
        ctype_t c, 
        c_tag_t *)
        : callback_(NULL)
    {
    }

    jtype_t jvalue() const
    {
        return (jtype_t)callback_;
    }

    ctype_t cvalue() const
    {
        return (ctype_t)callback_;
    }

protected:
    JniCallback<F> * callback_;
};

#define BIND_CALLBACK_TYPE(name) \
    BIND_TYPE(JniCallback<name>, name)

#endif // _PLUGINS_JNI_JNI_CALLBACK_H_
