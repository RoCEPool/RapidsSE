/*============================================================================*/
/* LimitOp.cpp -- Limit                                                       */
/*============================================================================*/

#include "DataOperators.h"

OpDesc __Op_Limit (
  CN_LIMITOP,         // class number
  OK_SIMPLE,          // op kind
  "Limit"_Stz,        // first word of prompt command
  "Lim"_Stz,          // prefix
  false,              // not partitioned
  LimitOp::pNew,      // function which parses the command and new's an op
  LimitOp::eCall,     // function which calls the op loop
  LimitOp::eDrain,    // function which calls the drain loop

  "<help>"_Stz
  );


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
LimitOp* DataOperators::_pLimitOp(void* aAddr)
{
  LimitOp* pLimitOp = reinterpret_cast<LimitOp*>(aAddr);
  
  return pLimitOp;
}

/*----------------------------------------------------------------------------*/
/* new                                                                        */
/*----------------------------------------------------------------------------*/
void* LimitOp::operator new(size_t)
{
  LimitOp* pLimitOp = static_cast<LimitOp*>(HeapMem::pAllocate(CN_LIMITOP));
  
  return pLimitOp;
}

/*----------------------------------------------------------------------------*/
/* delete                                                                     */
/*----------------------------------------------------------------------------*/
void LimitOp::operator delete(void* aAddress)
{
  LimitOp* pLimitOp = static_cast<LimitOp*>(aAddress);
  
  HeapMem::vFree(pLimitOp);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
LimitOp::LimitOp(Stz* szOpLabel, Digest* pDigest, OpDefn* aOpDefn, bool bAppR)
: Op(CN_LIMITOP,szOpLabel,pDigest,aOpDefn,bAppR)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LIMITOP,_FOOTER_);
  
  m_fpCall = LimitOp::eCall;

  m_nLimit     = C_UNKNOWN;
  m_pSrtBuffer = nullptr;
  
  _checkInitialised_(CN_LIMITOP);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_LIMITOP,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
LimitOp::~LimitOp(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_LIMITOP);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* parse & new                                                                */
/*----------------------------------------------------------------------------*/
Op* LimitOp::pNew(Stz* szLabel,
                  Digest* pDigest, OpDefn* aOpDefn, bool bAppR,
                  TokenVec* pTknVec, i8& i_Tkn_, i8 i_Slot)
{
  i_Tkn_++;
  
  ASSERT(i_Slot == -1);
  
  LimitOp* pLimitOp = new LimitOp(szLabel,pDigest,aOpDefn,bAppR);
  
  pLimitOp->m_sArgs = pTknVec->pGet(i_Tkn_)->szText();
  
  pLimitOp->m_nLimit = pTknVec->pGet(i_Tkn_++)->iToI8();
  

  return pLimitOp;
}

/*----------------------------------------------------------------------------*/
/* call op loop                                                               */
/*----------------------------------------------------------------------------*/
eOR LimitOp::eCall(void* aThisOp)
{
  eOR eCallAgain = ((LimitOp*)aThisOp)->eOpLoop();
  
  return eCallAgain;
}

/*----------------------------------------------------------------------------*/
/* call drain loop                                                            */
/*----------------------------------------------------------------------------*/
eOR LimitOp::eDrain(void* aThisOp)
{
  eOR eCallAgain = ((LimitOp*)aThisOp)->eDrainLoop();
  
  return eCallAgain;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* op  loop                                                                   */
/*----------------------------------------------------------------------------*/
eOR LimitOp::eOpLoop(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LIMITOP,_FOOTER_);
  
  for (;;) {
    
    Operand* pOperand = pPopQueue();
    
    if (! pOperand) {
      ASSERT(bIsLocked());
      return OR_BIDE;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    switch (pOperand->eClass()) {
        
      case CN_ENDMARKER:
        if (--m_nLeftEndMarkers == 0) {
          if (m_pSrtBuffer) {
            vPipeOut(m_pSrtBuffer);
            m_pSrtBuffer = nullptr;
          }
          vPipeOut(pOperand);
          goto L_EXIT;
        }
        else
          delete (EndMarker*)pOperand;
        break;

      case CN_STREAMDEF:
        vPipeOut(pOperand);
        break;
        
      // -- -- -- -- -- -- 
        
      case CN_PAUSEMARKER:
        AM::vPulseTask(m_pDigest->m_pOpToPulse->pTask());
        vPipeOut(pOperand);
       break;

      // -- -- -- -- -- --
        
      case CN_CRWBUFFER:
//        vPipeOut(pOperand);
//        vIncWorkDone();
//
//        if (m_iWorkDone == m_nLimit) {
//          vTrace("%s::!CANCELLING : after popping %lld buffers\n"_Stz,
//                 m_sLabel.aStz(),
//                 m_iWorkDone);
//          m_pDigest->m_bCancelling = true;
//        }
        RASSERT(0);
        break;
      
      case CN_ENTBUFFER: {
        EntBuffer* pEntBuffer = _pEntBuffer(pOperand);
        
        // are we done ?
        if (bLimitDone()) {
          break;
        }
        
        // do we have a buffer ?
        if ( ! m_pSrtBuffer ) {
          m_pSrtBuffer = new SrtBuffer(1, -1);
        }
        
        //copy limit no of rows from srtBuffer
        Ent* pEnt = pEntBuffer->aLeadEntry();
        while (pEnt && m_iWorkDone < m_nLimit) {
          SrtSet* aSrt = m_pSrtBuffer->aAllotSrtSet();
          
          if ( ! aSrt ) {
            vPipeOut(m_pSrtBuffer);
            m_pSrtBuffer = new SrtBuffer(1, -1);
            aSrt = m_pSrtBuffer->aAllotSrtSet();
          }
          CrwBuffer* pCrwBuffer = CrwBuffer::pRefdCrwBuffer(pEnt->xRef());
          Crw* aRowData = pCrwBuffer->pCrw(pEnt->xRef().iItem());
          aSrt->vRowRef(0, (i8)aRowData);
          pEnt = pEntBuffer->aGetNextEntry(&pEntBuffer, pEnt);
          m_iWorkDone++;
        }
      
        break;
      }
        
      case CN_SRTBUFFER: {
        SrtBuffer* pSrtBuffer = _pSrtBuffer(pOperand);
        
        // are we done ?
        if (bLimitDone()) {
          delete pSrtBuffer;
          break;          
        }
        
        // do we have a buffer ?
        if ( ! m_pSrtBuffer ) {
          m_pSrtBuffer = new SrtBuffer(pSrtBuffer->nRowRefs(), -1);
        }
        
        //copy limit no of rows from srtBuffer
        SrtSet* aSrtSet = pSrtBuffer->aFirstSrtSet();
        while (aSrtSet && m_iWorkDone < m_nLimit) {
          m_pSrtBuffer = m_pSrtBuffer->aCopySrtSet(aSrtSet);
          
          // if bufferr was full and a next was created, pipe it out
          if (m_pSrtBuffer->pNextBuffer()) {
            vPipeOut(m_pSrtBuffer->pNextBuffer());
            m_pSrtBuffer->vNextBuffer(nullptr);
          }
            
          aSrtSet = pSrtBuffer->aNextSrtSet(aSrtSet);
          m_iWorkDone++;
        }
        
        // deete the received buffer
        delete pSrtBuffer;
        break;
      }
        
      default:
        vPipeOut(pOperand);
        break;
    }
    
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
L_EXIT:
  return OR_EXIT;
}

/*----------------------------------------------------------------------------*/
/* has the limit been reached ?                                               */
/*----------------------------------------------------------------------------*/
bool LimitOp::bLimitDone(void)
{
  if (m_iWorkDone == m_nLimit) {
    if (AM::bVerbose())
      vTrace("%s::!CANCELLING : after popping %lld buffers\n"_Stz,
             m_sLabel.aStz(),
             m_iWorkDone);
      m_pDigest->m_bCancelling = true;
    
    if (m_pSrtBuffer) {
      vPipeOut(m_pSrtBuffer);
      m_pSrtBuffer = nullptr;
    }
    return true;
  }
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* drain loop                                                                 */
/*----------------------------------------------------------------------------*/
eOR LimitOp::eDrainLoop(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LIMITOP,_FOOTER_);
  
  for (;;) {
    
    Operand* pOperand = pPopQueue();
    
    if (! pOperand) {
      ASSERT(bIsLocked());
      return OR_BIDE;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
    switch (pOperand->eClass()) {
        
      case CN_ENDMARKER:
        vPipeOut(pOperand);
        goto L_EXIT;
        
        // -- -- -- -- -- --
        
      case CN_PAUSEMARKER:
        delete (PauseMarker*)pOperand;
        break;
        
      case CN_CRWBUFFER:
        break;
        
      case CN_STREAMDEF: {
        StreamDef* pStreamDef = _pStreamDef(pOperand);
        if (pStreamDef->m_bTemp)
          delete pStreamDef;
        break;
      }
        
      default:
        break;
    }
    
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
L_EXIT:
  return OR_EXIT;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__aFooterFunction(LimitOp);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
