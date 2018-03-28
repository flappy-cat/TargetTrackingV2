#include "serviceudpvideo.h"
#include "globalsettings.h"
#include "datamanager.h"
#include "serviceserialpayload.h"
#include <vector>
using namespace std;
#include <QDebug>
#include "servicevideoproc.h"


ServiceUDPVideo::ServiceUDPVideo(QObject *parent) : QObject(parent)
{
    m_pUDPSocket_Video = new QUdpSocket(this);
    GlobalSettings *pInstance = GlobalSettings::getInstance ();
    m_pUDPSocket_Video->bind (QHostAddress(pInstance->m_str_LocalAddr_IP),pInstance->m_uint_LocalAddr_Port,QUdpSocket::ShareAddress);
    qDebug()<<"UDP bind Param: IP="<<pInstance->m_str_LocalAddr_IP<<",port="<<pInstance->m_uint_LocalAddr_Port;
    connect (m_pUDPSocket_Video,SIGNAL(readyRead()),this,SLOT(Parse_ReceivedData()));

    sycBytesNum = 0;
    memset (sycBuf, 0, SYC_BUF_SIZE);
    gndRecvFrameType = FRAME_DEFAULT;
    frameLen = 0;
    qDebug()<<"Service UDP Video has Initialized!";
}

void ServiceUDPVideo::Parse_ReceivedData()
{
    //GlobalSettings* pInstance = GlobalSettings::getInstance ();

    while(m_pUDPSocket_Video->hasPendingDatagrams ())
    {
    //    QByteArray dataArray;
        dataArray.resize (m_pUDPSocket_Video->pendingDatagramSize ());
        m_pUDPSocket_Video->readDatagram (dataArray.data (), dataArray.size ());
        //==================Debug region below======================================
       ParseGndRecvData ((unsigned char*)dataArray.data (), dataArray.size ());
    }
}


bool ServiceUDPVideo::ParseGndRecvData (unsigned char* bufData, int dataLen)
{

    if(bufData == NULL || dataLen <= 0)
    {
        return false;
    }

    for(int i=0; i<dataLen; i++)
    {
        if (sycBytesNum == 0)
        {
            if (bufData[i] == 0xEB)
            {
                sycBuf[sycBytesNum] = bufData[i];
                sycBytesNum = 1;
            }
            else
            {
                sycBytesNum = 0;
            }
            continue;
        }

        if (sycBytesNum == 1)
        {
            if (bufData[i] == 0x90)
            {
                sycBuf[sycBytesNum] = bufData[i];
                sycBytesNum = 2;
            }
            else
            {
                sycBytesNum = 0;
            }
            continue;
        }

        if ((sycBytesNum >= 2) && (sycBytesNum <= 13))
        {
            sycBuf[sycBytesNum] = bufData[i];
            if(sycBytesNum ==13)
            {
                frameLen = sycBuf[12]<<8 | sycBuf[13];
                //qDebug()<<"package length"<<frameLen;

            }
            sycBytesNum++;
            continue;
        }
        if((sycBytesNum > 13) && (sycBytesNum < (14 + frameLen +2)))
        {
            sycBuf[sycBytesNum] = bufData[i];
            sycBytesNum++;


        }
#ifdef FLAPPYDEBUG
 //qDebug()<<sycBytesNum<<" ,frameLen = "<<frameLen;
#endif
        if (sycBytesNum == (14 + frameLen +2))
        {
            //qDebug()<<"Get data: GCS--[UDP]-->GDT.....>ADT-->JET\n";
            QByteArray bufCheckData((char*)sycBuf , sycBytesNum);
            //qDebug()<<bufCheckData;
           if(CheckDataFrame (bufCheckData) == true)//CRC check
            {
                //qDebug()<<"Data obtained from ADT via UDP  CRC check TRUE\n";
                //QByteArray sycArray(sycBuf,sycBytesNum);
                ParseDataFrame(sycBuf, sycBytesNum);
            }
            else
            {
#ifdef FLAPPYDEBUG
            qDebug()<<"Receive Data CRC check ERROR!\n";
#endif
            }

            memset (sycBuf, 0, SYC_BUF_SIZE);
            sycBytesNum = 0;
            frameLen = 0;
        }


    }

    return true;
}


bool ServiceUDPVideo::CheckDataFrame (QByteArray& bufData)
{
    quint16 crcResult = GlobalSettings::CRC_check(bufData.mid(2, bufData.length ()-4));
    quint16 tmp = bufData[bufData.length ()-2]<<8 | bufData[bufData.length () - 1];

    if(crcResult == tmp)
    {
        return true;
    }
    else
    {
        return false;
    }

}


void ServiceUDPVideo::ParseDataFrame (unsigned char *bufData, int dataLen)
{
    //DataManager *pDataManager = DataManager::GetInstance();

    if(bufData[2] == 0x12) //image control frame
    {
        if(bufData[14] == 0x01)//target select info
        {
            qDebug()<<"target select info\n";
            ParseTargetRectFrame (bufData, dataLen);// parse rect info
            //ParseTargetFeatureFrame (bufData, dataLen);// parse feature

            //pDataManager->bIsStartTracking = true;
            //pDataManager->bIsResetTracking = true;
        }
        else if(bufData[14] == 0x02)//image ctrl info
        {
            ParseImageControlFrame (bufData, dataLen);
        }
    }
    else if(bufData[2] == 0x13)//payload frame
    {
      //  bufData[2] = 0x18;
        //Need treated seperately?
        ServiceSerialPayload* pService = ServiceSerialPayload::getInstance ();
       // qDebug()<<"PayLoad ctrl Frame\n";
       // vector<unsigned char> vecPayload;
        vecPayload.clear();
        vecPayload.resize(dataLen);
        memcpy(vecPayload.data(), bufData, dataLen);

        if(pService->m_pSerial_Payload->isOpen ())
        {
            //qDebug()<<"From UDP to serialport";

            //pService->Send_Data (vecPayload);
            QByteArray bufArray((char*)bufData , dataLen);
            //qDebug()<<bufArray;
            emit Send_Data_signal (bufArray);
            //for(int i=0;i<vecPayload.size ();i++)
                //qDebug("%x",vecPayload[i]);
        } //pService->m_pSerial_Payload->Send(vecPayload);
        else
        {
            //pService->m_pSerial_Payload->Open();
            qDebug()<<"Payload serialPort is not OPEN!\n";
        }

        /*
        switch(bufData[14])
        {
        case 0x01://pod control
            ParsePodCtrlFrame (bufData, dataLen);
            break;
        case 0x02://spinning top zero
            ParseSpinningTopFrame (bufData, dataLen);
            break;
        case 0x03://visible camera ctrl
            ParseVisibleCameraCtrlFrame (bufData, dataLen);
            break;
        case 0x04://infrared camera ctrl
            ParseInfraredCameraCtrlFrame (bufData, dataLen);
            break;
        case 0x05://laser distance measure
            ParseLaserDistanceMeasureCtrlFrame (bufData, dataLen);
            break;
        default:
            break;
        }
        */

    }

    return;
}


void ServiceUDPVideo::ParseTargetRectFrame (unsigned char *bufData, int dataLen)
{
    DataManager *pDataManager = DataManager::GetInstance();
    if(dataLen < 22) return;
    int rect_x = ( int(bufData[15]) << 8 ) | int(bufData[16]);//高字节在前，低字节在后
    int rect_y = ( int(bufData[17]) << 8) | int(bufData[18]);
    int rect_width = ( int(bufData[19]) << 8) | int(bufData[20]);
    int rect_height = ( int(bufData[21]) << 8) | int(bufData[22]);




    if(rect_x<0 || rect_x>720 || rect_y >576 ||rect_y <0 || rect_width <7||
            (rect_x + rect_width) >720 || rect_height<7 || (rect_y + rect_height) >576)
    {
        pDataManager->bIsStartTracking = false;
        pDataManager->bIsResetTracking = false;
        return;
     }


//    pDataManager->recvTargetRect.tl() = cv::Point(rect_x, rect_y);
//    pDataManager->recvTargetRect.br() = cv::Point(rect_x+rect_width, rect_y+rect_height);
    
    pDataManager->bIsStartTracking = true;
    pDataManager->bIsResetTracking = true;
    
    pDataManager->recvTargetRect.x = rect_x;
    pDataManager->recvTargetRect.y = rect_y;
    pDataManager->recvTargetRect.width = rect_width;
    pDataManager->recvTargetRect.height = rect_height;


    ParseTargetFeatureFrame (bufData, dataLen);// parse feature
#ifdef FLAPPYDEBUG
//    qDebug("receive rect is %d %d %d %d\n", pDataManager->recvTargetRect.x,
//           pDataManager->recvTargetRect.y, pDataManager->recvTargetRect.width,
//           pDataManager->recvTargetRect.height);
#endif
}


void ServiceUDPVideo::ParseTargetFeatureFrame (unsigned char *bufData, int dataLen)
{
    if(dataLen < 24) return;
    DataManager *pDataManager = DataManager::GetInstance();

    short featureLen = bufData[23]<<8 | bufData[24];


    if(featureLen == 100)
    {
        for (int i = 25; i< featureLen+25; i+=2)
        {
            unsigned char tempBuf[2];
            tempBuf[0] = bufData[i+1];
            tempBuf[1] = bufData[i];
            //qDebug("%x %x ", bufData[i+1], bufData[i]);
            

            unsigned short *a = (unsigned short*)(&tempBuf);
            pDataManager->featureVisible[(i-25)/2] = *a;

            //qDebug("%d\n", *a);
        }

        //qDebug()<<"feature visible\n";
    }
    else if(featureLen == 30)
    {
        for (int i = 25; i< featureLen+25; i+=2)
        {
            unsigned char tempBuf[2];
            tempBuf[0] = bufData[i+1];
            tempBuf[1] = bufData[i];
            //qDebug("%x %x ", bufData[i+1], bufData[i]);

            unsigned short *a = (unsigned short*)(&tempBuf);
            pDataManager->featureInfrared[(i-25)/2] = *a;
        }
    }


}


void ServiceUDPVideo::ParseImageControlFrame (unsigned char *bufData, int dataLen)
{
    DataManager *pDataManager = DataManager::GetInstance();

    if(dataLen < 15) return;

    if((bufData[15] != 0x00) && (bufData[15] != 0x01) && (bufData[15]!= 0x02))
    {
#ifdef FLAPPYDEBUG
        qDebug()<<"Invalid video source\n";
#endif
        return;
    }

    if((bufData[15] != 0x00) && (pDataManager->videoSrc != bufData[15]))
    {
        pDataManager->videoSrc = bufData[15];

        pDataManager->bIsStartVideoCapture = false;
        pDataManager->bIsStartTracking = false;
        pDataManager->bIsResetTracking = false;
    }

    if((bufData[16] != 0x00) && (bufData[16] != 0x01) && (bufData[16]!= 0x10))
    {
#ifdef FLAPPYDEBUG
        qDebug()<<"Invalid image control flag \n";
#endif
        return;
    }

    if(bufData[16]!=0x00)
        pDataManager->assistFlag = bufData[16];
#ifdef FLAPPYDEBUG
    qDebug("Get image control CMD : Video Source Priv and Now =[ %x and %x ] ,Assist Flag = %x\n", bufData[15],pDataManager->videoSrc, bufData[16]);
#endif
}

/*
void ServiceUDPVideo::ParsePodCtrlFrame (unsigned char *bufData, int dataLen)
{
    DataManager *pDataManager = DataManager::GetInstance();

    pDataManager->ctrlMode = bufData[15];

    unsigned char pitchBuf[4];
    unsigned char yawBuf[4];

    pitchBuf[0] = bufData[19];
    pitchBuf[1] = bufData[18];
    pitchBuf[2] = bufData[17];
    pitchBuf[3] = bufData[16];

    yawBuf[0] = bufData[23];
    yawBuf[1] = bufData[22];
    yawBuf[2] = bufData[21];
    yawBuf[3] = bufData[20];

    float *pitch = (float*)(&pitchBuf);
    float *yaw = (float*)(&yawBuf);

    pDataManager->pitchVelocity = *pitch;
    pDataManager->yawVelocity = *yaw;

    pDataManager->resetInstruction = bufData[24];

}


void ServiceUDPVideo::ParseSpinningTopFrame (unsigned char *bufData, int dataLen)
{
    DataManager *pDataManager = DataManager::GetInstance();

    pDataManager->adjustAxis = bufData[15];
    pDataManager->adjustStep = bufData[16];

}


void ServiceUDPVideo::ParseVisibleCameraCtrlFrame (unsigned char *bufData, int dataLen)
{
    DataManager *pDataManager = DataManager::GetInstance();

    pDataManager->changeFocalLen = bufData[15];
    pDataManager->restart = bufData[16];
    pDataManager->dayNightReverse = bufData[17];
    pDataManager->anti_shake = bufData[18];
}


void ServiceUDPVideo::ParseInfraredCameraCtrlFrame (unsigned char *bufData, int dataLen)
{
    DataManager *pDataManager = DataManager::GetInstance();

    pDataManager->manualRevise = bufData[15];
    pDataManager->electronicZoom = bufData[16];
    pDataManager->pseudoColorSwitch = bufData[17];
}


void ServiceUDPVideo::ParseLaserDistanceMeasureCtrlFrame (unsigned char *bufData, int dataLen)
{
    DataManager *pDataManager = DataManager::GetInstance();

    pDataManager->startMeasure = bufData[15];
}
*/















