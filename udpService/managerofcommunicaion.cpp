#include "managerofcommunicaion.h"

ManagerOfCommunicaion::ManagerOfCommunicaion(QObject *parent) : QObject(parent)
{

}

ManagerOfCommunicaion::~ManagerOfCommunicaion ()
{

}

ManagerOfCommunicaion* ManagerOfCommunicaion::instance = NULL;
ManagerOfCommunicaion* ManagerOfCommunicaion::getInstance ()
{
    if(NULL == instance)
    {
        instance = new ManagerOfCommunicaion();
    }
    return instance;
}

void ManagerOfCommunicaion::StartCommunication()
{
    pUDPSingleService = new ServiceUDPVideo(0);
    pUdpSingleServiceThread = new QThread(0);
    pUDPSingleService->moveToThread(pUdpSingleServiceThread);
    connect(pUdpSingleServiceThread,SIGNAL(finished()),pUDPSingleService,SLOT(deleteLater()));
    connect(pUdpSingleServiceThread,SIGNAL(finished()),pUdpSingleServiceThread,SLOT(deleteLater()));


    ServiceSerialPayload* pSerialServicePayload = ServiceSerialPayload::getInstance ();



    QObject::connect(pUDPSingleService,SIGNAL(Send_Data_signal(QByteArray)),
                      pSerialServicePayload , SLOT(Send_Data(QByteArray)));

    pUdpSingleServiceThread->start();
}
