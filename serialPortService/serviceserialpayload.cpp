#include "serviceserialpayload.h"
#include "globalsettings.h"
#include <QDebug>
#include "datamanager.h"



ServiceSerialPayload::ServiceSerialPayload(QObject *parent) : QObject(parent)
{
    m_bIsServiceStart = false;

    timerSend = new QTimer(this);
    connect(timerSend, &QTimer::timeout, this, &ServiceSerialPayload::SendData); //20171221 bzz
}

ServiceSerialPayload::~ServiceSerialPayload()
{

}

ServiceSerialPayload* ServiceSerialPayload::instance = NULL;
ServiceSerialPayload* ServiceSerialPayload::getInstance ()
{
    if(NULL == instance)
    {
        instance = new ServiceSerialPayload;
    }
    return instance;
}

bool ServiceSerialPayload::StartService()
{
    GlobalSettings *pGlobalSettings = GlobalSettings::getInstance ();

    if(!m_bIsServiceStart)
    {
        QString commNum = pGlobalSettings->m_str_PayloadComm;
        quint32 commBaudrate = pGlobalSettings->m_uint_PayloadPortBaud;
        //std::string strCommNum = std::string((const char*)commNum.toLocal8Bit());
        m_pSerial_Payload = new QSerialPort();

        m_pSerial_Payload->setPortName (commNum);
        m_pSerial_Payload->setBaudRate (commBaudrate);
        m_pSerial_Payload->setDataBits (QSerialPort::Data8);
        m_pSerial_Payload->setStopBits (QSerialPort::OneStop);
        m_pSerial_Payload->setParity (QSerialPort::NoParity);
        m_pSerial_Payload ->setFlowControl (QSerialPort::NoFlowControl);
        if (m_pSerial_Payload->open (QIODevice::ReadWrite))
        {
            m_bIsServiceStart = true;
#ifdef FLAPPYDEBUG
            qDebug()<<"Open payload serialport successful\n";
#endif
            timerSend->start (PERIOD_CTRL);
            if(m_pSerial_Payload->isOpen ())
             qDebug()<<"Timer start\n";
        }
        else
        {
            m_bIsServiceStart = false;

#ifdef FLAPPYDEBUG
            qDebug()<<"Open payload serialport failed\n";
#endif
        }



        connect (m_pSerial_Payload,SIGNAL(readyRead()),this,SLOT(ReceiveData()));
    }

    return true;
}

bool ServiceSerialPayload::TerminateService()
{
    if(m_pSerial_Payload->isOpen ())
    {
        m_pSerial_Payload->close ();
    }
    return true;
}

void ServiceSerialPayload::Send_Data(QByteArray bufArray)
{

    m_pSerial_Payload->write (bufArray);
    // qDebug()<<"Write to Payload:"<<bufArray.toHex ();
}

void ServiceSerialPayload::SendData()
{
   // static quint32 ii = 0;

    DataManager* pDataManager = DataManager::GetInstance ();

    unsigned char *p = NULL;


    QByteArray bufSend;
    bufSend.resize (24);
    //memset(bufSend.data(), 0x00, 24);
    bufSend.fill(0);

    bufSend[0] = 0xEB;
    bufSend[1] = 0x90;
    bufSend[2] = 0x18;
   // memset(bufSend.data ()+3, 0, 9);
    bufSend[12] = 0x00;
    bufSend[13] = 0x08;

    //===edit 2017-12-29  15:37 --by yh
    //===To avoid overshooting , set the angle value devided by 3.

    float pitchAngle = pDataManager->pitchAngle_target / 2;
    p = (unsigned char*)(&pitchAngle);
    bufSend[14] = *(p+3);
    bufSend[15] = *(p+2);
    bufSend[16] = *(p+1);
    bufSend[17] = *p;
    float yawAngle = pDataManager->yawAngle_target / 2;
    p = (unsigned char*)(&yawAngle);
    bufSend[18] = *(p+3);
    bufSend[19] = *(p+2);
    bufSend[20] = *(p+1);
    bufSend[21] = *p;



    quint16 crcRes = GlobalSettings::CRC_check(bufSend.mid (2, bufSend.length () - 4));
    bufSend[22] = crcRes >> 8;
    bufSend[23] = crcRes;

    //quint32 sendbytes =
#if 0
    static quint32 tt = 0;
    if((tt++)%20 == 0)
        qDebug()<<"Write to Payload:"<<bufSend.toHex ();
#endif
    if((pDataManager->ctrlMode == 0x01) && (pDataManager->bIsStartTracking == true))
    {
        m_pSerial_Payload->write (bufSend);

#ifdef FLAPPYDEBUG1
    static quint32 tt = 0;
    if((tt++)%10 == 0)
        qDebug()<<"Write to Payload:"<<bufSend.toHex ();

#endif
    }
    //if(sendbytes == bufSend.length ())
        //qDebug()<<"Payload CMD transmitted "<<ii++;
}

void ServiceSerialPayload::ReceiveData()
{
    QByteArray dataRaw;

    while(m_pSerial_Payload->bytesAvailable ()>0)
    {
        dataRaw.clear ();
        dataRaw.resize (m_pSerial_Payload->bytesAvailable ());
        dataRaw = m_pSerial_Payload->read (1024);
        m_parser_Payload.ParseDownData (dataRaw);
    }
}














