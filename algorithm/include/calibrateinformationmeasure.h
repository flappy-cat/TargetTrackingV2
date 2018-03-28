#ifndef CALIBRATEINFORMATIONMEASURE_H
#define CALIBRATEINFORMATIONMEASURE_H

#include "calibrateinformationmeasure_global.h"

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <fstream>
#include <math.h>

#include <time.h>

using namespace cv;
using namespace std;

class CALIBRATEINFORMATIONMEASURESHARED_EXPORT Cameracalibrate
{
public:

    //
    int Cameracalibrateread(char ymlReadPath[], int focalLength, int imageWidth, int imageHeight, int visibleInfrared);

    int CameracalibratePoint2Angle(cv::Point inputPoint, float &xangle, float &yangle, int visibleInfrared);
    int CameracalibrateAngle2Point(float xangle, float yangle, cv::Point &inputPoint, int visibleInfrared);

    Cameracalibrate();
    ~Cameracalibrate();


private:
    int Cameracalibratewrite(char ymlWritePath[]);
    cv::Mat pointsIn;
    cv::Mat pointsOut;
    cv::Mat angleMat;

    cv::Mat cameraMatrixRead;
    cv::Mat distCoeffsRead;
    cv::Mat cameraMatrixWrite;
    cv::Mat distCoeffsWrite;
    float halfViewAngleX;
    float halfViewAngleY;
    int imageXpixel;
    int imageYpixel;



};
#endif // CALIBRATEINFORMATIONMEASURE_H
