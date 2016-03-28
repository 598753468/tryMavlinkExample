#include"Bird.h"


//constructor and public methods

/////////////////////////////////
/// \brief Bird::Bird
///
Bird::Bird()
{

}

Bird::Bird(Posix_QextSerialPort *serialport)
{
    readingStatus=false;
    writingStatus=false;
    controlStatus=false;
    timeToExit=false;
    readThreadID=0;
    writeThreadID=0;
    serialPort=serialport;
}

void Bird::Start()
{
    if(!serialPort->isOpen())
    {
        std::cout<<"ERROR: serial port not open!"<<std::endl;
        //throw 1;
        return;
    }
    isComLocked=false;
    pthread_create(&readThreadID,NULL,&StartBirdReadThread,this);
}

void Bird::Stop()
{
    timeToExit=true;
    serialPort->close();
}

void Bird::SendCommand(Command1 command1,unsigned char command2,int16_t value)
{
    currentCommand.head=head;
    currentCommand.address=0x07;/////////////////
    currentCommand.command1=command1;
    currentCommand.command2=command2;
    currentCommand.value=value;
    currentCommand.check=CheckSum(currentCommand);
    WriteMessage(currentCommand);
}

bool Bird::IsFocalNew()
{
    return isFocalNew;
}

bool Bird::IsSelfCheckMessageNew()
{
    return isSelfCheckMessageNew;
}
bool Bird::IsPitchNew()
{
    return isPitchNew;
}

bool Bird::IsYawNew()
{
    return isYawNew;
}

bool Bird::IsWorkModeNew()
{
    return isWorkModeNew;
}

bool Bird::IsMultiDistanceMeasurement()
{
    return isMultiDistanceMeasurement;
}

bool Bird::IsDistanceNew()
{
    return isDistanceNew;
}

float Bird::Focal()
{
    isFocalNew=false;
    return focal;
}

float Bird::Pitch()
{
    isPitchNew=false;
    return pitch;
}

float Bird::Yaw()
{
    isYawNew=false;
    return yaw;
}

int16_t Bird::SelfCheckMessage()
{
    isSelfCheckMessageNew=false;
    return selfCheckMessage;
}

int16_t Bird::WorkModeMessage()
{
    isWorkModeNew=false;
    return workModeMessage;
}

int16_t Bird::DistanceMessage()
{
    isDistanceNew=false;
    return distanceMessage;
}

void Bird::WriteMessage(BirdMessage message)
{
    int size=sizeof(BirdMessage);
    char buffer[size];
    memcpy(buffer,&message,size);
    while(isComLocked)
    {
        usleep(100);
    }
    isComLocked=true;
    serialPort->write(buffer,sizeof(buffer));
    isComLocked=false;
    //serialPort->_write_port(buffer,&size);
}

unsigned char Bird::CheckSum(BirdMessage message)
{
    unsigned char bytes[1000];
    int messageSize=sizeof(BirdMessage);
    memcpy(bytes,&message,messageSize);
    unsigned char check=bytes[0];
    for(int i=1;i<messageSize-1;i++)
    {
        check=check^bytes[i];
    }
    return check;
}

unsigned char Bird::CheckSum(BirdMessageLarge messageLarge)
{
    unsigned char bytes[1000];
    int messageLargeSize=sizeof(BirdMessageLarge);
    memcpy(bytes,&messageLarge,messageLargeSize);
    unsigned char check=bytes[0];
    for(int i=1;i<messageLargeSize-1;i++)
    {
        check=check^bytes[i];
    }
    return check;
}

void Bird::start_read_thread()
{
    if(readingStatus)
    {
        std::cout<<"read thread already running!"<<std::endl;
    }
    else
    {
        read_thread();
        return;
    }
}

void Bird::read_thread()
{
    readingStatus=true;
    while(!timeToExit)
    {
        read_messages();
        usleep(20000);
    }
    readingStatus=false;
    return;
}

void Bird::ByteConventer(char* byte)
{
    char a=*byte;
    *byte=*(byte+1);
    *(byte+1)=a;
}

void Bird::read_messages()
{
    int ret=0;
    int messageSize=sizeof(BirdMessage);
    int messageLargeSize=sizeof(BirdMessageLarge);
    char bytesToRead[1024];
    char *readByte;
    BirdMessage message;
    BirdMessageLarge messageLarge;
    while(isComLocked)
    {
        usleep(100);
    }
    isComLocked=true;
    ret=serialPort->read(bytesToRead,128);
    isComLocked=false;
    readByte=bytesToRead;
    if(ret!=-1)
    {
        while(ret>=messageSize)
        {
            if(((unsigned char)*readByte)==head)
            {
                switch(*(readByte+1))
                {
                case 7:
                    memcpy(&message,readByte,messageSize);
                    if(message.check==CheckSum(message))
                    {
                        switch(message.command1)
                        {
                        case VisibleLightCamera:
                            if(message.command2==GetFocal)
                            {
                                focal=message.value/100.0;
                                isFocalNew=true;
                            }
                            break;
                        case SelfCheck:
                            if(message.command2==SelfCheckReturn)
                            {
                                selfCheckMessage=message.value;
                                isSelfCheckMessageNew=true;
                            }
                            break;
                        case BirdRotate:
                            switch(message.command2)
                            {
                            case PitchReturn:
                                pitch=message.value/100.0;
                                isPitchNew=true;
                                break;
                            case YawReturn:
                                yaw=message.value/100.0;
                                isYawNew=true;
                                break;
                            }
                            break;
                        case SystemWorkMode:
                            if(message.command2==SystemWorkModeReturn)
                            {
                                workModeMessage=message.value;
                                isWorkModeNew=true;
                            }
                            break;
                        case DistanceMeasurement:
                            switch(message.command2)
                            {
                            //char dist[2];
                            //char turn;
                            case SingleReturn:
                                isMultiDistanceMeasurement=false;
                                //GetDistance()
                                //memcpy(dist,message.value,2);
                                distanceMessage=message.value;
                                isDistanceNew=true;
                                break;
                            case MultiReturn:
                                isMultiDistanceMeasurement=true;
                                distanceMessage=message.value;
                                isDistanceNew=true;
                                break;
                            }
                            break;
                        }
                    }
                    if(ret>messageSize)
                    {
                        readByte=readByte+messageSize;
                    }
                    ret=ret-messageSize;
                    break;
                case 11:
                    if(ret>=messageLargeSize)
                    {
                        //双字节数据低位在后，高位在前
                        ByteConventer(readByte+2);
                        ByteConventer(readByte+4);
                        ByteConventer(readByte+6);
                        ByteConventer(readByte+8);
                        memcpy(&messageLarge,readByte,messageLargeSize);
                        if(messageLarge.check==CheckSum(messageLarge))
                        {
                            pitch=messageLarge.pitch/100.0;
                            yaw=messageLarge.yaw/100.0;
                            distanceMessage=messageLarge.distance;
                            focal=messageLarge.focus/10.0;
                            isPitchNew=true;
                            isYawNew=true;
                            isDistanceNew=true;
                            isFocalNew=true;
                        }
                    }
                    if(ret>messageLargeSize)
                    {
                        readByte=readByte+messageLargeSize;
                    }
                    ret=ret-messageLargeSize;
                    break;
                }

            }
            else
            {
                readByte=readByte+1;
                ret=ret-1;
            }
        }
    }

}

// ------------------------------------------------------------------------------
//  Pthread Starter Helper Functions
// ------------------------------------------------------------------------------

void*StartBirdReadThread(void *args)
{
    // takes an autopilot object argument
    Bird *bird = (Bird *)args;

    // run the object's read thread
    bird->start_read_thread();

    // done!
    return NULL;
}
