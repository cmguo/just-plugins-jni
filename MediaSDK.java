package com.pplive.sdk;

import java.util.Properties;

public class MediaSDK
{

    public static final int LEVEL_ERROR = 0;
    public static final int LEVEL_ALARM = 1;
    public static final int LEVEL_EVENT = 2;
    public static final int LEVEL_INFOR = 3;
    public static final int LEVEL_DEBUG = 4;
    public static final int LEVEL_DEBUG1 = 5;
    public static final int LEVEL_DEBUG2 = 6;
    
    public static String libPath = ".";
    public static String logPath = ".";
    public static boolean logOn = true;
    public static int logLevel = LEVEL_DEBUG;

    public static class Download_Statistic
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


    public native static long startP2PEngine(String gid, String pid, String auth);

    public native static long stopP2PEngine();

    // 返回打开下载用例的句柄
    public native static long fileDownloadOpenItem(String playlink, String format, String save_filename);

    public native static void fileDownloadCloseItem(long handle);

    public native static long getFileDownloadItemInfo(long handle, Download_Statistic stat);

    // public native static void insertMedia(InsertMedia[] medias);

    // public native static void getInsertMediaEvent(InsertMediaEvent event);

    static
    {
        try {
            Properties props = System.getProperties();
            String osArch = props.getProperty("os.arch");
            System.out.println(osArch);
            if (osArch != null && osArch.contains("x86"))
            {
                System.loadLibrary("ppbox_jni-android-x86-gcc44-mt-1.0.0");
            } 
            else if (osArch != null && osArch.contains("mips"))
            {
                System.loadLibrary("ppbox_jni-mips-android-gcc44-mt-1.0.0");
            }
            else if (osArch != null && osArch.contains("i386"))
            {
                System.loadLibrary("ppbox_jni-linux-x86-gcc44-mt-1.0.0");
            }
            else
            {
                System.loadLibrary("ppbox_jni-armandroid-r4-gcc44-mt-1.0.0");
            }
        }

        catch (Throwable e) {
        }
    }

    public static void main(String[] argv)
    {
        startP2PEngine(argv[0], argv[1], argv[2]);
        stopP2PEngine();
    }
}
