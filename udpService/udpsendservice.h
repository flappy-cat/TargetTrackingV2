#ifndef UDPSENDSERVICE_H
#define UDPSENDSERVICE_H

#include <QObject>
#include <QUdpSocket>
#include <QTimer>
#include <QThread>

//该类负责发送udp的数据
class UdpSendService : public QObject
{
    Q_OBJECT
private:
    explicit UdpSendService(QObject *parent = 0);
    ~UdpSendService();

private:
    static UdpSendService* instance;

public:
    static UdpSendService* GetInstance();

public:
    QUdpSocket *udpSocket;
 //   void SendUdpGndData();

    //QTimer *timerUdpSend;
    //void onTimerUdpSendData();

};

#endif // UDPSENDSERVICE_H
