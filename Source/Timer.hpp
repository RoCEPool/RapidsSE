/*============================================================================*/
/* Timer.hpp -- timer                                                         */
/*============================================================================*/
#pragma once

#include "Common.h"

namespace Common
{
  class Timer : public mn,
                public cn
  {
  public:
  
  private:
    i8      m_iStart;
    i8      m_iUpdate;
    
  public:
    void* operator new(size_t, void* pTimerAddress);
    void operator delete(void* pTimerAddress);
    Timer(void);
    ~Timer();
    
    void vInit(void);
    void vReset(void);
    void vBegin(void);
    void vUpdate(void);
    
    i8   iNanoseconds(void);
    i8   iMicroseconds(void);
    i8   iMilliseconds(void);
    i8   iSeconds(void);
    i8   iTicks(void);
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/