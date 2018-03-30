#include "managerofcommunicaion.h"

ManagerOfCommunicaion::ManagerOfCommunicaion(QObject *parent) : QObject(parent)
{
    connect (this,SIGNAL(Send_Data_signal(QByteArray)),pSerialServicePayload,SLOT(Send_Data(QByteArray)));
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

void ManagerOfCommunicaion::InitCommunication()
{

}

void ManagerOfCommunicaion::StartCommunication()
{
    pSerialServicePayload   =      ServiceSerialPayload::getInstance ();
    pSerialServiceFireCtrl  =      ServiceSerialFireCtrl::getInstance ();

    pSerialServiceFireCtrl->StartService ();
    pSerialServicePayload->StartService ();

}

void ManagerOfCommunicaion::StopCommunication()
{

}

void ManagerOfCommunicaion::DeinitCommunication()
{

}

void ManagerOfCommunicaion::SerialPayloadSend(QByteArray bufArray)
{
    emit Send_Data_signal (bufArray);
}
