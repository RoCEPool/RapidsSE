/*============================================================================*/
/* OptimiserCatalog.hpp -- the catalog of optimiser functions                 */
/*============================================================================*/
#pragma once

namespace STORE
{
  class OptimiserCatalog
  {
  public:
    i8            m_nFunctions;  // number of functions
    OptimiserFun* m_aaFun[128];  // array of function descriptions

  public:
    OptimiserFun* pFindFun(Stz* szCommand1, Stz* szCommand2);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
