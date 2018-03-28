#ifndef CVFEATUREEXTRACTMATCH_H
#define CVFEATUREEXTRACTMATCH_H

#include "cvfeatureextractmatch_global.h"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include "recti.h"
#include <iostream>
#include <stdio.h>
#include<omp.h>
using namespace std;
using namespace cv;

class CVFEATUREEXTRACTMATCHSHARED_EXPORT cvFeatureExtractMatch
{
public:

    //???????÷?á???????€????50????×é
    //??×é???·???€??featurenum=50,·?????·??á??????
    //?÷?????÷?á??????
    //????????1-featureValue[50],featureNum=50,VisibleoInfred=1
    //????????2-featureValue[15],featureNum=15,VisibleoInfred=2
    bool cvFeatureExtract(unsigned short int featureValue[], short int featureNum, cv::Mat featureExtrackImage, cv::Rect inputR,int VisibleoInfred);

    //图像特征匹配使用该函数，返回数值为匹配程度，越接近1匹配度越高
    //outputR为输出的矩形位置信息（图像坐标定义，左上角为（0,0），右下角为（720，576））
    //featureValue为从地面获得的特征数据，featureNum为特征数据的大小（可见光与红外不相同）
    //featureMatchImage为当前图像帧
    //inputR为从地面获得的矩形位置信息
    //VisibleoInfred为可见光红外选择开关，1为可见光，2为红外
    //可见光时输入的变量的初始化1-featureValue[50],featureNum=50,VisibleoInfred=1
    //红外时输入的变量的初始化2-featureValue[15],featureNum=15,VisibleoInfred=2
    //！！！匹配完成之后使用outputR，featureMatchImage（当前帧）初始化目标跟踪
    float cvFeatureMatch(cv::Rect &outputR, unsigned short int featureValue[], short int featureNum, cv::Mat featureMatchImage,cv::Rect inputR, int VisibleoInfred);

    //?ì??
    bool cvFeatureExtract_i(unsigned short int featureValue[], short int featureNum, cv::Mat featureExtrackImage, cv::Rect inputR);
    //??????
    bool cvFeatureExtract_v(unsigned short int featureValue[], short int featureNum, cv::Mat featureExtrackImage, cv::Rect inputR);
    //bool cvFeatureExtract_v2(unsigned short int featureValue[], short int featureNum, cv::Mat featureExtrackImage, cv::Rect inputR);
    //?ú?????÷???????????€????50????×é????????????????·?????????????????
    //float cvFeatureMatch(cv::Rect &outputR, cv::Mat featureExtrackImage, cv::Mat featureMatchImage, cv::Rect inputR);
    //?ì??
    //openmp
    float cvFeatureMatch_i0(cv::Rect &outputR, unsigned short int featureValue[], short int featureNum, cv::Mat featureMatchImage, cv::Rect inputR);
    //original
    float cvFeatureMatch_i1(cv::Rect &outputR, unsigned short int featureValue[], short int featureNum, cv::Mat featureMatchImage, cv::Rect inputR);
    //??????
    //openmp
    float cvFeatureMatch_v0(cv::Rect &outputR, unsigned short int featureValue[], short int featureNum, cv::Mat featureMatchImage, cv::Rect inputR);
    //original
    float cvFeatureMatch_v1(cv::Rect &outputR, unsigned short int featureValue[], short int featureNum, cv::Mat featureMatchImage, cv::Rect inputR);
    //integral  accelerate improved
    float cvFeatureMatch_v2(cv::Rect &outputR, unsigned short int featureValue[], short int featureNum, cv::Mat featureMatchImage, cv::Rect inputR);
    //float cvFeatureMatch_v4(cv::Rect &outputR, unsigned short int featureValue[], short int featureNum, cv::Mat featureMatchImage, cv::Rect inputR);// integral accelerate-improved with openmp

    cvFeatureExtractMatch();
    ~cvFeatureExtractMatch();
private:
    unsigned int maxrectcount;// = 6;
    int h_bins;// = 5;
    int s_bins;// = 10;
    int histSize[2];// = { h_bins, s_bins };
    // hue varies from 0 to 179, saturation from 0 to 255
    float h_ranges[2];// = { 0, 180 };
    float s_ranges[2];// = { 0, 256 };
    const float* ranges[2];// = { h_ranges, s_ranges };
    // Use the o-th and 1-st channels
    int channels[2];// = { 0, 1 };
    bool cvRectIn(cv::Rect rf, cv::Rect rs);
    //?????÷??????????rf??·???rs??
private:

    int gray_bins;//
    //int s_bins;// = 10;
    int gray_histSize[1];//
    // hue varies from 0 to 179, saturation from 0 to 255
    float gray_ranges[2];// = { 0, 255 };
    //float s_ranges[2];// = { 0, 255 };
    const float* g_ranges[1];// = { h_ranges,// s_ranges };
    // Use the o-th and 1-st channels
    int gray_channels[1];// = { 0,  };
};

#endif // CVFEATUREEXTRACTMATCH_H
