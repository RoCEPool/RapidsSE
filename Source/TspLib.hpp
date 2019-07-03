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
  
    static Dtm wToLocal(Since* pSince);
    static Dtm wToUTC(Since* pSince);
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
