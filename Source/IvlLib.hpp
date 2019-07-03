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
  
    //! to be coded
  
    static Dtm wAdd(Dtm wIvl1, Dtm wIvl2);
    static Dtm wSubtract(Dtm wIvl1, Dtm wIvl2);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
