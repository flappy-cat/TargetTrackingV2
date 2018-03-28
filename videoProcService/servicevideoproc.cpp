#include "servicevideoproc.h"


ServiceVideoProc::ServiceVideoProc(QObject *parent) : QObject(parent)
{
    pVideoProcThread = NULL;
}


ServiceVideoProc::~ServiceVideoProc ()
{
    if(!instance)
    {
        delete instance;
        instance = NULL;
    }
}

ServiceVideoProc* ServiceVideoProc::instance = NULL;

ServiceVideoProc* ServiceVideoProc::getInstance ()
{
    if(NULL == instance)
    {
        instance = new ServiceVideoProc();
    }
    return instance;
}


void ServiceVideoProc::StartService ()
{
    pVideoProcThread = new VideoProcess(0);
    connect (pVideoProcThread, SIGNAL(finished()), pVideoProcThread, SLOT(deleteLater()));
    pVideoProcThread->start();
}


void ServiceVideoProc::TerminateService ()
{
//    if(pVideoProcThread != NULL)
//    {
////        pVideoProcThread->quit ();
////        pVideoProcThread->wait ();

//        delete pVideoProcThread;
//        pVideoProcThread = NULL;
//    }
}















