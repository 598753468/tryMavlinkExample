#include"CameraRead.h"

using namespace cv;
using namespace std;
//////////////init////////////////////
cv::VideoCapture CameraRead::cap;
double CameraRead::pi=3.14159265358979326226;
float CameraRead::fc1 = 2328.94795134537*6/25.0;
float CameraRead::fc2 = 2328.07882489417*6/25.0;
float CameraRead::cc1 = 1973.77902508872*6/25.0;
float CameraRead::cc2 = 1460.77504859926*6/25.0;
float CameraRead::kc1 = -0.123631906445824;
float CameraRead::kc2 = 0.0841239992939220;
float CameraRead::kc3 = 0;
float CameraRead::kc4 = 0;
int CameraRead::ImageWidth=1280,CameraRead::ImageHeight=720,CameraRead::ChangeMarkerSizeCount=0;
float CameraRead::distorsionCoeff[4]={CameraRead::kc1,CameraRead::kc2,CameraRead::kc3,CameraRead::kc4};
bool CameraRead::isInited,
CameraRead::isImageGot,
CameraRead::isMarkerCatched,
CameraRead::isCameraBusy,
CameraRead::isWindowBusy,
CameraRead::ChangeToLittleMarker,
CameraRead::isLargeMarkerValid,
CameraRead::isSmallMarkerValid;
double
//CameraRead::positionX,
//CameraRead::positionY,
//CameraRead::positionZ,
//CameraRead::radX,
//CameraRead::radY,
//CameraRead::radZ,
//CameraRead::degreeX,
//CameraRead::degreeY,
//CameraRead::degreeZ,
CameraRead::xPoint,
CameraRead::yPoint;
CameraCalibration CameraRead::calibration(fc1,fc2,cc1,cc2,distorsionCoeff);
MarkerDetector CameraRead::markerDetector(calibration);
cv::Mat_<float> CameraRead::srcMat=cv::Mat_<float>(3,3);
cv::Mat_<float> CameraRead::dstMat=cv::Mat_<float>(1,3);
unsigned char CameraRead::buffer[2764800]={0};
CTimer CameraRead::timer=CTimer(0,30);
unsigned int CameraRead::FrameCount=0;
cv::Mat CameraRead::input,CameraRead::inputCut,CameraRead::camera;
BGRAVideoFrame CameraRead::frame;
std::vector<Transformation> CameraRead::trans;
std::vector<Transformation>::iterator CameraRead::tr;
double CameraRead::currentTime,CameraRead::lastTime;
FoundMarker CameraRead::smallMarker,
CameraRead::largeMarker;
/////////////public///////////////////

void CameraRead::SetMarkerSize(double LargeMarkerSize, double LittleMarkerSize)
{
    markerDetector.ChangeMarkerSize(LargeMarkerSize,LittleMarkerSize);
}



bool CameraRead::Init()
{
    cap.open(1);
//    cap.set(CV_CAP_PROP_FRAME_WIDTH,1280);
//    cap.set(CV_CAP_PROP_FRAME_HEIGHT,720);
    if(!cap.isOpened())
    {
        cout << "无法打开摄像头"<<endl;
        return false;
    }
    else
    {
        timer.StartTimer(&CameraRead::OnTime_Handle);
        isInited=true;
        return true;
    }
    //int ret=manifold_cam_init(GETBUFFER_MODE|TRANSFER_MODE);
//    if(ret==0)
//    {
//        timer.StartTimer(&CameraRead::OnTime_Handle);
//        isInited=true;
//        //cvNamedWindow("tt");
//        return true;
//    }
//    else
//    {
//        isInited=false;
//        return false;
//    }
}

bool CameraRead::stop()
{

    //timer.StopTimer();
//    while (!manifold_cam_exit())
//    {
//        sleep(1);
//    }
//    isInited=false;
    //destroyWindow("tt");
    return true;
}

FoundMarker CameraRead::SmallMarker()
{
    isSmallMarkerValid=false;
    return smallMarker;
}

FoundMarker CameraRead::LargeMarker()
{
    isLargeMarkerValid=false;
    return largeMarker;
}


//double CameraRead::PositionX()
//{
//    return  positionX;
//}

//double CameraRead::PositionY()
//{
//    return positionY;
//}

//double CameraRead::PositionZ()
//{
//    return positionZ;
//}

//double CameraRead::RadX()
//{
//    return radX;
//}

//double CameraRead::RadY()
//{
//    return radY;
//}

//double CameraRead::RadZ()
//{
//    return radZ;
//}

//double CameraRead::DegreeX()
//{
//    return degreeX;
//}

//double CameraRead::DegreeY()
//{
//    return degreeY;
//}

//double CameraRead::DegreeZ()
//{
//    return degreeZ;
//}

double CameraRead::XPoint()
{
    return xPoint;
}

double CameraRead::YPoint()
{
    return yPoint;
}

bool CameraRead::IsInited()
{
    return isInited;
}

bool CameraRead::IsImageGot()
{
    return isImageGot;
}

bool CameraRead::IsMarkerCatched()
{
    return isMarkerCatched;
}


bool CameraRead::IsLargeMarkerValid()
{
    return isLargeMarkerValid;
}

bool CameraRead::IsSmallMarkerValid()
{
    return isSmallMarkerValid;
}




///////////private///////////////////
void CameraRead::OnTime_Handle(int)
{
    if(!isCameraBusy)
    {
        isCameraBusy=true;
        cap>>input;
        //int ret=manifold_cam_read(buffer,&FrameCount,1);
        isCameraBusy=false;
        if(true)
        {
            isImageGot=true;
            //camera=Mat(ImageHeight+ImageHeight/2,ImageWidth,CV_8UC1,buffer);
            //cvtColor(camera,input,CV_YUV2BGR_NV12);
            //inputCut=input(Range(0,720),Range(160,1120));
            //input=inputCut;
            //dst=IplImage(input);
            //inputCut=cvCloneImage(&dst);
            //src=IplImage(inputCut);
            //cvUndistort2(&src,&dst,&intrinsic_matrix, &distortion_coeffs);
            //cvReleaseImage(src);
            //destroyWindow("tt");
            //namedWindow("tt");
            //imshow("tt",input);
            frame.height=input.rows;
            frame.width=input.cols;
            frame.stride=input.step;
            frame.data=input.data;

            markerDetector.processFrame(frame);
            trans=markerDetector.getTransformations();
            int count=trans.size();
            currentTime=clock();
            if(count>0)
            {
                isMarkerCatched=true;
            }
            else
            {
                isMarkerCatched=false;
            }
//            if((count==1)&&ChangeToLittleMarker)
//            {
//                markerDetector.ChangeMarkerSize(littleMarkerSize);
//            }
//            else
//            {
//                markerDetector.ChangeMarkerSize(largeMarkerSize);
//            }
//            if(count==2)
//            {
//                ChangeMarkerSizeCount++;
//            }
//            else
//            {
//                ChangeMarkerSizeCount=0;
//            }

//            if(ChangeMarkerSizeCount>5)
//            {
//                ChangeToLittleMarker=true;
//            }

            //cout<<"find "<<count<<"\t"<<currentTime-lastTime<<"\t"<<input.cols<<"\t"<<input.rows<<"\t"<<""<<"\t"<<""<<endl;
            for(tr=trans.begin();tr!=trans.end();tr++)
            {
                for (int col=0; col<3; col++)
                {
                    for (int row=0; row<3; row++)
                    {
                        srcMat(row,col)=tr.base()->r().mat[row][col];
                    }
                }
                Rodrigues(srcMat,dstMat);
                if(tr.base()->id()==1)
                {
                    largeMarker.radX=dstMat(0,0);
                    largeMarker.radY=dstMat(0,1);
                    largeMarker.radZ=dstMat(0,2);
                    largeMarker.degreeX=largeMarker.radX*180/pi;
                    largeMarker.degreeY=largeMarker.radY*180/pi;
                    largeMarker.degreeZ=largeMarker.radZ*180/pi;
                    largeMarker.x=tr.base()->t().data[0];
                    largeMarker.y=tr.base()->t().data[1];
                    largeMarker.z=tr.base()->t().data[2];
                    isSmallMarkerValid=true;
                }
                else if(tr.base()->id()==2)
                {
                    smallMarker.radX=dstMat(0,0);
                    smallMarker.radY=dstMat(0,1);
                    smallMarker.radZ=dstMat(0,2);
                    smallMarker.degreeX=smallMarker.radX*180/pi;
                    smallMarker.degreeY=smallMarker.radY*180/pi;
                    smallMarker.degreeZ=smallMarker.radZ*180/pi;
                    smallMarker.x=tr.base()->t().data[0];
                    smallMarker.y=tr.base()->t().data[1];
                    smallMarker.z=tr.base()->t().data[2];
                    isLargeMarkerValid=true;
                }

                xPoint=markerDetector.xPoints;
                yPoint=markerDetector.yPoints;
                //cout<<dstMat(0,0)*180/pi<<"\t"<<dstMat(0,1)*180/pi<<"\t"<<dstMat(0,2)*180/pi<<endl;
                //cout<<tr.base()->t().data[0]<<"\t"<<tr.base()->t().data[1]<<"\t"<<tr.base()->t().data[2]<<endl;
            }
            lastTime=currentTime;
        }
        else
        {
            isImageGot=false;
        }
    }

}
