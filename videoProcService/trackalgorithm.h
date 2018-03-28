#ifndef TRACKALGORITHM_H
#define TRACKALGORITHM_H

#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgproc/types_c.h"
#include "opencv2/core/core.hpp"
using namespace cv;

#include <calibrateinformationmeasure_global.h>
#include <calibrateinformationmeasure.h>


class TrackAlgorithm
{
public:
    TrackAlgorithm();
    ~TrackAlgorithm();

public:
    void MatchAndTrackInit(Rect rectIn, Mat frameIn, unsigned short feature[], int featureNum, int videoSrc);
    Rect KeepTrack(Mat frameIn);

    void ReadCalibrateFile(int videoSrc, u_int8_t focusZoom);
    void Point2Angle(cv::Point pointIn, float &xangle, float &yangle, int videoSrc);
    void Angle2Point(float xangle, float yangle, cv::Point &pointOut, int videoSrc);

public:
    Cameracalibrate Measure;

};

#endif // TRACKALGORITHM_H
