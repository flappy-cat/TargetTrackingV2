#include "serviceserialfirectrl.h"
#include "globalsettings.h"
#include <QDebug>
#include "datamanager.h"
#include"comutils.h"
ServiceSerialFireCtrl::ServiceSerialFireCtrl(QObject *parent) : QObject(parent)
{
    m_bIsServiceStart = false;

    timerSend = new QTimer(this);
    connect(timerSend, &QTimer::timeout, this, &ServiceSerialFireCtrl::SendData);
}


ServiceSerialFireCtrl::~ServiceSerialFireCtrl()
{

}

ServiceSerialFireCtrl* ServiceSerialFireCtrl::instance = NULL;
ServiceSerialFireCtrl* ServiceSerialFireCtrl::getInstance ()
{
    if(NULL == instance)
    {
        instance = new ServiceSerialFireCtrl;
    }
    return instance;
}

//Start service for fireControl ..
bool ServiceSerialFireCtrl::StartService()
{
    GlobalSettings *pGlobalSettings = GlobalSettings::GetInstance ();

    if(!m_bIsServiceStart)
    {
        QString commNum = pGlobalSettings->m_str_FireCtrlComm;
        quint32 commBaudrate = pGlobalSettings->m_uint_FireCtrlPortBaud;
        //std::string strCommNum = std::string((const char*)commNum.toLocal8Bit());
        m_pSerial_FireCtrl = new QSerialPort();
        m_pSerial_FireCtrl->setPortName (commNum);
        m_pSerial_FireCtrl->setBaudRate (commBaudrate);
        m_pSerial_FireCtrl->setDataBits (QSerialPort::Data8);
        m_pSerial_FireCtrl->setStopBits (QSerialPort::OneStop);
        m_pSerial_FireCtrl->setParity (QSerialPort::NoParity);
        m_pSerial_FireCtrl->setFlowControl (QSerialPort::NoFlowControl);


        //m_pSerial_FireCtrl->ParseDataFunc = boost::bind(&DataParser4FireCtrl::ParseDownData, &m_parser_FireCtrl, _1);
        if (m_pSerial_FireCtrl->open (QIODevice::ReadWrite))
        {
            m_bIsServiceStart = true;
#ifdef FLAPPYDEBUG
            qDebug()<<"StartService fire ctrl successful\n";
#endif
            timerSend->start (PERIOD_CTRL);
        }
        else
        {
            m_bIsServiceStart = false;
#ifdef FLAPPYDEBUG
            qDebug()<<"StartService fire ctrl failed\n";
#endif
        }
        connect (m_pSerial_FireCtrl,SIGNAL(readyRead()),this,SLOT(ReceiveData()));
    }

    return true;
}

bool ServiceSerialFireCtrl::TerminateService()
{
    if(m_pSerial_FireCtrl->isOpen ())
    {
        m_pSerial_FireCtrl->close ();
    }
    return true;
}


void ServiceSerialFireCtrl::SendData()
{
    DataManager* pDataManager = DataManager::GetInstance ();

    unsigned char *p = NULL;

    QByteArray bufSend;
    bufSend.resize (37);
    //memset(bufSend.data(), 0x00, 37);
    bufSend.fill(0);
    bufSend[0] = 0xEB;
    bufSend[1] = 0x90;
    bufSend[2] = 0x25;

    //memset(bufSend.data ()+3, 0, 9);

    bufSend[12] = 0x00;
    bufSend[13] = 0x15;

    bufSend[14] = 0xC5;//self-detect normal && track switch off

    p = (unsigned char*)(&pDataManager->pitchAngle_target);
    bufSend[15] = *(p+3);
    bufSend[16] = *(p+2);
    bufSend[17] = *(p+1);
    bufSend[18] = *p;

    p = (unsigned char*)(&pDataManager->yawAngle_target);
    bufSend[19] = *(p+3);
    bufSend[20] = *(p+2);
    bufSend[21] = *(p+1);
    bufSend[22] = *p;

    p = (unsigned char*)(&pDataManager->pitchAngle_r);
    bufSend[23] = *(p+3);
    bufSend[24] = *(p+2);
    bufSend[25] = *(p+1);
    bufSend[26] = *p;

    p = (unsigned char*)(&pDataManager->yawAngle_r);
    bufSend[27] = *(p+3);
    bufSend[28] = *(p+2);
    bufSend[29] = *(p+1);
    bufSend[30] = *p;

    p = (unsigned char*)(&pDataManager->measureDistance);
    bufSend[31] = *(p+3);
    bufSend[32] = *(p+2);
    bufSend[33] = *(p+1);
    bufSend[34] = *p;


    quint16 crcRes = FuncUtils::CRC_check(bufSend.mid(2, bufSend.length () - 4));
    bufSend[35] = crcRes >> 8;
    bufSend[36] = crcRes;

    //qDebug()<<"Send to FireCtrl\n";
    m_pSerial_FireCtrl->write (bufSend);
}

void ServiceSerialFireCtrl::ReceiveData()
{
    QByteArray dataRaw;

    while(m_pSerial_FireCtrl->bytesAvailable ()>0)
    {
        dataRaw.clear ();
        dataRaw.resize (m_pSerial_FireCtrl->bytesAvailable ());
        dataRaw = m_pSerial_FireCtrl->read (1024);
        m_parser_FireCtrl.ParseDownData (dataRaw);
    }
}










