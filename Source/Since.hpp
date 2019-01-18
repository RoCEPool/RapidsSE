/*============================================================================*/
/* Since.hpp - seconds&microseconds since Posix Epoch January 1, 1970         */
/*============================================================================*/
#pragma once

namespace System
{
  class Time;
  
  class Since : public Object
  {
  public:
    i8  m_iSeconds;
    i8  m_iMicroseconds;
  
    s32 m_sFormatted;

    // must be last
    Footer m_xFooter;
    
  public:
    Since();
    ~Since() noexcept(false);
    
    static void vGetTimeOfDay(i8& iSeconds, i8& iMicroseconds, SysCall& SysCall);
  
    void  vAdd(i8 iMicroseconds);
    void  vGet(i8& iSeconds, i8& iMicroseconds);
    i8    iGetAsMicroseconds(void);
    Stz*  sFormatted(void);
    void  vNow(void);
    void  vSetUTC(Time* pTime);
    void  vSet(i8 iSeconds, i8 iMicroseconds);
    Ivl   wSubtract(Since& oArg);

    i8    iSeconds(void);
    i8    iMicroseconds(void);
    
    void  vToTime(Time* pTime);
    
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
