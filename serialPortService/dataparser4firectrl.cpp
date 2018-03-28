#include "dataparser4firectrl.h"
#include "globalsettings.h"
#include "datamanager.h"
#include<QDebug>
DataParser4FireCtrl::DataParser4FireCtrl()
{
    dataBuffer.resize (FIRECTRL_BUF_SIZE);
    dataBuffer.fill (0);
    nBufIndex = 0;
    frameLen = 0;
}

DataParser4FireCtrl::~DataParser4FireCtrl ()
{

}

bool DataParser4FireCtrl::ParseDownData (QByteArray& bufData)
{
    if(bufData.isEmpty ())
    {
        return false;
    }
    dataBuffer.resize (bufData.length ());
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
            if(CheckFrameFireCtrl(dataBuffer) == true)//CRC check
            {
                ParseFrameFireCtrl(dataBuffer);
            }

            dataBuffer.clear ();
            nBufIndex = 0;
            frameLen = 0;
        }
    }

    return true;
}


bool DataParser4FireCtrl::CheckFrameFireCtrl (QByteArray& bufData)
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


void DataParser4FireCtrl::ParseFrameFireCtrl (QByteArray& bufData)
{
    if(bufData[2] == 0x15)
    {

#if 0
    QString strOut;
    strOut = "FileCtrl(No. 0x15) --[SerilPort]--> Image\n";
    qDebug()<<strOut;

    qDebug()<<"Received FireCtrl Data:\n";
    for(int i=0;i<bufData.size();i++)    qDebug("%x ,",bufData.at (i));
#endif
        DataManager *pDataManager = DataManager::GetInstance ();

        quint8 tempBuf[4];
        float *val = NULL;

        tempBuf[0] = bufData[17];
        tempBuf[1] = bufData[16];
        tempBuf[2] = bufData[15];
        tempBuf[3] = bufData[14];
        val = (float*)(&tempBuf);
        pDataManager->pitchAngle_predict = *val;

        tempBuf[0] = bufData[21];
        tempBuf[1] = bufData[20];
        tempBuf[2] = bufData[19];
        tempBuf[3] = bufData[18];
        val = (float*)(&tempBuf);
        pDataManager->yawAngle_predict = *val;

        pDataManager->mutex_predictdata.lock ();

        static unsigned int tt =0;
        if(++tt%30==0)
        {
            qDebug()<<"Target predict Angle="<<pDataManager->yawAngle_predict<<","<<pDataManager->pitchAngle_predict;
        }

        pDataManager->yawAngle_predict = pDataManager->yawAngle_predict*PI/180;
        pDataManager->pitchAngle_predict = pDataManager->pitchAngle_predict*PI/180;
        pDataManager->mutex_predictdata.unlock ();


    }
}











