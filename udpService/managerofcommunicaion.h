#ifndef MANAGEROFCOMMUNICAION_H
#define MANAGEROFCOMMUNICAION_H

#include <QObject>
#include "serviceudpvideo.h"
#include <QThread>


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


};

#endif // MANAGEROFCOMMUNICAION_H
