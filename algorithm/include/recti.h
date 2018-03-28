#ifndef RECTI_H
#define RECTI_H

#include "recti_global.h"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include <iostream>
#include <stdio.h>
#include<omp.h>
using namespace std;
using namespace cv;

class RECTISHARED_EXPORT Recti
{
public:
    cv::Rect R;
    double f;
    Recti();
    ~Recti();
};

#endif // RECTI_H
