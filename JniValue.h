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
