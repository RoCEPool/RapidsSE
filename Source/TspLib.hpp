/*============================================================================*/
/* TspLib.hpp -- TimeStamp Library                                            */
/*============================================================================*/
#pragma once

namespace Library
{
  class TspLib
  {
  public:
    static Dtm wGetCurrTsp_local(void);
    static Dtm wGetCurrTsp_utc(void);
  
    static Dtm wToTsp_local(Ivl wIvl);
  
    //! to be coded / used
  
    static Dtm wToTsp_utc(Ivl wIvl);
  
    static Dtm wAdd(Tsp wTsp, Ivl wIvl);
    static Dtm wSubtract(Dtm wTsp, Ivl wIvl);

    static Ivl wDifference(Tsp wTsp_after, Tsp wTsp_before);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
