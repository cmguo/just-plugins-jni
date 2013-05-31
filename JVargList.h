// JVargList.h

#ifndef _PLUGINS_JNI_J_VARG_LIST_H_
#define _PLUGINS_JNI_J_VARG_LIST_H_

#include "plugins/jni/JniValue.h"

struct JVargList
{
    typedef void const * jtype_t;
    static char const * sig() { return ""; }
};

BIND_TYPE(JVargList, va_list);

template<>
struct Value<JVargList, va_list>
{
    typedef void const * jtype_t;
    typedef va_list ctype_t;

    Value(
        JNIEnv * env, 
        ctype_t c, 
        c_tag_t *)
        : j_(NULL)
    {
    }

    jtype_t jvalue() const
    {
        return j_;
    }

protected:
    jtype_t j_;
};

#endif // _PLUGINS_JNI_J_VARG_LIST_H_

