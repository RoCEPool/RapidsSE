/*============================================================================*/
/* RulesFun.cpp -- description of an optimiser function                       */
/*============================================================================*/

#include "RulesFun.hpp"
#include "RulesCatalog.hpp"

extern RulesCatalog gxRulesCatalog;

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
RulesFun::RulesFun(Stz* szName,
                           Stz* szCommand1,
                           Stz* szCommand2,
                           bool (*fpRuleFun)(Optimiser* pOptimiser,
                                             TokenVec*  pTokenVec,
                                             i8&        riAction),
                           i8   iSpare0,
                           i8   iSpare1,
                           Stz* szDescription
                           )
{
  m_szName        = szName;
  m_szCommand1    = szCommand1;
  m_szCommand2    = szCommand2;
  m_fpRuleFun     = fpRuleFun;
  m_iSpare0       = iSpare0;
  m_iSpare1       = iSpare1;
  m_szDescription = szDescription;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // add the function to the catalog
  i8 i_Fun = gxRulesCatalog.m_nFunctions++;
  gxRulesCatalog.m_aaFun[i_Fun] = this;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
