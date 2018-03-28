#include "datamanager.h"
#include<qobject.h>
DataManager::DataManager()
{
    memset(gnd_Send_Buf, 0, GND_SEND_BUF_SIZE);
    memset(camera_Send_Buf, 0, CAMERA_SEND_BUF_SIZE);
    memset(fire_Ctrl_Send_Buf, 0, FIRE_CTRL_SEND_BUF_SIZE);

    /*------------ state -----------*/
    bIsStartVideoCapture = false;
    bIsStartTracking = false;
    bIsResetTracking = false;

    /*------------ received data ---------------*/
    //image ctrl info
    recvTargetRect = cv::Rect(0,0,0,0);
    memset(featureVisible, 0, 50);
    memset(featureInfrared, 0, 15);
    videoSrc = 0x01;//visible video
    assistFlag = 0x10;//close

    //payload ctrl info
    //1.pod ctrl
    ctrlMode = 0x10;//manual
    pitchAngle_r = 0.0;
    yawAngle_r = 0.0;
    //2.visible camera ctrl
    focusZoom = 1;
    dayNightReverse = 0x55;
    anti_shake = 0x53;
    //3.infrared camera ctrl
    electronicZoom = 1;
    //4.laser distance measure
    measureDistance = 0.0;

    //predict missile position
    pitchAngle_predict = 0.0;
    yawAngle_predict = 0.0;


    /*--------------- send data ----------------*/
//    pitchAngle_payload = 0.0;
//    yawAngle_payload = 0.0;

    pitchAngle_target = 0.0;
    yawAngle_target = 0.0;

    cv::FileStorage ymlFile;
    ymlFile.open ("/home/nvidia/appsettings/algorithm/file/CalibrateInformationVisible.yml" , cv::FileStorage::READ);

    for(int i = 0;i<30;i++)
    {
        cameraMatrix[i] = cv::Mat(3,3,CV_32FC1,cv::Scalar::all (0));
        char cameraMatrixName[255];
        sprintf(cameraMatrixName,"cameraMatrix%d",i+1);

        ymlFile[cameraMatrixName]>>cameraMatrix[i];//...
#if 0
        qDebug("%s=[%.6f,%.6f,%.6f,%.6f,%.6f,%.6f,%.6f,%.6f,%.6f]",cameraMatrixName,
               cameraMatrix[i].at<double>(0,0),
               cameraMatrix[i].at<double>(0,1),
               cameraMatrix[i].at<double>(0,2),
               cameraMatrix[i].at<double>(1,0),
               cameraMatrix[i].at<double>(1,1),
               cameraMatrix[i].at<double>(1,2),
               cameraMatrix[i].at<double>(2,0),
               cameraMatrix[i].at<double>(2,1),
               cameraMatrix[i].at<double>(2,2)
               );
#endif
    }


}


DataManager::~DataManager()
{
    if(instance != NULL)
    {
        delete instance;
        instance = NULL;
    }
}


DataManager* DataManager::instance = NULL;

DataManager* DataManager::GetInstance()
{
    if(NULL == instance)
    {
        instance = new DataManager();
    }

    return instance;
}
