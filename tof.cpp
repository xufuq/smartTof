#include "tof.h"

/* 一个frame的尺寸，尺寸计算应该是这样
 * 一共有640*480个像素
 * 每个像素有四个维度的信息
 * 每个信息的存储长度是16位，分开成为两个8位
 * */
#define FRAME_SIZE (640*480*4 * 2)
#define FRAME_BUF_FCNT 10 // 一个buf包含10个frame


Tof::Tof(bool capvideo, QString filename, QObject *parent) : QObject(parent)
{
    CapVideo=capvideo;
    FileName=filename;
    fbuf = new uint8_t[FRAME_SIZE];
    gray_u16 = new uint16_t[FRAME_SIZE];
    dist_u16 = new uint16_t[FRAME_SIZE];
    IrGray = new uint8_t[FRAME_SIZE];
    DistGrayU8 = new uint8_t[FRAME_SIZE];
}


void Tof::myDmcam_Init()
{
    dmcam_dev_t *dev1;
    dmcam_dev_t dev_list[4];

    dmcam_init(NULL); // 初始化摄像头驱动
//    dmcam_log_cfg(LOG_LEVEL_INFO, LOG_LEVEL_TRACE, LOG_LEVEL_NONE);
    if(CapVideo==true) // 使用录的数据
        dev1 = dmcam_dev_open_by_uri(FileName.toLatin1().data());
    else{
        int dev_cnt = dmcam_dev_list(dev_list, 4);
        if(dev_cnt==0){
            qDebug() << "None dmcam devices found" << Qt::endl;
            return;
        }
        else // 存在dmcam设备
            dev1 = dmcam_dev_open(&dev_list[0]); // 打开第0个设备
    }
    if(!dev1){
        qDebug() << "open device failed" << Qt::endl;
        return;
    }
    dev=new dmcam_dev_t(*dev1);
    // 到这里设备成功打开

    int fps = 20; // 设置帧率
    dmcam_param_item_t wparam[2];
    memset(wparam, 0, sizeof(wparam));
    wparam[0].param_id = PARAM_FRAME_RATE;
    wparam[0].param_val.frame_rate.fps = fps;
    wparam[1].param_id = PARAM_INTG_TIME;
    wparam[1].param_val.intg.intg_us = 1000;
    // 设置帧率参数
    bool success = dmcam_param_batch_set(dev, wparam, sizeof(wparam) / sizeof(wparam[0]));
    if(success)
        qDebug() << "The fps has been set as " << fps << Qt::endl;
    else{
        qDebug() << "The fps set failed!" << Qt::endl;
        return;
    }

    // 设置捕获配置
    dmcam_cap_cfg_t cap_cfg = {
        .cache_frames_cnt = FRAME_BUF_FCNT, /* FRAME_BUF_FCNT frames can be cached in frame buffer*/
        .on_frame_ready = NULL, /* No frame ready callback*/
        .on_error = NULL,      /* No error callback */
        .en_save_manually = false,
        .en_save_replay = false, /* 是否保存原始数据（raw data stream）到可回放文件,这里设置了的话应该是自动保存，不需要再调用额外的函数 */
        .en_save_dist_u16 = false, /* disable save dist stream into replay file */
        .en_save_gray_u16 = false, /* disable save gray stream into replay file*/
        .fname_replay = NULL,/* replay filename，在这里设置想要保存的文件的名字 */
        .en_fdev_rewind = false,
    };
    dmcam_cap_config_set(dev, &cap_cfg);

    // 滤波器可以设置镜头矫正等功能,这里设置镜头矫正没有生效
    dmcam_filter_id_e filter_e= DMCAM_FILTER_ID_LEN_CALIB;         /** lens calibration ,使能镜头矫正*/
    dmcam_filter_args_u witem;
    witem.lens_id=1;
    dmcam_filter_enable(dev, filter_e, &witem, sizeof(dmcam_filter_args_u));

    success = dmcam_cap_start(dev);
    if(success)
        qDebug() << "开始采集数据帧 " << fps << Qt::endl;
    else{
        qDebug() << "数据采集失败" << Qt::endl;
        return;
    }
}


void Tof::getFrame()
{
    dmcam_frame_t frinfo;
    int fr_cnt;
    int ImageWidth, ImageHeight; // 数据图像的宽和长

    /* 获取指定数目帧数据，并填入缓冲
     * dev: 目标设备
     * 1：要获取的帧数，当前是1
     * fbuf：获取到的帧数据（像素）
     * 帧数据缓冲长度（10）
     * 第一帧的信息
     * 返回值是获取到的帧数目
     * */
    fr_cnt = dmcam_cap_get_frames(dev, 1, fbuf, FRAME_SIZE, &frinfo);
    qDebug() << "Get " <<  fr_cnt << "frame! " << Qt::endl;
    if(fr_cnt<1)
        return;

    ImageWidth=frinfo.frame_info.width;
    ImageHeight=frinfo.frame_info.height;

    qDebug() << "width: " << ImageWidth << " height: " << ImageHeight<< Qt::endl;

    int dist_len = ImageWidth * ImageHeight; // w*h就是像素数

    dmcam_frame_get_dist_u16(dev, dist_u16, dist_len, fbuf, frinfo.frame_info.frame_size, &frinfo.frame_info);
    dmcam_cmap_gray_u16_to_IR(DistGrayU8, dist_len, dist_u16, dist_len, 255);

    dmcam_frame_get_gray_u16(dev, gray_u16, dist_len, fbuf, frinfo.frame_info.frame_size, &frinfo.frame_info);
    dmcam_cmap_gray_u16_to_IR(IrGray, dist_len, gray_u16, dist_len, 255);
}

void Tof::myDmcam_Uninit()
{
    dmcam_cap_stop(dev);
    dmcam_dev_close(dev);
    dmcam_uninit();
}
