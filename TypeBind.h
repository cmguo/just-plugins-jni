// TypeBind.h

#ifndef _PLUGINS_JNI_TYPE_BIND_H_
#define _PLUGINS_JNI_TYPE_BIND_H_

#include "plugins/jni/JniTypes.h"

#include <boost/cstdint.hpp>

template <
    typename T
>
struct typej2c
{
    typedef T ctype; // same
};

template <
    typename T
>
struct typec2j
{
    typedef T jtype; // same
};

#define BIND_TYPE(j, c) \
    template <> struct typej2c<j> { typedef c ctype; }; \
    template <> struct typec2j<c> { typedef j jtype; };

BIND_TYPE(JVoid, void)
BIND_TYPE(JBoolean, bool)
BIND_TYPE(JByte, boost::int8_t)
BIND_TYPE(JShort, boost::int16_t)
BIND_TYPE(JInt, boost::int32_t)
BIND_TYPE(JLong, boost::int64_t)
BIND_TYPE(JUByte, boost::uint8_t)
BIND_TYPE(JUShort, boost::uint16_t)
BIND_TYPE(JUInt, boost::uint32_t)
BIND_TYPE(JULong, boost::uint64_t)
BIND_TYPE(JFloat, float)
BIND_TYPE(JDouble, double)

BIND_TYPE(JPointer<void *>, void *)
BIND_TYPE(JPointer<void const *>, void const *)

#endif // _PLUGINS_JNI_TYPE_BIND_H_
