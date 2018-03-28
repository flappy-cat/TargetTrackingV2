#include "udpsendservice.h"
#include <QHostAddress>


UdpSendService::UdpSendService(QObject *parent) : QObject(parent)
{
    udpSocket = new QUdpSocket(this);

    //timerUdpSend = new QTimer(this);
    //connect(timerUdpSend, &QTimer::timeout, this, &UdpSendService::onTimerUdpSendData);
//    timerUdpSend->start(500);
}


UdpSendService::~UdpSendService()
{
    if(instance != NULL)
    {
        delete instance;
        instance = NULL;
    }
}

UdpSendService* UdpSendService::instance = NULL;

UdpSendService* UdpSendService::GetInstance()
{
    if(NULL == instance)
    {
        instance = new UdpSendService();
    }

    return instance;
}

//void UdpSendService::SendUdpGndData()
//{
//    DataManager *pManager = DataManager::GetInstance();
//    GlobalParam *pParam = GlobalParam::GetInstance();

//    udpSocket->writeDatagram((char*)pManager->gnd_Send_Buf,
//               GND_SEND_BUF_SIZE, pParam->udpServerIP, pParam->udpLocalPort);
//}


//void UdpSendService::onTimerUdpSendData()
//{

//}













