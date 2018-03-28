#include "videoprocess.h"
#include <QDebug>


VideoProcess::VideoProcess(QObject *parent) : QThread(parent)
{
    moveToThread (this);
}


VideoProcess::~VideoProcess ()
{
    quit();
    wait();
}


void VideoProcess::run ()
{
    StartVideoCapture ();
    exec();
}


void VideoProcess::StartVideoCapture ()
{
    encode.StartEncodeLoop ();
}












