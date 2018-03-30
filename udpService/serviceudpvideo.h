#ifndef SERVICEUDPVIDEO_H
#define SERVICEUDPVIDEO_H

#include <QObject>
#include <QtNetwork/QUdpSocket>
#include"serialPortService/serviceserialpayload.h"
#include <QThread>
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
