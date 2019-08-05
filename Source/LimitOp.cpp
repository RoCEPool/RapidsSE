/*============================================================================*/
/* LimitOp.cpp -- Limit                                                       */
/*============================================================================*/

#include "PushDown.h"

OpDesc __Op_Limit (
  CN_LIMITOP,         // class number
  OK_SIMPLE,          // op kind
  "Limit"_Stz,        // first word of prompt command
  "Lim"_Stz,          // prefix

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
LimitOp* PushDown::_pLimitOp(void* aAddr)
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
LimitOp::LimitOp(Stz* szOpLabel, Digest* pDigest, OpDefn* aOpDefn)
: Op(CN_LIMITOP,szOpLabel,pDigest,aOpDefn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LIMITOP,_FOOTER_);
  
  m_fpCall = LimitOp::eCall;

  m_nLimit = 0;
  m_nSoFar = 0;
  
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
Op* LimitOp::pNew(Stz* szLabel, Digest* pDigest, OpDefn* aOpDefn,
                  TokenVec* pTknVec, i8& i_Tkn_)
{
  i_Tkn_++;
  
  LimitOp* pLimitOp = new LimitOp(szLabel,pDigest,aOpDefn);
  
  pLimitOp->m_nLimit = pTknVec->pGet(i_Tkn_++)->iToI8();
  
  return pLimitOp;
}

/*----------------------------------------------------------------------------*/
/* call op loop                                                               */
/*----------------------------------------------------------------------------*/
eOR LimitOp::eCall(void* aThisOp)
{
  eOR eCallAgain = ((LimitOp*)aThisOp)->eLimitLoop();
  
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
eOR LimitOp::eLimitLoop(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LIMITOP,_FOOTER_);
  
  for (;;) {
    
    Operand* pOperand = pPopOperand();
    
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
        AM::vPulseTask(m_pDigest->m_pOpToPulse->pTask());
        vPipeOut(pOperand);
       break;

      // -- -- -- -- -- --
        
      case CN_CRWBUFFER:
        vPipeOut(pOperand);
        m_nSoFar++;
        
        if (m_nSoFar == m_nLimit) {
          vTrace("%s::!cancelling : after popping %lld buffers\n"_Stz,
                 m_sLabel.aStz(),
                 m_nSoFar);
          m_pDigest->m_bCancelling = true;
        }
        
        break;
        
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
/* drain loop                                                                 */
/*----------------------------------------------------------------------------*/
eOR LimitOp::eDrainLoop(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LIMITOP,_FOOTER_);
  
  for (;;) {
    
    Operand* pOperand = pPopOperand();
    
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
