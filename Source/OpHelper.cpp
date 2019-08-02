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
  s256 sTknStr;
  i8 nTokens = pTknVec->nItems();

  i8 nOps = 0;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // create
  
  i8 i_Tkn = 1;
  while (i_Tkn < nTokens) {

    sTknStr = pTknVec->pGet(i_Tkn)->aText();
    
    if (sTknStr.bSame("|"_Stz)) {
      i_Tkn++;
      continue;
    }
    
    OpDefn* aOpDefn = gxOpCatalog.aFindOp(sTknStr);
    
    if (aOpDefn) {
      s32 sLabel(aOpDefn->m_szCommand);
      Op* pOp = (aOpDefn->m_fpNew)(pJob,sLabel,pTknVec,i_Tkn);
      apaOp[nOps] = pOp;
      nOps++;
      continue;
    }

    // unknown
    vTrace(" ? %s\n"_Stz,sTknStr.aStz());
    i_Tkn++;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // link
  
  for (i8 i_Op = 0; i_Op < nOps - 1; i_Op++) {
    
    Op* pOp = apaOp[i_Op];
    Op* pOpNext = apaOp[i_Op + 1];
    
    pOp->vNextLabel(pOpNext->szLabel());
    pOp->vOpOut(pOpNext);
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // dump

  for (i8 i_Op = 0; i_Op < nOps; i_Op++) {
    
    Op* pOp = apaOp[i_Op];

    vTrace(" [%02lld] { %8s: %8s :%-8s }\n"_Stz,
           i_Op,
           pOp->szLabel(),
           aCN[pOp->eClass()],
           pOp->szNextLabel());
  }
  
  vDebugPoint();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
