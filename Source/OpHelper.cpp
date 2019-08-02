/*============================================================================*/
/* OpHelper.cpp -- a set of op helper functions                               */
/*============================================================================*/

#include "PushDown.h"
#include "AM.hpp"

extern OpCatalog gxOpCatalog;

/*----------------------------------------------------------------------------*/
/* process dynamic command                                                    */
/*----------------------------------------------------------------------------*/
void OpHelper::vDynamicCommand(Op** apaOp, TokenVec* pTknVec)
{
  i8 nTokens = pTknVec->nItems();

  s256 sTknStr;
  
  i8 nOps = 0;
  i8 i_Tkn = 1;
  while (i_Tkn < nTokens) {

    sTknStr = pTknVec->pGet(i_Tkn)->aText();
    
    if (sTknStr.bSame("|"_Stz)) {
      i_Tkn++;
      continue;
    }
    
    OpDefn* aOpDefn = gxOpCatalog.aFindOp(sTknStr);
    
    if (aOpDefn) {
      vTrace(" { [%lld] %s }\n"_Stz,
             nOps,aOpDefn->m_szCommand);
      nOps++;
      i_Tkn++;
      continue;
    }

    // unknown
    vTrace(" ? %s\n"_Stz,sTknStr.aStz());
    i_Tkn++;
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
