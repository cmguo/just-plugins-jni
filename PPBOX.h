// PpboxJni.h

#include "plugins/jni/JniTypes.h"
#include "plugins/jni/JniValue.h"
#include "plugins/jni/JniStruct.h"
#include "plugins/jni/JniCallback.h"
#include "plugins/jni/JString.h"
#include "plugins/jni/JByteBuffer.h"
#include "plugins/jni/JVargList.h"

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/stringize.hpp>

#define PARAM_JNI_TYPE_NAME(i, t, n) , typec2j<t>::jtype::jtype_t BOOST_PP_CAT(n, _j)
#define PARAM_JNI_TYPE(i, t, n) , typec2j<t>::jtype
#define PARAM_LOCAL_VARIABLE(i, t, n) CValue<t>::value_t BOOST_PP_CAT(n, _l)(env, BOOST_PP_CAT(n, _j));
#define PARAM_LOCAL_VALUE(i, t, n) BOOST_PP_COMMA_IF(i) BOOST_PP_CAT(n, _l).cvalue()

#define PARAMS_JNI_TYPE_NAME(np, params) LIST_PAIR_FORMAT(PARAM_JNI_TYPE_NAME, np, params)
#define PARAMS_JNI_TYPE(np, params) LIST_PAIR_FORMAT(PARAM_JNI_TYPE, np, params)
#define PARAMS_LOCAL_VARIABLE(np, params) LIST_PAIR_FORMAT(PARAM_LOCAL_VARIABLE, np, params)
#define PARAMS_LOCAL_VALUE(np, params) LIST_PAIR_FORMAT(PARAM_LOCAL_VALUE, np, params)

#define FIELD_TO_JNI(i, t, n) \
    CValue<t>::value_t BOOST_PP_CAT(n, _l)(env_, struct_.n, c_tag); \
    _class.field_jvalue<typec2j<t>::jtype>(BOOST_PP_STRINGIZE(n), obj_, BOOST_PP_CAT(n, _l).jvalue());

#define FIELDS_TO_JNI(np, params) LIST_PAIR_FORMAT(FIELD_TO_JNI, np, params)

#define FIELD_FROM_JNI(i, t, n) \
    CValue<t>::value_t BOOST_PP_CAT(n, _l) = \
        _class.field_value<typec2j<t>::jtype>(BOOST_PP_STRINGIZE(n), obj_); \
    struct_.n = BOOST_PP_CAT(n, _l).cvalue();

#define FIELDS_FROM_JNI(np, params) LIST_PAIR_FORMAT(FIELD_FROM_JNI, np, params)

#define PPBOX_FUNC(type, name, np, params) \
    typedef type (* FT_ ## name)(FUNCTION_PARAMS_TYPE(np, params)); \
    inline type name(FUNCTION_PARAMS_TYPE_NAME(np, params)) \
    { \
        static FT_ ## name fp = NULL; \
        if (fp == NULL) { \
            LOG(3, "[" #name "]"); \
            fp = (FT_ ## name)dlsym(PPBOX_Load(), #name); \
            if (fp == NULL) { \
                PPBOX_FUNCTION_NOT_EXIST(name); \
                return type##_defalut(); \
            } \
        } \
        return fp(FUNCTION_PARAMS_NAME(np, params)); \
    } \
    extern "C" JNIEXPORT typec2j<type>::jtype::jtype_t JNICALL BOOST_PP_CAT(PPBOX_JNI_PREFIX_, name)( \
        JNIEnv *env, jclass thiz PARAMS_JNI_TYPE_NAME(np, params)) \
    { \
        PARAMS_LOCAL_VARIABLE(np, params); \
        return CValue<type>::value_t(env, name(PARAMS_LOCAL_VALUE(np, params)), c_tag).jvalue(); \
    } \

#define PPBOX_STRUCT(name, nf, fields) \
    struct name { STRUCT_FIELDS_TYPE_NAME(nf, fields) }; \
    template <> \
    void Value<JStruct<name *>, name *>::to_jni() \
    { \
        JniClass _class(env_, obj_); \
        FIELDS_TO_JNI(nf, fields); \
    } \
    template <> \
    void Value<JStruct<name const *>, name const *>::from_jni() \
    { \
        JniClass _class(env_, obj_); \
        FIELDS_FROM_JNI(nf, fields); \
    } \
    BIND_STRUCT_TYPE(name *) \
    BIND_STRUCT_TYPE(name const *)

#define VA_ARG_SIG(i, t) \
    strncat(str, typec2j<t>::jtype::sig(), sizeof(str));
#define VA_ARGS_SIG(np, params) LIST_FORMAT(VA_ARG_SIG, np, params)

#define VA_ARG_TO_JNI(i, t) \
    , CValue<t>::value_t(env_, va_arg(args, t), c_tag).jvalue()
#define VA_ARGS_TO_JNI(np, params) LIST_FORMAT(VA_ARG_TO_JNI, np, params)

#define PPBOX_CALLBACK(type, name, np, params) \
    typedef type (*name)params; \
    BIND_CALLBACK_TYPE(name) \
    template <> \
    char const * JniCallback<name>::name_str() { return #name; } \
    template <> \
    char const * JniCallback<name>::invoke_sig() \
    { \
        static char str[1024] = {0}; \
        if (str[0] == 0) { \
            strncat(str, "(", sizeof(str)); \
            VA_ARGS_SIG(np, params); \
            strncat(str, ")", sizeof(str)); \
            strncat(str, typec2j<type>::jtype::sig(), sizeof(str)); \
        } \
        return str; \
    } \
    template <> \
    void JniCallback<name>::invoke(void * result, va_list args) const \
    { \
        invoke2((type *)result VA_ARGS_TO_JNI(np, params)); \
    }
