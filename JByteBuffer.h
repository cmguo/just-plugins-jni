// JByteBuffer.h

#ifndef _PLUGINS_JNI_J_BYTE_BUFFER_H_
#define _PLUGINS_JNI_J_BYTE_BUFFER_H_

struct JByteBuffer
    : public JObject
{
    static const char * sig()
    {
        return "Ljava/nio/ByteBuffer;";
    }
};

BIND_TYPE(JByteBuffer, unsigned char const *)

template <>
struct Value<JByteBuffer, unsigned char const *>
{
    typedef jobject jtype_t;
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
        c_ = (ctype_t)env->GetDirectBufferAddress(j_);
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

#endif // _PLUGINS_JNI_J_BYTE_BUFFER_H_
