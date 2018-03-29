#include "mainui.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    VideoEncodeH264     encThread;
    ServiceUDPVideo     udpThread;

    QObject::connect (&encThread,SIGNAL(finished()),&encThread,SLOT(deleteLater()));
    QObject::connect (&udpThread,SIGNAL(Send_Data_signal(QByteArray)),
                  pSerialServicePayload,SLOT(Send_Data(QByteArray)));

    encThread.start ();

    MainUI w;
    w.show();

    return a.exec();
}
