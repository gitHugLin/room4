//
// Created by linqi on 16-4-5.
//

#ifndef MY_JNI_GETHOMOGRAPHY_H
#define MY_JNI_GETHOMOGRAPHY_H



#include "OrbPatch.h"
#include "iostream"
#include "vector"
#include "string"
#include "opencv2/opencv.hpp"
#include "log.h"

using namespace cv;
using namespace std;


class GetHomography
{
public:
    GetHomography(Mat& srcImage,Mat& dstImage);
    ~GetHomography();

public:
    //reprojectionThreshold = 4
    //RHO ,RANSAC 和 LMEDS
    Mat getHomography(double reprojectionThreshold = 10);
    bool setMode(int homoMethod = RANSAC);
private:
    //_patchSize = 440,const int keyPointThread = 20  //stable1
    bool detectPoints(const int _patchSize = 400,const int keyPointThread = 15
            ,const int robEdgeThreshold = 21 );
    void savePoints(vector<Point2f>& _queryPoints,vector<Point2f>& _trainPoints);

private:
    void workBegin();
    void workEnd();

    OrbPatch m_patch;
    Mat queryPattern;
    Mat trainPattern;
    int HomoMethod;
//单映射矩阵
    Mat m_Homography;

private:
    pthread_mutex_t g_mutex;
    int scaleSize;
    int64 work_begin;
    int64 work_end;
    vector<Point2f> m_queryPoints;
    vector<Point2f> m_trainPoints;

};





#endif //MY_JNI_GETHOMOGRAPHY_H
