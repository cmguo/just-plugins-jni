// JniCallback.h

#ifndef _PLUGINS_JNI_JNI_CALLBACK_H_
#define _PLUGINS_JNI_JNI_CALLBACK_H_

#include "plugins/jni/JniValue.h"

template <
    typename F
>
struct FuncResult;

template <
    typename R
>
struct FuncResult<R ()>
{
    typedef R result_t;
};

template <
    typename R, 
    typename A1
>
struct FuncResult<R (*)(A1)>
    : FuncResult<R()>
{
};

template <
    typename R, 
    typename A1, 
    typename A2
>
struct FuncResult<R (*)(A1, A2)>
    : FuncResult<R()>
{
};

template <
    typename R, 
    typename A1, 
    typename A2, 
    typename A3
>
struct FuncResult<R (*)(A1, A2, A3)>
    : FuncResult<R()>
{
};

template <
    typename R, 
    typename A1, 
    typename A2, 
    typename A3, 
    typename A4
>
struct FuncResult<R (*)(A1, A2, A3, A4)>
    : FuncResult<R()>
{
};

struct __JniCallback
    : JObject
{
    __JniCallback(
        JNIEnv * env, 
        jobject method)
        : env_(env)
    {
        method_ = env->FromReflectedMethod(method);
    }

    virtual ~__JniCallback()
    {
    }

    virtual void invoke(
        jclass _class, 
        void * result, 
        va_list args) const = 0;

protected:
    JNIEnv * env_;
    jmethodID method_;
};

template <
    typename R
>
struct _JniCallback
    : __JniCallback
{
    typedef R cresult_t;
    typedef typename typec2j<cresult_t>::jtype jtype;
    typedef typename jtype::jtype_t jresult_t;

    _JniCallback(
        JNIEnv * env, 
        jobject method)
        : __JniCallback(env, method)
    {
    }

    virtual void invoke(
        jclass _class, 
        void * result, 
        va_list args) const
    {
        *(jresult_t *)result = jtype::call_static(env_, _class, method_, args);
    }
};

template <>
struct _JniCallback<void>
    : __JniCallback
{
    typedef void cresult_t;
    typedef JVoid jtype;
    typedef void jresult_t;

    _JniCallback(
        JNIEnv * env, 
        jobject method)
        : __JniCallback(env, method)
    {
    }

    virtual void invoke(
        jclass _class, 
        void * result, 
        va_list args) const
    {
        jtype::call_static(env_, _class, method_, args);
    }
};

template <
    typename F
>
struct JniCallback
    : _JniCallback<typename FuncResult<F>::result_t>
{
public:
    typedef _JniCallback<typename FuncResult<F>::result_t> super_t;

    JniCallback(
         JNIEnv * env, 
         jobject method)
         : super_t(env, method)
    {
    }
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
        : callback_(new JniCallback<F>(env, j))
    {
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
