#ifndef CameraRead_H_
#define CameraRead_H_

#include<opencv2/opencv.hpp>
#include"Marker.hpp"
#include"MarkerDetector.hpp"
#include"BGRAVideoFrame.h"
#include<time.h>
#include"djicam.h"
#include<unistd.h>
#include"CTimer.h"
//static class CameraRead;
//typedef void (*CameraReadPTR)(int);
struct FoundMarker
{
    int id;
    double x;
    double y;
    double z;
    double radX;
    double radY;
    double radZ;
    double degreeX;
    double degreeY;
    double degreeZ;
public:
    FoundMarker()
    {
        id=0;
        x=0;
        y=0;
        z=0;
        radX=0;
        radY=0;
        radZ=0;
        degreeX=0;
        degreeY=0;
        degreeZ=0;
    }
};

class CameraRead
{
private:
    static cv::VideoCapture cap;
    static double pi;
    static float fc1;
    static float fc2;
    static float cc1;
    static float cc2;
    static float kc1;
    static float kc2;
    static float kc3;
    static float kc4;
    static float distorsionCoeff[4];
    static CameraCalibration calibration;
    static MarkerDetector markerDetector;
    static int ImageWidth,ImageHeight,ChangeMarkerSizeCount;
    //static cv::Mat input,inputCut,camera;
    static BGRAVideoFrame frame;
    static std::vector<Transformation> trans;
    static std::vector<Transformation>::iterator tr;
    static cv::Mat_<float> srcMat,dstMat;
    static unsigned int FrameCount;
    static unsigned char buffer[2764800];
    static CTimer timer;
    //static double positionX,positionY,positionZ,radX,radY,radZ,degreeX,degreeY,degreeZ,
    static double xPoint,yPoint;
    static bool isInited,isImageGot,isMarkerCatched,isCameraBusy,isWindowBusy,ChangeToLittleMarker,isLargeMarkerValid,isSmallMarkerValid;
    static double currentTime,lastTime;

    static void OnTime_Handle(int);
    static FoundMarker smallMarker,largeMarker;

public:
    static FoundMarker SmallMarker();
    static FoundMarker LargeMarker();
    static bool IsLargeMarkerValid();
    static bool IsSmallMarkerValid();
    static cv::Mat input,inputCut,camera;
    static void SetMarkerSize(double LargeMarkerSize,double LittleMarkerSize);
    static bool Init();
    static bool stop();
//    static double PositionX();
//    static double PositionY();
//    static double PositionZ();
//    static double RadX();
//    static double RadY();
//    static double RadZ();
//    static double DegreeX();
//    static double DegreeY();
//    static double DegreeZ();
    static double XPoint();
    static double YPoint();
    static bool IsInited();
    static bool IsImageGot();
    static bool IsMarkerCatched();
};


#endif
