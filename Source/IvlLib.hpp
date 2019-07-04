/*============================================================================*/
/* IvlLib.hpp -- Interval Library                                             */
/*============================================================================*/
#pragma once

namespace System
{
  class IvlLib
  {
  public:
    static Dtm wGetCurrIvl(void);
  
    static Dtm wToIvl(Dtm wTsp);
    static Dtm wToIvl(u8 uMrocoseconds);
    
    //! to be coded
  
    static Dtm  wAdd(Dtm wIvl1, Dtm wIvl2);
    static Dtm  wAdd(Dtm wIvl, u8 uMicro);
    static u8   uSubtract(Dtm wIvl1, Dtm wIvl2);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
