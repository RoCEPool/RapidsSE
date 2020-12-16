/*============================================================================*/
/* TspLib.hpp -- TimeStamp Library                                            */
/*============================================================================*/
#pragma once

namespace Resources
{
  class TspLib
  {
  public:
    static Tsp wGetCurrTsp_local(void);
  
    static Tsp wToTsp_local(Ivl wIvl);
  
    //! to be coded / used
  
    static Tsp wGetCurrTsp_utc(void);
    static Tsp wToTsp_utc(Ivl wIvl);
  
    static Tsp wAdd(Tsp wTsp, Ivl wIvl);
    static Tsp wSubtract(Dtm wTsp, Ivl wIvl);

    static Ivl wDifference(Tsp wTsp_after, Tsp wTsp_before);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
