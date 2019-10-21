/*============================================================================*/
/* DelayOp.cpp -- Fetch                                                       */
/*============================================================================*/

#include "DataOperators.h"

OpDesc __Op_Delay (
  CN_DELAYOP,         // class number
  OK_SOURCE,          // op kind
  "Delay"_Stz,        // first word of prompt command
  "Dly"_Stz,          // prefix
                  
  true,               // partitioned
  false,              // compiler op

  false,              // accepts CrwBuffer?
  false,              // accepts EntBuffer?
  false,              // accepts IrwBuffer?
  false,              // accepts SrtBuffer?
                  
  true,               // accepts CrwBuffer?
  false,              // accepts EntBuffer?
  false,              // accepts IrwBuffer?
  false,              // accepts SrtBuffer?
                  
  CN_NONE,            // what kind of output
                  
  DelayOp::pNew,      // function which parses the command and new's an op
  DelayOp::eCall,     // function which calls the op loop
  nullptr,            // function which calls the drain loop

  "scan <tableNm>"_Stz
);

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
DelayOp* DataOperators::_pDelayOp(void* aAddr)
{
  DelayOp* pDelayOp = reinterpret_cast<DelayOp*>(aAddr);
  
  return pDelayOp;
}

/*----------------------------------------------------------------------------*/
/* new                                                                        */
/*----------------------------------------------------------------------------*/
void* DelayOp::operator new(size_t)
{
  DelayOp* pDelayOp =
    static_cast<DelayOp*>(HeapMem::pAllocate(CN_DELAYOP));
  
  return pDelayOp;
}

/*----------------------------------------------------------------------------*/
/* delete                                                                     */
/*----------------------------------------------------------------------------*/
void DelayOp::operator delete(void* aAddress)
{
  DelayOp* pDelayOp = static_cast<DelayOp*>(aAddress);
  
  HeapMem::vFree(pDelayOp);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
DelayOp::DelayOp(OpSpec* pOpSpec, Pipeline* pPipeline)
: Op(CN_DELAYOP,pOpSpec,pPipeline)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DELAYOP,_FOOTER_);

  m_fpCall = DelayOp::eCall;
  m_nDelay = C_UNKNOWN;
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_DELAYOP,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
DelayOp::~DelayOp(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DELAYOP,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* parse & new                                                                */
/*----------------------------------------------------------------------------*/
Op* DelayOp::pNew(OpSpec* pOpSpec, Pipeline* pPipeline)
{
  DelayOp* pDelayOp = new DelayOp(pOpSpec,pPipeline);
  
  pDelayOp->m_nDelay = StrLib::iToI8(pDelayOp->m_pOpSpec->pEnameSet()->szValue((i8)0)->azChar());
  
  return pDelayOp;
}

/*----------------------------------------------------------------------------*/
/* call loop                                                                  */
/*----------------------------------------------------------------------------*/
eOR DelayOp::eCall(void* aThisOp)
{  
  eOR eCallAgain = ((DelayOp*)aThisOp)->eOpLoop();
  
  return eCallAgain;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* op loop                                                                    */
/*----------------------------------------------------------------------------*/
eOR DelayOp::eOpLoop(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DELAYOP,_FOOTER_);
  
  
  for (;;) {
    
    Operand* pOperand = pPopQueue();
    
    if (! pOperand) {
      ASSERT(bIsLocked());
      return OR_BIDE;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
    vTrace("DelayOP: Napping for %lld seconds\n"_Stz, m_nDelay);
    AM::vNap(m_nDelay * C_1K,CN_SEPROMPT);
    
    // delay piping out the operand
    vPipeOut(pOperand);
    
    // exit if got ENDMARKER
    if (pOperand->eClass() == CN_ENDMARKER)
      break;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  return OR_EXIT;
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__aFooterFunction(DelayOp);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
