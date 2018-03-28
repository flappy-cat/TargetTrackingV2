#include "dataparser4payload.h"
#include "globalsettings.h"
#include "udpsendservice.h"
#include "datamanager.h"

DataParser4Payload::DataParser4Payload()
{
    dataBuffer.resize (1024);
    dataBuffer.fill (0);
    nBufIndex = 0;
    frameLen = 0;
}

DataParser4Payload::~DataParser4Payload()
{

}

bool DataParser4Payload::ParseDownData(QByteArray& bufData)
{
    if(bufData.isEmpty ())
    {
        return false;
    }
    bufData.resize (bufData.length ());
    for(int i=0; i<bufData.size (); i++)
    {
        if(nBufIndex == 0)
        {
            if(bufData[i] == 0xEB)
            {
                dataBuffer[nBufIndex] = bufData[i];
                nBufIndex = 1;
            }
            else
            {
                nBufIndex = 0;
            }
            continue;
        }

        if(nBufIndex == 1)
        {
            if(bufData[i] == 0x90)
            {
                dataBuffer[nBufIndex] = bufData[i];
                nBufIndex = 2;

            }
            else
            {
                nBufIndex = 0;
            }
            continue;
        }
        if(nBufIndex >= 2 && nBufIndex <= 13)//frameLen)
        {
            dataBuffer[nBufIndex] = bufData[i];

        if(nBufIndex ==13)
        {
            frameLen = ((dataBuffer[12]&0xff)<<8)|(dataBuffer[13]&0xff);
        }
            nBufIndex++;
            continue;
        }
        if(nBufIndex >13 && nBufIndex < 14 + frameLen + 2)
        {
            dataBuffer[nBufIndex] = bufData[i];
            nBufIndex++;
            continue;
        }
        if(nBufIndex == 14 + frameLen + 2)
        {
            if(CheckFramePayload (dataBuffer) == true)//CRC check
            {
                ParseFramePayload(dataBuffer);
            }

            dataBuffer.clear ();
            nBufIndex = 0;
            frameLen = 0;
        }
    }

    return true;
}


bool DataParser4Payload::CheckFramePayload (QByteArray& bufData)
{
    quint16 crcResult = GlobalSettings::CRC_check (bufData.mid (2, bufData.length ()-4));
    quint16 tmp = bufData[bufData.length ()-2]<<8 | bufData[bufData.length ()-1];

    if(crcResult == tmp)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void DataParser4Payload::ParseFramePayload (QByteArray& bufData)
{
    //Data obtained from Payload via SerialPort(RS422).
    //Data chain : Payload (Origin)--> Jetson(PASSBY) --> ADT ......>GDT-->GCS(Receiver)
    //Prot EB 90 23 ...

    GlobalSettings *pGlobalSetting = GlobalSettings::getInstance ();

    if(bufData[2] == 0x23)//
    {
        UdpSendService* pUdpSendInstance = UdpSendService::GetInstance ();
        pUdpSendInstance->udpSocket->writeDatagram (bufData,
                                                    QHostAddress(pGlobalSetting->m_str_RemoteAddr_IP),
                                                    pGlobalSetting->m_uint_RemoteAddr_Port);
#if 0
//QString strOut;
//strOut = "Payload(No. 0x23) --[UDP]--> ADT\n";
//qDebug()<<strOut;
#endif
        DataManager *pDataManager = DataManager::GetInstance ();

        pDataManager->ctrlMode = bufData[14];

        unsigned char tempBuf[4];
        float *val = NULL;

        tempBuf[0] = bufData[18];
        tempBuf[1] = bufData[17];
        tempBuf[2] = bufData[16];
        tempBuf[3] = bufData[15];
        val = (float*)(&tempBuf);
        pDataManager->pitchAngle_r = *val;

        tempBuf[0] = bufData[22];
        tempBuf[1] = bufData[21];
        tempBuf[2] = bufData[20];
        tempBuf[3] = bufData[19];
        val = (float*)(&tempBuf);
        pDataManager->yawAngle_r = *val;

        if(pDataManager->focusZoom != (u_int8_t)bufData[23])
        {
            pDataManager->focusZoom = (u_int8_t)bufData[23];

            if(pDataManager->focusZoom < 1) pDataManager->focusZoom = 1;
            if(pDataManager->focusZoom > 30) pDataManager->focusZoom =30;

            pDataManager->bIsResetTracking = true;
        }



        pDataManager->dayNightReverse = bufData[24];
        pDataManager->anti_shake = bufData[25];
        pDataManager->electronicZoom = (u_int8_t)bufData[26];

        tempBuf[0] = bufData[30];
        tempBuf[1] = bufData[29];
        tempBuf[2] = bufData[28];
        tempBuf[3] = bufData[27];
        val = (float*)(&tempBuf);
        pDataManager->measureDistance = *val;
    }
}













