/*============================================================================*/
/* OpHelper.cpp -- a set of op helper functions                               */
/*============================================================================*/

#include "PushDown.h"

extern OpCatalog gxOpCatalog;

/*----------------------------------------------------------------------------*/
/* process dynamic command                                                    */
/*----------------------------------------------------------------------------*/
void OpHelper::vDynamicCommand(Job_Dynamic* pJob, Op** apaOp, TokenVec* pTknVec)
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
      Op* pOp = (aOpDefn->m_fpNew)(pJob,pTknVec,i_Tkn);
      if (nOps)
        apaOp[nOps-1]->vOpOut(pOp);
      apaOp[nOps] = pOp;
      nOps++;
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
