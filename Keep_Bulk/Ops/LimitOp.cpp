/*============================================================================*/
/* LimitOp.cpp -- Limit                                                       */
/*============================================================================*/

#include "DataOperators.h"

OpDesc __Op_Limit (
  CN_LIMITOP,         // class number
  OK_SINGLE,          // op kind
  "Limit"_Stz,        // first word of prompt command
  "Lim"_Stz,          // prefix
                   
  false,              // partitioned
  false,              // compiler op

  true,               // accepts CrwBuffer?
  true,               // accepts EntBuffer?
  true,               // accepts IrwBuffer?
  true,               // accepts SrtBuffer?
                   
  true,               // accepts CrwBuffer?
  false,              // accepts EntBuffer?
  true,               // accepts IrwBuffer?
  true,               // accepts SrtBuffer?

  CN_NONE,            // what kind of output
                   
  LimitOp::pNew,      // function which parses the command and new's an op
  LimitOp::eCall,     // function which calls the op loop
  LimitOp::eDrain,    // function which calls the drain loop

  "limit <nRows>"_Stz
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
LimitOp::LimitOp(OpSpec* pOpSpec, Pipeline* pPipeline)
: Op(CN_LIMITOP,pOpSpec,pPipeline)
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
Op* LimitOp::pNew(OpSpec* pOpSpec, Pipeline* pPipeline)
{

  LimitOp* pLimitOp = new LimitOp(pOpSpec,pPipeline);
  
  pLimitOp->m_nLimit = pLimitOp->m_pOpSpec->pEnameSet()->iValue((i8)0);
  
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
  
      // -- -- -- -- -- --
        
      case CN_CRWBUFFER: {
        CrwBuffer* pCrwBuffer = _pCrwBuffer(pOperand);
        
        // are we done ?
        if (bLimitDone()) {
          break;
        }
        
        // do we have a srtbuffer ?
        if ( ! m_pSrtBuffer ) {
          m_pSrtBuffer = new SrtBuffer(1, -1);
        }
        
        // copy limit no of rows from crwBuffer
        
        i8 nRows = pCrwBuffer->nRowCount();
        
        for (i8 i_Row = 0; i_Row < nRows; i_Row++) {
          
          if (m_iWorkDone < m_nLimit) {
            SrtSet* aSrt = m_pSrtBuffer->aAllotSrtSet();
            
            if ( ! aSrt ) {
              vPipeOut(m_pSrtBuffer);
              m_pSrtBuffer = new SrtBuffer(1, -1);
              aSrt = m_pSrtBuffer->aAllotSrtSet();
            }
            
            Crw* aRowData = (Crw*)pCrwBuffer->m_oRowTray.pNub()->aGetHead(i_Row);
            aSrt->vRowRef(0, (i8)aRowData);
            m_iWorkDone++;
          }
        }
          
        break;
      }
        
      // -- -- -- -- -- --
        
      case CN_ENDMARKER:
        if (--m_nLeftEndMarkers == 0) {
          
          // pipeout dangling srtbuffer
          if (m_pSrtBuffer) {
            vPipeOut(m_pSrtBuffer);
            m_pSrtBuffer = nullptr;
          }
          
          // pipeout the end marker
          vPipeOut(pOperand);
          goto L_EXIT;
        }
        else
          delete (EndMarker*)pOperand;
        break;

      // -- -- -- -- -- --
        
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
        
        // copy limit no of rows from entryBuffer
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
      
      // -- -- -- -- -- --
        
      case CN_IRWBUFFER: {
        IrwBuffer* pIrwBuffer = _pIrwBuffer(pOperand);
        
        // are we done ?
        if (bLimitDone()) {
          delete pIrwBuffer;
          break;
        }
        
        // are rows in SrtBuffer lessthan what we need ?
        if (pIrwBuffer->nItems() <  m_nLimit - m_iWorkDone) {
          m_iWorkDone += pIrwBuffer->nItems();
          vPipeOut(pIrwBuffer);
        }
        else {
          // almost done - just trim the srtBuffer
          pIrwBuffer->vItems(m_nLimit - m_iWorkDone);
          m_iWorkDone += pIrwBuffer->nItems();
          vPipeOut(pIrwBuffer);
        }
        
        break;
      }
        
      // -- -- -- -- -- --
        
      case CN_PAUSEMARKER:
        AM::vPulseTask(m_pPipeline->pOpToPulse()->pTask());
        vPipeOut(pOperand);
        break;
     
      // -- -- -- -- -- --
        
      case CN_SRTBUFFER: {
        SrtBuffer* pSrtBuffer = _pSrtBuffer(pOperand);
        
        // are we done ?
        if (bLimitDone()) {
          delete pSrtBuffer;
          break;
        }
        
        // are rows in SrtBuffer lessthan what we need ?
        if (pSrtBuffer->nItems() <  m_nLimit - m_iWorkDone) {
          m_iWorkDone += pSrtBuffer->nItems();
          vPipeOut(pSrtBuffer);
        }
        else {
          // almost done - just trim the srtBuffer
          pSrtBuffer->vItems(m_nLimit - m_iWorkDone);
          m_iWorkDone += pSrtBuffer->nItems();
          vPipeOut(pSrtBuffer);
        }
        
        break;
      }
        
      // -- -- -- -- -- --
        
      case CN_STREAMDEF:
        // pipeout the streamdef as is
        vPipeOut(pOperand);
        break;
      
      // -- -- -- -- -- --
        
      default:
        RASSERT(0);
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
             m_sName.szStz(),
             m_iWorkDone);
      m_pPipeline->vCancelling(true);
    
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
