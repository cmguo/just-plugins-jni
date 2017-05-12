package com.just.sdk;

import java.nio.ByteBuffer;
import java.util.Properties;

public class JUST
{

    public static final int LEVEL_FATAL = 1;
    public static final int LEVEL_ERROR = 2;
    public static final int LEVEL_WARN = 3;
    public static final int LEVEL_INFO = 4;
    public static final int LEVEL_DEBUG = 5;
    public static final int LEVEL_TRACE = 6;
    
    public static String libPath = ".";
    public static String cfgPath = ".";
    public static String logPath = ".";
    public static String libName = null;
    public static boolean logOn = true;
    public static int logLevel = LEVEL_DEBUG;

    public static long load()
    {
        try {
            Properties props = System.getProperties();
            String osArch = props.getProperty("os.arch");
            System.out.println(osArch);
            if (osArch == null) {
                System.out.println("Can't get arch info.");
            } else if (osArch.contains("x86")) {
                System.loadLibrary("libjust_jni-win32-msvc90-mt-gd-1");
            } else if (osArch.contains("mips")) {
                System.loadLibrary("just_jni-mips-android-gcc44-mt-1");
            } else if (osArch.contains("i386")) {
                System.loadLibrary("just_jni-linux-x86-gcc44-mt-1");
            } else if (osArch.contains("amd64")) {
                System.loadLibrary("just_jni-linux-x64-gcc44-mt-gd-1");
            } else if (osArch.contains("armv7l")) {
                System.loadLibrary("just_jni-arm-android-r9-gcc46-mt-1");
            } else if (osArch.contains("arm")) {
                System.out.println(osArch);
                System.loadLibrary("just_jni-arm-android-r9-gcc46-mt-1");
            } else {
                System.out.println("Arch " + osArch + " not supported.");
           }
        } catch (Throwable e) {
        	e.printStackTrace();
        	return -1;
        }
        return 0;
    }

    public native static String GetVersion();

    public native static long StartEngine(String gid, String pid, String auth);

    public native static long StopEngine();

    public native static long SetConfig(String module, String section, String key, String value);

    public native static String GetConfig(String module, String section, String key);

    public interface CallBack
    {
    	boolean invoke(long context, int err);
    };
    
    public static class DownloadStatistic
    {
        public long total_size;
        public long finish_size;
        public int speed;
        @Override
        public String toString()
        {
            return "Download_Statistic [total_size=" + total_size + ", finish_size=" + finish_size + ", speed=" + speed
                    + "]";
        }
    }

    public native static long DownloadOpen(String playlink, String format, String save_filename, CallBack callback);

    public native static void DownloadClose(long handle);

    public native static long GetDownloadInfo(long handle, DownloadStatistic stat);

    public static class SampleBuffer
    {
    	public byte[] data;
    	public int len;
    }
    
    public interface GetSampleBuffersCallBack
    {
    	boolean invoke(long context, SampleBuffer buffers);
    };
    
    public interface FreeSampleCallBack
    {
    	boolean invoke(long context);
    };
    
    public static class CaptureConfigData
    {
    	public int stream_count;
    	public int flags;
    	public GetSampleBuffersCallBack get_sample_buffers;
    	public FreeSampleCallBack free_sample;
    };
    
    public static class StreamInfo
    {
    	public int type;
    	public int sub_type;
    	public int time_scale;
    	public int bitrate;
    	public int __union0;
    	public int __union1;
    	public int __union2;
    	public int __union3;
    	public int format_type;
    	public int format_size;
    	public ByteBuffer format_buffer;
    };
    
    public static class Sample
    {
    	public int itrack;
    	public int flags;
    	public long time;
    	public long decode_time;
    	public int composite_time_delta;
    	public int duration;
    	public int size;
    	public ByteBuffer buffer;
    	public long context;
    };
    
    public native static long CaptureCreate(String name, String dest);
    
    public native static long CaptureInit(long handle, CaptureConfigData config);
    
    public native static long CaptureSetStream(long handle, int index, StreamInfo info);
    
    public native static long CapturePutSample(long handle, Sample sample);
    
    public native static long CaptureDestroy(long handle);

    public static void main(String[] argv)
    {
    	load();
        SetConfig("trip", "trip.client.UdpManager", "uid", "25e5a06d29804f83a71207872500df0b");
        System.out.println(GetConfig(null, "just", "libname"));
        System.out.println(GetConfig("trip", "trip.client.Bootstrap", "url"));
        StartEngine(argv[0], argv[1], argv[2]);
        StopEngine();
    }
}
