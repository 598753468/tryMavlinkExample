#ifndef     _BIRD_H___
#define     _BIRD_H___
//#include"c_uart_interface_example/serial_port.h"
#include"QSerialPort/posix_qextserialport.h"
#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include<iostream>
#pragma pack(push)
#pragma pack(1)
struct BirdMessage
{
    unsigned char head;
    unsigned char address;
    unsigned char command1;
    unsigned char command2;
    int16_t value;
    unsigned char check;
};

struct BirdMessageLarge
{
    unsigned char head;
    unsigned char address;
    int16_t yaw;
    int16_t pitch;
    int16_t distance;
    int16_t focus;
    unsigned char check;
};

#pragma pack(pop)
enum MessageEnum
{
    head=0xAA,
};

enum Command1
{
    BirdRotate=0x01,
    InertialSpace=0x02,
    SelfCheck=0x03,
    VisibleLightCamera=0x04,
    SystemWorkMode=0x05,
    TrackingBox=0x06,
    DistanceMeasurement=0x07,
    SystemStatus=0x08,
};

enum birdRotate
{
    Pitch=0x02,
    Yaw=0x03,
    PitchReturn=0xFD,
    YawReturn=0xFE,
};

enum inertialSpace
{
    YawAxis=0x01,
    PitchAxis=0x02,
};

enum selfCheck
{
    SelfcheckCommand=0x01,
    SelfCheckReturn=0xFE,
};

enum visibleLightCamera
{
    FocalAdd=0x20,
    FocalSubtract=0x40,
    GetFocal=0xFD
};

enum systemWorkMode
{
    WorkMode=0x01
};

enum trackingBox
{
    MoveTrackX=0x03,
    MoveTrackY=0x04,
    FixTrackX=0x05,
    FixTrackY=0x06,
    SetSensitivity=0x07
};

enum distanceMeasurement
{
    Single=0x01,
    Multi=0x02,
    SingleReturn=0xFE,
    MultiReturn=0xFD
};

enum systemStatus
{
    Stop=0x01,
    Start=0x02,
};

enum workMode
{
    Manual=1,
    Tracking =2,
    Landing=3,
    SystemWorkModeReturn=0xFE,
};


//helper functions
void*StartBirdReadThread(void *args);

class Bird
{
public:
    bool readingStatus;
    bool writingStatus;
    bool controlStatus;

    BirdMessage currentCommand,currentMessage;

    Bird();
    Bird(Posix_QextSerialPort *serialPort);
    void Start();
    void Stop();
    void SendCommand(Command1 command1,unsigned char command2,int16_t value);
    void start_read_thread();
    void read_messages();
//        ///
//        void SetAngularSpeed(short int pitchSpeed,short int yawSpeed);
//        ///
//        void SetRollAxisAngle(float angle);
//        ///
//        void SelfChecking();
//        ///
//        void FocalAdd();
//        ///
//        void FocalSubtract();
//        ///
//        void GetFocal();
//        ///
//        void SetWorkMode(WorkMode mode);
    bool IsFocalNew();
    bool IsSelfCheckMessageNew();
    bool IsPitchNew();
    bool IsYawNew();
    bool IsWorkModeNew();
    bool IsMultiDistanceMeasurement();
    bool IsDistanceNew();
    float Focal();
    float Pitch();
    float Yaw();
    int16_t SelfCheckMessage();
    int16_t WorkModeMessage();
    int16_t DistanceMessage();
private:
    //Serial_Port *serialPort;
    bool isComLocked;
    void ByteConventer(char* byte);
    float focal,pitch,yaw;
    int16_t selfCheckMessage,workModeMessage,distanceMessage;
    bool isFocalNew,
    isSelfCheckMessageNew,
    isPitchNew,
    isYawNew,
    isWorkModeNew,
    isMultiDistanceMeasurement,
    isDistanceNew;
    Posix_QextSerialPort *serialPort;
    bool timeToExit;
    pthread_t readThreadID;
    pthread_t writeThreadID;
    void WriteMessage(BirdMessage message);
    unsigned char CheckSum(BirdMessage message);
    unsigned char CheckSum(BirdMessageLarge messageLarge);
    void read_thread();
    //int16_t GetDistance(int16_t source);
};


#endif
