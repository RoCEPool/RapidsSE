/*============================================================================*/
/* Since.hpp - seconds.microseconds since January 1, 1970                     */
/*============================================================================*/

#pragma once

#include "System.h"

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
    ~Since();
    
    void  vAdd(i8 iMicroseconds);
    void  vGet(i8& iSeconds, i8& iMicroseconds);
    void  vSet(void);
    void  vSet(Time* pTime);
    void  vSet(i8 iSeconds, i8 iMicroseconds);
    
    i8    iSeconds(void);
    i8    iMicroseconds(void);
    
    void  vToTime(Time* pTime);
    
    //-------------------------------------------------------------------------
    
    static void vGetTimeOfDay(i8& iSeconds, i8& iMicroseconds, SysCall& SysCall);

    Footer* pFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
