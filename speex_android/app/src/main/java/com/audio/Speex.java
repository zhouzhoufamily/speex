package com.audio;

public class Speex  {

    /* quality
     * 1 : 4kbps (very noticeable artifacts, usually intelligible)
     * 2 : 6kbps (very noticeable artifacts, good intelligibility)
     * 4 : 8kbps (noticeable artifacts sometimes)
     * 6 : 11kpbs (artifacts usually only noticeable with headphones)
     * 8 : 15kbps (artifacts not usually noticeable)
     */
    private static final int DEFAULT_COMPRESSION = 8;

    Speex() {
    }
    //初始化方法
    public void init() {
        load();
        open(DEFAULT_COMPRESSION);
    }
    private void load() {
        try {
            //加载对应我们刚刚生成的.so文件。这里speex指的是libs文件夹中对应硬件构架文件夹下的的libsspeex.so
            //至于前置的libs是生成前自动补充的
            System.loadLibrary("speex");
        } catch (Throwable e) {
            e.printStackTrace();
        }
    }
    //设置默认压缩率
    public native int open(int compression);
    public native int getFrameSize();
    //解码
    public native int decode(byte encoded[], short lin[], int size);
    //编码
    public native int encode(short lin[], int offset, byte encoded[], int size);
    public native void close();
}