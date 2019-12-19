/*============================================================================*/
/* IvlLib.hpp -- Interval Library                                             */
/*============================================================================*/
#pragma once

namespace Library
{
  class IvlLib
  {
  public:
    static Dtm wGetCurrIvl(void);
  
    static Dtm wToIvl(Dtm wTsp);
    static Ivl wToIvl(u8 uMicro);
    
    //! to be coded
  
    static Dtm  wAdd(Ivl wIvl1, Ivl wIvl2);
    static Dtm  wAdd(Ivl wIvl, u8 uMicro);
    static u8   uSubtract(Ivl wIvl1, Ivl wIvl2);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
