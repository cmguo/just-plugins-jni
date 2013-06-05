package com.pplive.sdk;

import java.util.Properties;

public class PPBOX
{

    public static final int LEVEL_FATAL = 1;
    public static final int LEVEL_ERROR = 2;
    public static final int LEVEL_WARN = 3;
    public static final int LEVEL_INFO = 4;
    public static final int LEVEL_DEBUG = 5;
    public static final int LEVEL_TRACE = 6;
    
    public static String libPath = ".";
    public static String logPath = ".";
    public static boolean logOn = true;
    public static int logLevel = LEVEL_DEBUG;

     public static long load()
    {
        try {
            Properties props = System.getProperties();
            String libPath = props.getProperty("java.library.path");
            System.out.println("java.library.path = " + libPath);
            String osArch = props.getProperty("os.arch");
            System.out.println("os.arch = " + osArch);
            if (osArch != null && osArch.contains("x86"))             {
                System.loadLibrary("libppbox_jni-win32-msvc90-mt-gd-1.2.0");
            } else if (osArch != null && osArch.contains("mips")) {
                System.loadLibrary("ppbox_jni-mips-android-gcc44-mt-1.2.0");
            } else if (osArch != null && osArch.contains("i386")) {
                System.loadLibrary("ppbox_jni-linux-x86-gcc44-mt-gd-1.2.0");
            } else {
                System.loadLibrary("ppbox_jni-armandroid-r4-gcc44-mt-1.2.0");
            }
        } catch (Throwable e) {
            e.printStackTrace();
            return -1;
        }
        return 0;
    }

    public native static long StartEngine(String gid, String pid, String auth);

    public native static long StopEngine();

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
        public int format_type;
        public int format_size;
        public byte[] format_buffer;
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
        public byte[] buffer;
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
        StartEngine(argv[0], argv[1], argv[2]);

        long capture = CaptureCreate("test", "rtmp://192.168.14.205/live/capture?format=rtm");

        CaptureConfigData config = new CaptureConfigData();
        config.stream_count = 1;
        config.free_sample = new FreeSampleCallBack() {
            @Override
            public boolean invoke(long context) {
                return true;
            }
        };
        CaptureInit(capture, config);

        StreamInfo info = new StreamInfo();
        info.type = fourcc("VIDE");
        info.sub_type = fourcc("NV16");
        info.time_scale = 30;
        info.bitrate = 0;
        info.__union0 = 640;
        info.__union1 = 480;
        info.__union2 = 25;
        info.format_type = 0;
        info.format_size = 0;
        info.format_buffer = new byte[0];
        CaptureSetStream(capture, 0, info);

        Sample sample = new PPBOX.Sample();
        sample.itrack = 0;
        sample.flags = 0;
        sample.time = 0;
        sample.decode_time = 0;
        sample.composite_time_delta = 0;
        sample.duration = 0;
        sample.size = 0;
        sample.buffer = new byte[0];
        CapturePutSample(capture, sample);

        try {
            Thread.sleep(10000);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

        StopEngine();
    }

    private static int fourcc(String f)
    {
        byte[] bytes = f.getBytes();
        return (int)bytes[0] << 24 
           | (int)bytes[2] << 16 
           | (int)bytes[1] << 8 
           | (int)bytes[0];
    }
}
