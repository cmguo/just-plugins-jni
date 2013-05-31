// JniTypes.h

#ifndef _PLUGINS_JNI_J_BYTE_ARRAY_H_
#define _PLUGINS_JNI_J_BYTE_ARRAY_H_

struct JByteArray
{
    typedef jbyteArray jtype_t;

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

BIND_TYPE(JByteArray, unsigned char const *)

#endif // _PLUGINS_JNI_J_BYTE_ARRAY_H_
