/*============================================================================*/
/* Digest.cpp -- Operator Set Digest                                          */
/*============================================================================*/

#include "PushDown.h"

extern OpCatalog gxOpCatalog;

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Digest* PushDown::_pDigest(void* aAddr)
{
  Digest* pDigest = reinterpret_cast<Digest*>(aAddr);
  
  RASSERT_TYPE(pDigest,CN_DIGEST);
  
  return pDigest;
}

/*----------------------------------------------------------------------------*/
/* new                                                                        */
/*----------------------------------------------------------------------------*/
void* Digest::operator new(size_t)
{
  void* pObject = HeapMem::pAllocate(CN_DIGEST);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* delete                                                                     */
/*----------------------------------------------------------------------------*/
void Digest::operator delete(void* aAddr)
{
  Digest* pDigest = reinterpret_cast<Digest*>(aAddr);
  
  HeapMem::vFree(pDigest);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Digest::Digest(Job* pJob)
: Object(CN_DIGEST)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DIGEST,_FOOTER_);
  
  m_pJob = pJob;
  
  m_bPulsed = false;
  
  _checkInitialised_(CN_DIGEST);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_DIGEST,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Digest::~Digest(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DIGEST,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* process dynamic command                                                    */
/*----------------------------------------------------------------------------*/
void Digest::vParseCommand(TokenVec* pTknVec)
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
                   m_oOpVec.nItems());
    Op* pOp = (aOpDefn->m_fpNew)(sLabel,this,aOpDefn,pTknVec,i_Tkn);
    m_oOpVec.vAdd(pOp);
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // link
  
  for (i8 i_Op = 0; i_Op < m_oOpVec.nItems() - 1; i_Op++) {
    
    Op* pOp = m_oOpVec.pGet(i_Op);
    Op* pOpNext = m_oOpVec.pGet(i_Op + 1);
    
    pOp->vNextLabel(pOpNext->szLabel());
    pOp->vOpOut(pOpNext);
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // dump
  
  vTrace("Dynamic Job:\n"_Stz);
  
  for (i8 i_Op = 0; i_Op < m_oOpVec.nItems(); i_Op++) {
    
    Op* pOp = m_oOpVec.pGet(i_Op);
    
    vTrace("  [%02lld] { %6s: %8s(%s) :%-6s }\n"_Stz,
           i_Op,
           pOp->szLabel(),
           aCN[pOp->eClass()],
           pOp->szArgs(),
           pOp->szNextLabel());
  }
  
  vDebugPoint();
}

/*----------------------------------------------------------------------------*/
/* set up the job                                                             */
/*----------------------------------------------------------------------------*/
void Digest::vSetupJob(void)
{
  vDebugPoint();
  
  i8 nOps = m_oOpVec.nItems();
  
  for (i8 i_Op = 0; i_Op < nOps; i_Op++) {
    Op* pOp = m_oOpVec.pGet(i_Op);
    
    if (CN_LIMITOP == pOp->eClass()) {
      m_bPulsed = true;
      break;
    }
  }
  
  if (! m_bPulsed)
    return;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
}



/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(Digest);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
