#ifndef SERVICESERIALPAYLOAD_H
#define SERVICESERIALPAYLOAD_H

#include <QObject>
#include "dataparser4payload.h"
#include <QTimer>
#include<QtSerialPort/QSerialPort>


class ServiceSerialPayload : public QObject
{
    Q_OBJECT
public:
    explicit ServiceSerialPayload(QObject *parent = 0);
    ~ServiceSerialPayload();

signals:

public slots:

private:
    static ServiceSerialPayload* instance;
public:
    static ServiceSerialPayload* getInstance();

public:
    bool StartService();
    bool TerminateService();

public:
    QSerialPort*  m_pSerial_Payload;
    bool m_bIsServiceStart;
    DataParser4Payload m_parser_Payload;

public:
    QTimer *timerSend;

    void SendData();
public slots:
    void ReceiveData();
    void Send_Data(QByteArray bufArray);
};

#endif // SERVICESERIALPAYLOAD_H



