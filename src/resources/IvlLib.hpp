/*============================================================================*/
/* IvlLib.hpp -- Interval Library                                             */
/*============================================================================*/
#pragma once

namespace Resources
{
  class IvlLib
  {
  public:
    static Ivl wGetCurrIvl(void);
  
    static Ivl wToIvl(Dtm wTsp);
    static Ivl wToIvl(u8 uMicro);
    
    //! to be coded
  
    static Ivl  wAdd(Ivl wIvl1, Ivl wIvl2);
    static Ivl  wAdd(Ivl wIvl, u8 uMicro);
    static u8   uSubtract(Ivl wIvl1, Ivl wIvl2);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
