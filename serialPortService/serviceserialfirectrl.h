#ifndef SERVICESERIALFIRECTRL_H
#define SERVICESERIALFIRECTRL_H

#include <QObject>
#include "dataparser4firectrl.h"
#include <QTimer>
#include<QtSerialPort/QSerialPort>
class ServiceSerialFireCtrl : public QObject
{
    Q_OBJECT
public:
    explicit ServiceSerialFireCtrl(QObject *parent = 0);
    ~ServiceSerialFireCtrl();

signals:

public slots:

private:
    static ServiceSerialFireCtrl* instance;

public:
    static ServiceSerialFireCtrl* getInstance();

public:
    bool StartService();
    bool TerminateService();

public:
    QSerialPort* m_pSerial_FireCtrl;
    bool m_bIsServiceStart;
    DataParser4FireCtrl m_parser_FireCtrl;

public:
//    void SendData(const QByteArray& bufArray);
    QTimer *timerSend;
    void SendData();
private slots:
    void ReceiveData();

};

#endif // SERVICESERIALFIRECTRL_H
