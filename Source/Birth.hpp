/*============================================================================*/
/* Birth.hpp - birth timestamp                                                */
/*============================================================================*/
#pragma once

namespace System
{
  class Time;
  
  class Birth : public Object
  {
  public:
    Dtm m_wTsp; // birthh as tsp
    Dtm m_wIvl; // birth as Ivl since epoch
  
    // must be last
    Footer m_xFooter;
    
  public:
    Birth();
    ~Birth() noexcept(false);
    
    static void vGetTimeOfDay(i8& iSeconds, i8& iMicroseconds, SysCall& SysCall);
    
    void vNow(void);
    u8   uSecondsElapsed(void);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
