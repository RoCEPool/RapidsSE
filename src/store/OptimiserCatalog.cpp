/*============================================================================*/
/* OptimiserCatalog.cpp -- the catalog of prompt functions                       */
/*============================================================================*/

#include "BASE.h"

#include "OptimiserFun.hpp"
#include "OptimiserCatalog.hpp"


OptimiserCatalog gxOptimiserCatalog;

/*----------------------------------------------------------------------------*/
/* find function given command                                                */
/*----------------------------------------------------------------------------*/
OptimiserFun* OptimiserCatalog::pFindFun(Stz* szCommand1, Stz* szCommand2)
{
  _outsideOfPool_
  
  for (i8 i_Fun = 0; i_Fun < m_nFunctions; i_Fun++) {
    
    //vTrace("Find: %s v %s\n"_Stz, m_aaFun[i_Fun]->m_aCommand1->azChar(),
    //       aCommand1->azChar());
    
    if (! m_aaFun[i_Fun]->m_szCommand1->bSame(szCommand1))
      continue;
    
    if (m_aaFun[i_Fun]->m_szCommand2) {
      if (! szCommand2->nLength())
        continue;
      if (! m_aaFun[i_Fun]->m_szCommand2->bSame(szCommand2))
        continue;
    }
    
    return m_aaFun[i_Fun];
  }

  return nullptr;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
