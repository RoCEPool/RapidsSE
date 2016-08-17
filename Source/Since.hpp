/*============================================================================*/
/* Since.hpp - seconds.microseconds since January 1, 1970                     */
/*============================================================================*/

#pragma once

#include "Common.h"

namespace Common
{
  class Since
  {
  public:
    i8 m_iSeconds;
    i8 m_iMicroseconds;
    
  public:
    Since();
    ~Since();
    
    void  vGet(i8& iSeconds, i8& iMicroseconds);
    void  vSet(i8 iSeconds, i8 iMicroseconds);
    
    i8    iSeconds(void);
    i8    iMicroseconds(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
