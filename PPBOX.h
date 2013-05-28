// PpboxJni.h

#include "plugins/jni/JniTypes.h"
#include "plugins/jni/JniValue.h"
#include "plugins/jni/JniStruct.h"
#include "plugins/jni/JniCallback.h"

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
    CValue<t>::value_t BOOST_PP_CAT(n, _l)(env, s.n, c_tag); \
    _class.field_jvalue<typec2j<t>::jtype>(BOOST_PP_STRINGIZE(n), obj, BOOST_PP_CAT(n, _l).jvalue());

#define FIELDS_TO_JNI(np, params) LIST_PAIR_FORMAT(FIELD_TO_JNI, np, params)

#define FIELD_FROM_JNI(i, t, n) \
    CValue<t>::value_t BOOST_PP_CAT(n, _l) = \
        _class.field_value<typec2j<t>::jtype>(BOOST_PP_STRINGIZE(n), obj); \
    s.n = BOOST_PP_CAT(n, _l).cvalue();

#define FIELDS_FROM_JNI(np, params) LIST_PAIR_FORMAT(FIELD_FROM_JNI, np, params)

#define PPBOX_FUNC(type, name, np, params) \
    typedef type (* FT_ ## name)(PARAMS_TYPE(np, params)); \
    inline type name(PARAMS_TYPE_NAME(np, params)) \
    { \
        static FT_ ## name fp = NULL; \
        if (fp == NULL) { \
            fp = (FT_ ## name)dlsym(PPBOX_Load(), #name); \
            if (fp == NULL) { \
                PPBOX_FUNC_NOT_EXIST(name); \
                return type##_defalut(); \
            } \
        } \
        return fp(PARAMS_NAME(np, params)); \
    } \
    extern "C" JNIEXPORT typec2j<type>::jtype::jtype_t JNICALL BOOST_PP_CAT(PPBOX_JNI_PREFIX_, name)( \
        JNIEnv *env, jclass thiz PARAMS_JNI_TYPE_NAME(np, params)) \
    { \
        LOG(3, "[" #name "]"); \
        PARAMS_LOCAL_VARIABLE(np, params); \
        return CValue<type>::value_t(env, name(PARAMS_LOCAL_VALUE(np, params)), c_tag).jvalue(); \
    } \

#define PPBOX_STRUCT(name, nf, fields) \
    struct name { FIELDS_TYPE_NAME(nf, fields) }; \
    void to_jni(JNIEnv * env, jobject obj, name const & s) \
    { \
        static JniClass _class(env, obj); \
        FIELDS_TO_JNI(nf, fields); \
    } \
    void from_jni(JNIEnv * env, jobject obj, name & s) \
    { \
        static JniClass _class(env, obj); \
        FIELDS_FROM_JNI(nf, fields); \
    } \
    BIND_STRUCT_TYPE(name *) \
    BIND_STRUCT_TYPE(name const *)


#define PPBOX_CALLBACK(type, name, np, params) \
    typedef type (*name)params; \
    BIND_CALLBACK_TYPE(name)
