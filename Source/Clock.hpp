/*============================================================================*/
/* Clock.cpp - walltime clock type (seconds.microseconds)                     */
/*============================================================================*/

#pragma once

#include "Common.h"

namespace Common
{
  class Time;
  
  class Clock
  {
  public:
    i8 m_iSeconds;
    i8 m_iMicro;
    
  public:
    void* operator new(size_t);

    Clock();
    ~Clock();
    
    void  vAdd(i8 iNanoSeconds);
    i8    iElapsed();
    void  vSet(Time* pTime);
    void  vToTime(Time* pTime);
    void  vUpdate();
    
    char* sToUTC(char* aWorkspace);
  };

}

/*****************************************************************************/
/* end of file                                                               */
/*****************************************************************************/