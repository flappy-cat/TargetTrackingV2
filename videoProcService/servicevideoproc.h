#ifndef SERVICEVIDEOPROC_H
#define SERVICEVIDEOPROC_H

#include <QObject>
#include "videoprocess.h"

class ServiceVideoProc : public QObject
{
    Q_OBJECT
public:
    explicit ServiceVideoProc(QObject *parent = 0);
    ~ServiceVideoProc();

private:
    static ServiceVideoProc* instance;

public:
    static ServiceVideoProc* getInstance();
    void StartService();
    void TerminateService();

signals:

public slots:

public:
    VideoProcess* pVideoProcThread;
};

#endif // SERVICEVIDEOPROC_H
