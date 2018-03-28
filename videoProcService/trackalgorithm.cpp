#include "trackalgorithm.h"

#include "recti.h"
#include "cvfeatureextractmatch.h"
#include "kcftracker.h"
#include <QDebug>

bool HOG = true;
bool FIXEDWINDOW = true;
bool MULTISCALE = false;
bool SILENT = true;
bool LAB = false;

KCFTracker tracker(HOG, FIXEDWINDOW, MULTISCALE, LAB);


TrackAlgorithm::TrackAlgorithm()
{

}

TrackAlgorithm::~TrackAlgorithm ()
{

}


void TrackAlgorithm::MatchAndTrackInit (Rect rectIn,
                                     Mat frameIn,
                                     unsigned short feature[],
                                     int featureNum,
                                     int videoSrc)
{
    //imwrite ("curImg.jpg",frameIn);
#if FLAPPYDEBUG
    qDebug("rectIn is %d %d %d %d\n", rectIn.x, rectIn.y, rectIn.width, rectIn.height);
    qDebug()<<"feature number:"<<featureNum<<"\n"<<"videoSrc:"<<videoSrc<<"\n";


    for(int i=0; i<featureNum; i++)
    {
        qDebug()<<feature[i]<<"";
    }
    qDebug()<<"\n";
#endif
    //Mat mat;
    //frameIn.copyTo (mat);
    //rectangle (mat, rectIn, Scalar(255,0, 0, 0), 2, 8, 0);
    //imwrite ("srcImg.jpg",mat);


    cvFeatureExtractMatch T1;
    Rect matchRect(0,0,1,1);
    Mat tempMat;
    frameIn.copyTo (tempMat);



    T1.cvFeatureMatch(matchRect, feature, featureNum, tempMat, rectIn, videoSrc);
    //rectangle (tempMat, matchRect, Scalar(100,0, 0, 0), 1, 8, 0);



//    {
//        static Scalar colorTargetTag(250,250,250);
//        int x = matchRect.width/2;
//        int y = matchRect.height/2;

//        line(frameIn,Point(x-40,y-20) , Point(x-40,y-40) ,colorTargetTag , 2 ,8 ,0);
//        line(frameIn,Point(x-40,y-40) , Point(x-20,y-40) ,colorTargetTag , 2 ,8 ,0);

//        line(frameIn,Point(x+20,y-40) , Point(x+40,y-40) ,colorTargetTag , 2 ,8 ,0);
//        line(frameIn,Point(x+40,y-40) , Point(x+40,y-20) ,colorTargetTag , 2 ,8 ,0);

//        line(frameIn,Point(x+40,y+20) , Point(x+40,y+40) ,colorTargetTag , 2 ,8 ,0);
//        line(frameIn,Point(x+40,y+40) , Point(x+20,y+40) ,colorTargetTag , 2 ,8 ,0);

//        line(frameIn,Point(x-20,y+40) , Point(x-40,y+40) ,colorTargetTag , 2 ,8 ,0);
//        line(frameIn,Point(x-40,y+40) , Point(x-40,y+20) ,colorTargetTag , 2 ,8 ,0);
//    }

    //imwrite ("matchImg.jpg", tempMat);

    //qDebug()<<"match success\n";
    //qDebug("match rect is %d %d %d %d\n", matchRect.x, matchRect.y, matchRect.width, matchRect.height);

    if(matchRect.x<0 || matchRect.x>720 || matchRect.y >576 ||matchRect.y <0 || matchRect.width <7||
            (matchRect.x + matchRect.width) >720 || matchRect.height<7 || (matchRect.y + matchRect.height) >576)
        return;

    tracker.init(matchRect, frameIn);

}


Rect TrackAlgorithm::KeepTrack (Mat frameIn)
{
    Mat tempMat;
    frameIn.copyTo (tempMat);

    Rect result;
    result = tracker.update (tempMat);

    return result;
}


void TrackAlgorithm::ReadCalibrateFile (int videoSrc, u_int8_t focusZoom)
{
    if(videoSrc == 1)
    {
        Measure.Cameracalibrateread(
            "/home/nvidia/appsettings/algorithm/file/CalibrateInformationVisible.yml",focusZoom,720,576,videoSrc);

    }
    else if(videoSrc == 2)
    {
        Measure.Cameracalibrateread(
            "/home/nvidia/appsettings/algorithm/file/CalibrateInformationInfrared.yml",focusZoom,640,480,videoSrc);
    }

}


void TrackAlgorithm::Point2Angle (cv::Point pointIn, float &xangle, float &yangle, int videoSrc)
{
    Measure.CameracalibratePoint2Angle(pointIn, xangle, yangle,videoSrc);
}


void TrackAlgorithm::Angle2Point (float xangle, float yangle, Point &pointOut, int videoSrc)
{
    Measure.CameracalibrateAngle2Point(xangle, yangle, pointOut, videoSrc);
}


















