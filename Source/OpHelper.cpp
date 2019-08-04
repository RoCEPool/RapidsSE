/*============================================================================*/
/* OpHelper.cpp -- a set of op helper functions                               */
/*============================================================================*/

#include "PushDown.h"

extern OpCatalog gxOpCatalog;

/*----------------------------------------------------------------------------*/
/* process dynamic command                                                    */
/*----------------------------------------------------------------------------*/
void OpHelper::vDynamicCommand(Digest* pDigest, TokenVec* pTknVec)
{
  s256 sTknStr;
  s32  sLabel;
  i8 nTokens = pTknVec->nItems();
  
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
    
    // unknown
    if (! aOpDefn) {
      vTrace(" ? %s\n"_Stz,sTknStr.aStz());
      i_Tkn++;
      continue;
    }

    // new an op
    sLabel.vSprint("%s_%02lld"_Stz,
                   aOpDefn->m_szPrefix,
                   pDigest->m_oOpVec.nItems());
    Op* pOp = (aOpDefn->m_fpNew)(pDigest,(Job_Dynamic*)pDigest->m_pJob,sLabel,pTknVec,i_Tkn);
    pDigest->m_oOpVec.vAdd(pOp);
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // link
  
  for (i8 i_Op = 0; i_Op < pDigest->m_oOpVec.nItems() - 1; i_Op++) {
    
    Op* pOp = pDigest->m_oOpVec.pGet(i_Op);
    Op* pOpNext = pDigest->m_oOpVec.pGet(i_Op + 1);
    
    pOp->vNextLabel(pOpNext->szLabel());
    pOp->vOpOut(pOpNext);
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // dump

  vTrace("Dynamic Job:\n"_Stz);
         
  for (i8 i_Op = 0; i_Op < pDigest->m_oOpVec.nItems(); i_Op++) {
    
    Op* pOp = pDigest->m_oOpVec.pGet(i_Op);

    vTrace("  [%02lld] { %6s: %8s(%s) :%-6s }\n"_Stz,
           i_Op,
           pOp->szLabel(),
           aCN[pOp->eClass()],
           pOp->szArgs(),
           pOp->szNextLabel());
  }
  
  vDebugPoint();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
