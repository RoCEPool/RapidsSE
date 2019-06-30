/*============================================================================*/
/* TspLib.hpp -- TimeStamp Library                                            */
/*============================================================================*/
#pragma once

namespace System
{
  class TspLib
  {
  public:
    static Dtm wGetLocalNow(void);
    static Dtm wGetUTCNow(void);
  
    static Dtm wToLocal(Since* pSince);
    static Dtm wToUTC(Since* pSince);
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
