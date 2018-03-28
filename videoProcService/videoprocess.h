#ifndef VIDEOPROCESS_H
#define VIDEOPROCESS_H

#include <QThread>
#include <QMutex>
#include <QMutexLocker>
#include <videoencodeh264.h>



class VideoProcess : public QThread
{
    Q_OBJECT
public:
    VideoProcess(QObject *parent);
    ~VideoProcess();
    void run();

private:
    QMutex mutexVideoProc;

private slots:
    void StartVideoCapture();

public:
    VideoEncodeH264 encode;


};

#endif // VIDEOPROCESS_H
