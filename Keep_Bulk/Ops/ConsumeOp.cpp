/*============================================================================*/
/* ConsumeOp.cpp -- Consume                                                   */
/*============================================================================*/

#include "DataOperators.h"

OpDesc __Op_Consume (
  CN_CONSUMEOP,        // class number
  OK_SINK,             // op kind
  "ConsumeOp"_Stz,     // first word of prompt command
  "csm"_Stz,           // prefix
                  
  false,               // partitioned
  false,               // compiler op

  true,                // accepts CrwBuffer?
  true,                // accepts EntBuffer?
  true,                // accepts IrwBuffer?
  true,                // accepts SrtBuffer?
                    
  true,                // accepts CrwBuffer?
  true,                // accepts EntBuffer?
  true,                // accepts IrwBuffer?
  true,                // accepts SrtBuffer?

  CN_NONE,             // what kind of output
                  
  ConsumeOp::pNew,      // function which parses the command and new's an op
  ConsumeOp::eCall,     // function which calls the op loop
  ConsumeOp::eDrain,    // function which calls the drain loop

  "consume"_Stz
);

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
ConsumeOp* DataOperators::_pConsumeOp(void* aAddr)
{
  ConsumeOp* pConsumeOp = reinterpret_cast<ConsumeOp*>(aAddr);
  
  return pConsumeOp;
}

/*----------------------------------------------------------------------------*/
/* new                                                                        */
/*----------------------------------------------------------------------------*/
void* ConsumeOp::operator new(size_t)
{
  ConsumeOp* pConsumeOp =
    static_cast<ConsumeOp*>(HeapMem::pAllocate(CN_CONSUMEOP));
  
  return pConsumeOp;
}

/*----------------------------------------------------------------------------*/
/* delete                                                                     */
/*----------------------------------------------------------------------------*/
void ConsumeOp::operator delete(void* aAddress)
{
  ConsumeOp* pConsumeOp = static_cast<ConsumeOp*>(aAddress);
  
  HeapMem::vFree(pConsumeOp);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
ConsumeOp::ConsumeOp(OpSpec* pOpSpec, Pipeline* pPipeline)
: Op(CN_CONSUMEOP,pOpSpec,pPipeline)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CONSUMEOP,_FOOTER_);
  
  m_fpCall = ConsumeOp::eCall;
  m_pFirstOperand = nullptr;
  
  _checkInitialised_(CN_CONSUMEOP);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CONSUMEOP,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
ConsumeOp::~ConsumeOp(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CONSUMEOP,_FOOTER_);
  
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* parse & new                                                                */
/*----------------------------------------------------------------------------*/
Op* ConsumeOp::pNew(OpSpec* pOpSpec, Pipeline* pPipeline)
{  
  ConsumeOp* pConsumeOp = new ConsumeOp(pOpSpec,pPipeline);
  
  // get the end markers
  EnameSet* pEnameSet = pConsumeOp->m_pOpSpec->pEnameSet();
  pConsumeOp->m_nLeftEndMarkers = pEnameSet->iValue((i8)0);
  
  return pConsumeOp;
}

/*----------------------------------------------------------------------------*/
/* call loop                                                                  */
/*----------------------------------------------------------------------------*/
eOR ConsumeOp::eCall(void* aThisOp)
{
  eOR eCallAgain = ((ConsumeOp*)aThisOp)->eOpLoop();
  
  return eCallAgain;
}

/*----------------------------------------------------------------------------*/
/* call drain loop                                                            */
/*----------------------------------------------------------------------------*/
eOR ConsumeOp::eDrain(void* aThisOp)
{
  eOR eCallAgain = ((ConsumeOp*)aThisOp)->eDrainLoop();
  
  return eCallAgain;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* op loop                                                                    */
/*----------------------------------------------------------------------------*/
eOR ConsumeOp::eOpLoop(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CONSUMEOP,_FOOTER_);
  s4K sMetaData;
  
  RASSERT(0);
  
  for (;;) {
    
    Operand* pOperand = pPopQueue();
    
    //! HACK: TaskLoop_Op
    if (! pOperand) {
      //ASSERT(bIsLocked());
      //return OR_BIDE;
      continue;
    }
    
    // is this the first operand
    if ( ! m_pFirstOperand )
      m_pFirstOperand = pOperand;
    
    // assert first operand is a stream def
    //ASSERT(m_pFirstOperand->eClass() == CN_STREAMDEF);
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    switch (pOperand->eClass()) {
        
      // -- -- -- -- -- --
        
      case CN_CANCELMARKER: {
        m_pPipeline->vCancelling(true);
        break;
      }
        
      // -- -- -- -- -- --
        
      case CN_ENDMARKER: {
        
        // received all expected end markers ?
        if (--m_nLeftEndMarkers == 0) {
          vPipeOut(pOperand);
          goto L_EXIT;
        }
        else
          delete (EndMarker*)pOperand;
        
        break;
      }
       
      // -- -- -- -- -- --
        
      case CN_STREAMDEF: {
      
        if ( ! m_pStreamDef ) {
          
          m_pStreamDef = new StreamDef(_pStreamDef(pOperand));
          
          // pipe out first streamDef received
          vPipeOut(pOperand);
        }
        else {
          delete _pStreamDef(pOperand);
        }
        break;
      }
        
      // -- -- -- -- -- --
        
      default:
        if (bIsCancelling()) {
          // ignore operands if query is being cancelled
          if (pOperand->eClass() != CN_CRWBUFFER)
            delete pOperand;
        }
        else
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
eOR ConsumeOp::eDrainLoop(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CONSUMEOP,_FOOTER_);
  
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
        vPipeOut(pOperand);
        break;
        
      // -- -- -- -- -- --
        
      case CN_INTBUFFER:
        //vPipeOut(pOperand);
        break;
        
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

__aFooterFunction(ConsumeOp);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
