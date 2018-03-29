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
