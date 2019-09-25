/*============================================================================*/
/* Digest.cpp -- Operator Set Digest                                          */
/*============================================================================*/

#include "Operators.h"

extern OpCatalog gxOpCatalog;

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Digest* Operators::_pDigest(void* aAddr)
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
  
  m_pSession   = nullptr;
  m_pJob       = pJob;
  m_bPulsed    = false;
  m_pOpToPulse = nullptr;
  
  m_bCancelling = false;
  
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
  
  bool bAppendRight = false;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // create
  
  i8 i_Tkn = 1;
  i8 i_Op = 0;
  i8 i_SlotsPerNode = AM::pGrid()->m_nSlotsPerNode;
  
  while (i_Tkn < nTokens) {
    
    sTknStr = pTknVec->pGet(i_Tkn)->szText();
    
    if (sTknStr.bSame("|"_Stz)) {
      i_Tkn++;
      bAppendRight = false;
      continue;
    }
    
    if (sTknStr.bSame("||"_Stz)) {
      i_Tkn++;
      bAppendRight = true;
      continue;
    }

    //! temp
    //bAppendRight = false;

    OpDefn* aOpDefn = gxOpCatalog.aFindOp(sTknStr);
    
    // unknown
    if (! aOpDefn) {
      vTrace(" ? %s\n"_Stz,sTknStr.aStz());
      ASSERT(0);
      i_Tkn++;
      continue;
    }
    
    if (aOpDefn->m_bIsPartitioned) {
      
      i8 i_Tkn_Save = i_Tkn;
      
      for (i8 i_Slot = 0; i_Slot < i_SlotsPerNode; i_Slot++) {
        // new an op
        sLabel.vSprint("%s_%02lld_%02lld"_Stz,
                       aOpDefn->m_szPrefix,
                       i_Op,
                       i_Slot);
        i_Tkn = i_Tkn_Save;
        OpSpec* pOpSpec = new OpSpec(sLabel.aStz(),aOpDefn->m_szCommand,bAppendRight,i_Slot);
        Op* pOp = (aOpDefn->m_fpNewOp)(pOpSpec,this,aOpDefn,pTknVec,i_Tkn);
        m_oOpVec.vAdd(pOp);
      }
    }
    else {
      // new an op
      sLabel.vSprint("%s_%02lld"_Stz,
                     aOpDefn->m_szPrefix,
                     i_Op);
      OpSpec* pOpSpec = new OpSpec(sLabel.aStz(),aOpDefn->m_szCommand,bAppendRight,-1);
      Op* pOp = (aOpDefn->m_fpNewOp)(pOpSpec,this,aOpDefn,pTknVec,i_Tkn);
      m_oOpVec.vAdd(pOp);
    }
    i_Op++;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // link the operators

  i_Op = 0;
  
  while (true) {
    
    Op* pOp;
    Op* pOpNext;
    i8 i_OpNext;
    
    // get operator to be linked
    pOp = m_oOpVec.pGet(i_Op);
    
    // get next operator
    if (pOp->m_aOpDefn->m_bIsPartitioned) {
      
      // figure out next operator
      i_OpNext = i_Op + i_SlotsPerNode;
    
      // get the next operator
      pOpNext = m_oOpVec.pGet(i_OpNext);
      
      // is the next operator an appendRight ?
      if (pOpNext->m_bAppendRight) {
        
        // skip this appendRight operator
        if (pOpNext->m_aOpDefn->m_bIsPartitioned) {
          i_OpNext = i_OpNext + i_SlotsPerNode ;
        }
        else {
          i_OpNext = i_OpNext + 1;
        }
      }
      
      // now link this operator with the next we figured out
      for (i8 i_Slot = 0; i_Slot < i_SlotsPerNode; i_Slot++) {
        
        // get the next operator
        pOpNext = m_oOpVec.pGet(i_OpNext);
        
        // link the two operators
        pOp->vNextLabel(pOpNext->szLabel());
        pOp->vOpOut(pOpNext);
        
        if (pOp->m_aOpDefn->m_bIsPartitioned && ! pOpNext->m_aOpDefn->m_bIsPartitioned)
          pOpNext->m_nLeftEndMarkers = i_SlotsPerNode;
        
        if (pOp->m_bAppendRight)
          pOpNext->m_nRightEndMarkers = pOpNext->m_nLeftEndMarkers;
        
        // move to next partition of this operator
        i_Op++;
        pOp = m_oOpVec.pGet(i_Op);
        
        // move to next partition of next operator
        if (pOpNext->m_aOpDefn->m_bIsPartitioned)
          i_OpNext++;
      }
    }
    else {
      
      // not partitioned, figure out next operator
      i_OpNext = i_Op + 1;
      
      // get next operator
      pOpNext = m_oOpVec.pGet(i_OpNext);
      
      // is the next operator an appendRight ?
      if (pOpNext->m_bAppendRight) {
        
        // skip this appendRight operator
        if (pOpNext->m_aOpDefn->m_bIsPartitioned) {
          
          i_OpNext = i_OpNext + i_SlotsPerNode - pOp->m_nSlot;
        }
        else
          i_OpNext = i_OpNext + 1;
      }
      
      // link the two operators
      pOp->vNextLabel(pOpNext->szLabel());
      pOp->vOpOut(pOpNext);
      if (pOp->m_aOpDefn->m_bIsPartitioned && ! pOpNext->m_aOpDefn->m_bIsPartitioned)
        pOpNext->m_nLeftEndMarkers = i_SlotsPerNode;
      
      if (pOp->m_bAppendRight)
        pOpNext->m_nRightEndMarkers = pOpNext->m_nLeftEndMarkers;
      
      //move to next operator to be linked
      i_Op++;
    }

    // are we done ?
    if (i_Op == m_oOpVec.nItems() - 1)
      break;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // dump
  
  if (AM::bVerbose()) {
    vTrace("Dynamic Job:\n"_Stz);
    
    for (i8 i_Op = 0; i_Op < m_oOpVec.nItems(); i_Op++) {
      
      Op* pOp = m_oOpVec.pGet(i_Op);
      
      s32 sArgs;
      sArgs.vSprint("(%s)"_Stz,pOp->szArgs());
      
      pOp->m_pOpSpec->pEnmSet()->vPrint();
      
      vTrace("  [%02lld] { %9s: %8s%-24s endMarkers(L%lld R%lld) :%-6s %s }\n"_Stz,
             i_Op,
             pOp->szLabel(),
             aCN[pOp->eClass()],
             sArgs.aStz(),
             pOp->m_nLeftEndMarkers,
             pOp->m_nRightEndMarkers,
             pOp->szNextLabel(),
             pOp->m_bAppendRight ? "appendRight"_Stz : "appendLeft "_Stz
             );
    }
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
  
  for (i8 i_Op = 0; i_Op < nOps; i_Op++) {
    Op* pOp = m_oOpVec.pGet(i_Op);
    
    if (CN_SCANOP == pOp->eClass()) {
      m_pOpToPulse = pOp;
      pOp->m_bPulsed = true;
      break;
    }
  }
  
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(Digest);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
