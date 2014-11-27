package com.pplive.sdk;

import java.util.Properties;
import java.nio.ByteBuffer;

public class Test
{

    public static void main(String[] argv)
    {
        JUST.load();
        JUST.StartEngine(argv[0], argv[1], argv[2]);

        long capture = JUST.CaptureCreate("test", "rtmp://192.168.14.205/live/capture?format=rtm&mux.Muxer.video_codec=AVC1&mux.Encoder.AVC1.param={profile:baseline,ref:2}");

        JUST.CaptureConfigData config = new JUST.CaptureConfigData();
        config.stream_count = 1;
        config.flags = 1;
        config.free_sample = new JUST.FreeSampleCallBack() {
            @Override
            public boolean invoke(long context) {
                System.out.println("FreeSampleCallBack context: " + context);
                return true;
            }
        };
        JUST.CaptureInit(capture, config);

        JUST.StreamInfo info = new JUST.StreamInfo();
        info.type = fourcc("VIDE");
        info.sub_type = fourcc("NV12");
        info.time_scale = 15;
        info.bitrate = 0;
        info.__union0 = 176;
        info.__union1 = 144;
        info.__union2 = 15;
        info.__union3 = 1;
        info.format_type = 0;
        info.format_size = 0;
        info.format_buffer = ByteBuffer.allocateDirect(0);
        JUST.CaptureSetStream(capture, 0, info);

        JUST.Sample sample = new JUST.Sample();
        sample.itrack = 0;
        sample.flags = 0;
        sample.time = 0;
        sample.decode_time = 0;
        sample.composite_time_delta = 0;
        sample.duration = 1;
        sample.size = 38016;//50688;
        sample.buffer = ByteBuffer.allocateDirect(38016);
        sample.context = 1;
        JUST.CapturePutSample(capture, sample);

        sample.decode_time = 1;
        sample.context = 2;
        JUST.CapturePutSample(capture, sample);

        try {
            Thread.sleep(1000);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

        JUST.CaptureDestroy(capture);

        try {
            Thread.sleep(10000);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

        JUST.StopEngine();
    }

    private static int fourcc(String f)
    {
        byte[] bytes = f.getBytes();
        return (int)bytes[3] << 24 
           | (int)bytes[2] << 16 
           | (int)bytes[1] << 8 
           | (int)bytes[0];
    }
}
