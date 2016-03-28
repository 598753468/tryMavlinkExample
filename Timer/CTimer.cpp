/* 
* CTimer.cpp 
* 
* Created on: 2009-7-13 
*      Author: DEAN 
*/  

#include "CTimer.h"  


using namespace std;  
//////////////////////////public methods//////////////////////////  
CTimer::CTimer()
{  
    m_second=0;
    usecond=0;
    Callback=0;
}  

CTimer::CTimer(long second, long microsecond)
{  
    m_second=second;
    usecond=microsecond*1000;
    Callback=0;
}  

CTimer::~CTimer()  
{  
}  

void CTimer::SetTimer(long second, long microsecond)  
{  
    m_second = second;
    usecond = microsecond*1000;
}  

void CTimer::StartTimer(CTimerCallBack callback)
{  
    Callback=callback;
    pthread_create(&thread_timer, NULL, OnTimer_stub, this);
}  


void CTimer::StopTimer()  
{  
    pthread_cancel(thread_timer);
    pthread_join(thread_timer, NULL); //wait the thread stopped
}  

//////////////////////////private methods//////////////////////////  
void CTimer::thread_proc()  
{  
    while (true)
    {
        Callback(1);

        pthread_testcancel();
        struct timeval tempval;
        tempval.tv_sec = m_second;
        tempval.tv_usec = usecond;
        select(0, NULL, NULL, NULL, &tempval);
    }
}  
