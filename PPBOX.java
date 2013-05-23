package com.pplive.sdk;

import java.util.Properties;
import java.lang.reflect.Field;

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

    public static long load()
    {
    	System.setProperty("java.library.path", 
    			System.getProperty("user.dir") + 
    			";" + System.getProperty("java.library.path"));
        try {
            Properties props = System.getProperties();
            String osArch = props.getProperty("os.arch");
            System.out.println(osArch);
            if (osArch != null && osArch.contains("x86"))             {
                System.loadLibrary("libppbox_jni-win32-msvc90-mt-gd-1.2.0");
            } else if (osArch != null && osArch.contains("mips")) {
                System.loadLibrary("ppbox_jni-mips-android-gcc44-mt-1.2.0");
            } else if (osArch != null && osArch.contains("i386")) {
                System.loadLibrary("ppbox_jni-linux-x86-gcc44-mt-1.2.0");
            } else {
                System.loadLibrary("ppbox_jni-armandroid-r4-gcc44-mt-1.2.0");
            }
        } catch (Throwable e) {
        	e.printStackTrace();
        	return -1;
        }
        return 0;
    }

    public native static long StartP2PEngine(String gid, String pid, String auth);

    public native static long StopP2PEngine();

    public native static long DownloadOpen(String playlink, String format, String save_filename, Field callback);

    public native static void DownloadClose(long handle);

    public native static long GetDownloadInfo(long handle, Download_Statistic stat);

    public static void main(String[] argv)
    {
    	load();
        StartP2PEngine(argv[0], argv[1], argv[2]);
        StopP2PEngine();
    }
}
