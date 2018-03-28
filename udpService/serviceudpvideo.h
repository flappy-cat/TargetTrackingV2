#ifndef SERVICEUDPVIDEO_H
#define SERVICEUDPVIDEO_H

#include <QObject>
#include <QtNetwork/QUdpSocket>
#include "serviceserialpayload.h"
#include <QThread>
//接收的数据帧类型
//typedef enum _GND_RECV_Frame_Type
//{
//    FRAME_DEFAULT = 0,//缺省值
//    FRAME_TARGET_RECTANGLE = 1,//目标矩形框
//    FRAME_TARGET_FEATURE = 2,//目标特征数据
//    FRAME_CAMERA_CONTROL = 3,//相机控制指令
//    FRAME_POD_CONTROL = 4,//吊舱控制
//    FRAME_TRACKING_SWITCH = 5,//跟踪开启/关闭指令
//    FRAME_ASSIST_FLAG = 6,//辅助标记
//    FRAME_VIDEO_CHANGE = 7//视频切换
//}EnumGndRecvFrameType;

typedef enum _GND_RECV_Frame_Type
{
    FRAME_DEFAULT = 0,//default value
    FRAME_IMAGE_CONTROL_INFO = 1,//image control
    FRAME_PAYLOAD_INFO = 2,//payload
}EnumGndRecvFrameType;

#define SYC_BUF_SIZE 10*1024


class ServiceUDPVideo : public QObject
{
    Q_OBJECT
public:
    explicit ServiceUDPVideo(QObject *parent = 0);
private:
    QUdpSocket* m_pUDPSocket_Video;

signals:
    void Send_Data_signal(QByteArray bufArray);
public slots:
    void Parse_ReceivedData();

private:
    QByteArray dataArray;
    int sycBytesNum;//同步字节个数
    short frameLen;//

    unsigned char sycBuf[SYC_BUF_SIZE];//同步buffer
    EnumGndRecvFrameType gndRecvFrameType;//地面接收帧类型


    std::vector<unsigned char> vecPayload;
    bool ParseGndRecvData(unsigned char* bufData, int dataLen);

    bool CheckDataFrame(QByteArray& bufData);//校验
    void ParseDataFrame(unsigned char* bufData, int dataLen);//

    //image ctrl info
    void ParseTargetRectFrame(unsigned char* bufData, int dataLen);//解析目标矩形框
    void ParseTargetFeatureFrame(unsigned char* bufData, int dataLen);//解析目标特征信息
    void ParseImageControlFrame(unsigned char* bufData, int dataLen);//

    //payload ctrl info
    void ParsePodCtrlFrame(unsigned char* bufData, int dataLen);//pod ctrl
    void ParseSpinningTopFrame(unsigned char* bufData, int dataLen);
    void ParseVisibleCameraCtrlFrame(unsigned char* bufData, int dataLen);
    void ParseInfraredCameraCtrlFrame(unsigned char* bufData, int dataLen);
    void ParseLaserDistanceMeasureCtrlFrame(unsigned char* bufData, int dataLen);

};

#endif // SERVICEUDPVIDEO_H
