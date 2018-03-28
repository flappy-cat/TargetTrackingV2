#ifndef VIDEOENCODEH264_H
#define VIDEOENCODEH264_H


extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavutil/opt.h"
#include "libavutil/imgutils.h"
}

#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgproc/types_c.h"
#include "opencv2/core/core.hpp"
using namespace cv;

#include "udpsendservice.h"
#include <QHostAddress>

#include <QObject>
#include "trackalgorithm.h"
#include "QTime"
#include <QThread>
class VideoEncodeH264 : public QThread
{
    Q_OBJECT
public:
    VideoEncodeH264(QObject* parent = 0);
    ~VideoEncodeH264();
protected:
    void run();
public:
    void StartEncodeLoop();

public:
    //--------函数声明------//
    void GlobalInit();
    int InitFFmpeg();
    void CloseFFmpeg();
    void convertBGR2YUV420(Mat in, unsigned char* out_y, unsigned char* out_u, unsigned char* out_v);

    VideoCapture cap;
    TrackAlgorithm trackAlgorithm;

    //--------变量声明-----------//
    char *filename_out;
    FILE *fp_out;

    UdpSendService* pUdpSendInstance;

    Mat capFrame;
    Mat resizeFrame;

    AVCodec *pCodec;
    AVCodecContext *pCodecCtx;
    AVFrame *pFrame;
    AVPacket pkt;

    int64_t framecnt;
    int64_t capFrameCnt;

    int ret, got_output;

    unsigned char* data_y;
    unsigned char* data_u;
    unsigned char* data_v;
    QByteArray bufVideoSend;
    //=======For evaluating the time
    QTime m_timeStart;
    QTime m_timeEnd;
};

#endif // VIDEOENCODEH264_H
