#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include "opencv2/opencv.hpp"


#include <vector>
using namespace std;

#define GND_SEND_BUF_SIZE  20
#define CAMERA_SEND_BUF_SIZE  20
#define FIRE_CTRL_SEND_BUF_SIZE 20

#define PI 3.1415926
#include<QMutex>
class DataManager
{
private:
    DataManager();
    ~DataManager();

    static DataManager* instance;

public:
    static DataManager* GetInstance();

public:
    unsigned char gnd_Send_Buf[GND_SEND_BUF_SIZE];
    unsigned char camera_Send_Buf[CAMERA_SEND_BUF_SIZE];
    unsigned char fire_Ctrl_Send_Buf[FIRE_CTRL_SEND_BUF_SIZE];

    /*---------state---------*/
    bool bIsStartVideoCapture;
    bool bIsStartTracking;
    bool bIsResetTracking;

    /*-------------- received data ----------------*/
    //image ctrl info
    cv::Rect recvTargetRect;//目标跟踪框
    vector<float> vecTargetFeature;//目标特征信息
    unsigned short featureVisible[50];
    unsigned short featureInfrared[15];
    unsigned char videoSrc;//0x01:visible video  0x02:infrared video  init:0x01
    unsigned char assistFlag;//0x01:open   0x10:close    init:0x10

    //payload status info
    //1.pod ctrl
    unsigned char ctrlMode;//0x10:manual  0x01:auto    init:0x10
    float pitchAngle_r;//init:0.0
    float yawAngle_r;//init:0.0
    //2.visible camera ctrl
    u_int8_t focusZoom;//init:1
    unsigned char dayNightReverse;//0x55:night visible close   0xAA:night visible open    init:0x55
    unsigned char anti_shake;//0xAC:open   0x53:close   init:0x53
    //3.infrared camera ctrl
    u_int8_t electronicZoom;//init:1
    //4.laser distance measure
    float measureDistance;

    //predict missile position
    float pitchAngle_predict;
    float yawAngle_predict;
    QMutex mutex_predictdata;
    /*--------------- send data -------------*/
    //payload ctrl info
//    float pitchAngle_payload;
//    float yawAngle_payload;

    //target position info
    float pitchAngle_target;
    float yawAngle_target;

    cv::Mat cameraMatrix[30];

};

#endif // DATAMANAGER_H
