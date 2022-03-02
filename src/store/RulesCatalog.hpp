/*============================================================================*/
/* RulesCatalog.hpp -- the catalog of optimiser functions                     */
/*============================================================================*/
#pragma once

namespace STORE
{
  class RulesCatalog
  {
  public:
    i8        m_nFunctions;  // number of functions
    RulesFun* m_aaFun[128];  // array of function descriptions

  public:
    RulesFun* pFindFun(Stz* szCommand1, Stz* szCommand2);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
