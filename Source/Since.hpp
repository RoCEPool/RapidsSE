/*============================================================================*/
/* Since.hpp - seconds.microseconds since January 1, 1970                     */
/*============================================================================*/
#pragma once

namespace System
{
  class Since : public Object
  {
  public:
    i8 m_iSeconds;
    i8 m_iMicroseconds;
    
    // must be last
    Footer m_xFooter;
    
  public:
    Since();
    ~Since() noexcept(false);
    
    void  vAdd(i8 iMicroseconds);
    void  vGet(i8& iSeconds, i8& iMicroseconds);
    i8    iGetAsMicroseconds(void);
    void  vNow(void);
    void  vSet(Time* pTime);
    void  vSet(i8 iSeconds, i8 iMicroseconds);
    Ivl   wSubtract(Since& oArg);

    i8    iSeconds(void);
    i8    iMicroseconds(void);
    
    void  vToTime(Time* pTime);
    
    //-------------------------------------------------------------------------
    
    static void vGetTimeOfDay(i8& iSeconds, i8& iMicroseconds, SysCall& SysCall);

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
