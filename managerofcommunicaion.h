#ifndef MANAGEROFCOMMUNICAION_H
#define MANAGEROFCOMMUNICAION_H

#include <QObject>
#include <QThread>
#include "serialPortService/serviceserialfirectrl.h"
#include "serialPortService/serviceserialpayload.h"


class ManagerOfCommunicaion : public QObject
{
    Q_OBJECT
public:
    explicit ManagerOfCommunicaion(QObject *parent = 0);
    ~ManagerOfCommunicaion();
private:
    static ManagerOfCommunicaion* instance;
public:
    static ManagerOfCommunicaion* getInstance();
public:
    void InitCommunication();
    void StartCommunication();

    void StopCommunication();
    void DeinitCommunication();
signals:

public slots:

private:
    ServiceSerialPayload*   pSerialServicePayload;
    ServiceSerialFireCtrl*  pSerialServiceFireCtrl;


};

#endif // MANAGEROFCOMMUNICAION_H
