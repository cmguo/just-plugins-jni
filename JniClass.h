// JniClass.h

#ifndef _PLUGINS_JNI_JNI_CLASS_H_
#define _PLUGINS_JNI_JNI_CLASS_H_

#include "plugins/jni/JniValue.h"

template <
    typename jT, 
    typename cT
>
class JniStaticField
{
public:
    typedef typename jT::jtype_t jtype_t;
    typedef cT ctype_t;
    typedef Value<jT, ctype_t> value_t;

    JniStaticField(
         JNIEnv * env, 
         jclass _class, 
         char const * name)
         : env_(env)
         , class_(_class)
    {
        field_ = env->GetStaticFieldID(_class, name, jT::sig());
    }

    value_t value() const
    {
        return value_t(env_, jT::get_static(env_, class_, field_));
    }

    typename value_t::ctype_t cvalue() const
    {
        return value().cvalue();
    }

private:
    JNIEnv * env_;
    jclass class_;
    jfieldID field_;
};

template <
    typename jT, 
    typename cT
>
class JniField
{
public:
    typedef typename jT::jtype_t jtype_t;
    typedef cT ctype_t;
    typedef Value<jT, ctype_t> value_t;

    JniField(
        JNIEnv * env, 
        jclass _class, 
        char const * name)
        : env_(env)
    {
        field_ = env->GetFieldID(_class, name, jT::sig());
    }

    value_t value(
        jobject obj) const
    {
        return value_t(env_, jT::get(env_, obj, field_));
    }

    void jvalue(
        jobject obj, 
        jtype_t value) const
    {
        jT::set(env_, obj, field_, value);
    }

    typename value_t::ctype_t cvalue(
        jobject obj) const
    {
        return value(obj).cvalue();
    }

private:
    JNIEnv * env_;
    jfieldID field_;
};

class JniClass
{
public:
    JniClass(
        JNIEnv * env, 
        char const * name)
        : env_(env)
    {
        class_ = env->FindClass(name);
    }

    JniClass(
        JNIEnv * env, 
        jobject obj)
        : env_(env)
    {
        class_ = env->GetObjectClass(obj);
    }

    jclass get_class()
    {
        return (jclass)env_->NewGlobalRef(class_);
    }

    template <
        typename T
    >
    JniStaticField<T, typename typej2c<T>::ctype> static_field(
        char const * name)
    {
        return JniStaticField<T, typename jtype<T>::ctype>(env_, class_, name);
    }

    template <
        typename T
    >
    Value<T, typename typej2c<T>::ctype> static_field_value(
        char const * name)
    {
        return JniStaticField<T, typename typej2c<T>::ctype>(env_, class_, name).value();
    }

    template <
        typename T
    >
    typename Value<T, typename typej2c<T>::ctype>::jtype_t static_field_jvalue(
        char const * name)
    {
        return JniStaticField<T, typename jtype<T>::ctype>(env_, class_, name).value().jvalue();
    }

    template <
        typename T
    >
    typename Value<T, typename typej2c<T>::ctype>::ctype_t static_field_cvalue(
        char const * name)
    {
        return JniStaticField<T, typename typej2c<T>::ctype>(env_, class_, name).cvalue();
    }

    template <
        typename T
    >
    JniField<T, typename typej2c<T>::ctype> field(
        char const * name)
    {
        return JniField<T, typename typej2c<T>::ctype>(env_, class_, name);
    }

    template <
        typename T
    >
    Value<T, typename typej2c<T>::ctype> field_value(
        char const * name, 
        jobject obj)
    {
        return JniField<T, typename typej2c<T>::ctype>(env_, class_, name).value(obj);
    }

    template <
        typename T
    >
    typename Value<T, typename typej2c<T>::ctype>::jtype_t field_jvalue(
        char const * name, 
        jobject obj)
    {
        return JniField<T, typename typej2c<T>::ctype>(env_, class_, name).value(obj).jvalue();
    }

    template <
        typename T
    >
    void field_jvalue(
        char const * name, 
        jobject obj, 
        typename Value<T, typename typej2c<T>::ctype>::jtype_t value)
    {
        JniField<T, typename typej2c<T>::ctype>(env_, class_, name).jvalue(obj, value);
    }

    template <
        typename T
    >
    typename Value<T, typename typej2c<T>::ctype>::ctype_t field_cvalue(
        char const * name, 
        jobject obj)
    {
        return JniField<T, typename typej2c<T>::ctype>(env_, class_, name).cvalue(obj);
    }

private:
    JNIEnv * env_;
    jclass class_;
};

#endif // _PLUGINS_JNI_JNI_CLASS_H_
