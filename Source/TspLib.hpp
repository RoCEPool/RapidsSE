/*============================================================================*/
/* TspLib.hpp -- TimeStamp Library                                            */
/*============================================================================*/
#pragma once

namespace System
{
  class TspLib
  {
  public:
    static Dtm wGetCurrTsp_local(void);
    static Dtm wGetCurrTsp_utc(void);
  
    static Dtm wToTsp_local(Dtm wIvl);
    static Dtm wToTsp_utc(Dtm wIvl);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
