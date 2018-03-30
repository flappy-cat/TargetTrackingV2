#include "mainui.h"
#include "ui_mainui.h"
#include "globalsettings.h"
MainUI::MainUI(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MainUI)
{
    ui->setupUi(this);
    isWorking = false;
    btn_StartOrStop = ui->pushButton_StartOrStopWork;
    QObject::connect (btn_StartOrStop,SIGNAL(clicked(bool)),this,SLOT(startorstopWorking()));
    QObject::connect (&encThread,SIGNAL(finished()),SLOT(deleteLater()));
    QObject::connect (&udpThread,SIGNAL(Send_Data_signal(QByteArray)),pComManager,SLOT(SerialPayloadSend(QByteArray)));
}

MainUI::~MainUI()
{
    delete ui;
}

void MainUI::startorstopWorking()
{

    if(!isWorking)
    {
        pSettings = GlobalSettings::GetInstance ();
        pSettings->initSettings ();
        pComManager = ManagerOfCommunicaion::getInstance ();
        pComManager->InitCommunication ();
        pComManager->StartCommunication ();
        encThread.start ();
        btn_StartOrStop->setText ("Stop Working");
    }
    else
    {
        pComManager->StopCommunication ();
        pComManager->DeinitCommunication ();
        btn_StartOrStop->setText ("Start Working");
    }
    isWorking = !isWorking;

}
