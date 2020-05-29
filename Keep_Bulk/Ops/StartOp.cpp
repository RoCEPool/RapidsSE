/*============================================================================*/
/* StartOp.cpp -- Output                                                     */
/*============================================================================*/

#include "DataOperators.h"

OpDesc __Op_Start (
  CN_STARTOP,         // class number
  OK_SINGLE,          // op kind
  "StartOp"_Stz,      // first word of prompt command
  "Sta"_Stz,          // prefix
                 
  true,               // partitioned
  false,              // compiler op
                 
  true,               // accepts CrwBuffer?
  true,               // accepts EntBuffer?
  true,               // accepts IrwBuffer?
  true,               // accepts SrtBuffer?
                   
  false,              // accepts CrwBuffer?
  false,              // accepts EntBuffer?
  true,               // accepts IrwBuffer?
  false,              // accepts SrtBuffer?
                   
  CN_NONE,            // what kind of output

  StartOp::pNew,      // function which parses the command and new's an op
  StartOp::eCall,     // function which calls the op loop
  nullptr,            // function which calls the drain loop

  "start"_Stz
);

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
StartOp* DataOperators::_pStartOp(void* aAddr)
{
  StartOp* pStartOp = reinterpret_cast<StartOp*>(aAddr);
  
  return pStartOp;
}

/*----------------------------------------------------------------------------*/
/* new                                                                        */
/*----------------------------------------------------------------------------*/
void* StartOp::operator new(size_t)
{
  StartOp* pStartOp =
    static_cast<StartOp*>(HeapMem::pAllocate(CN_STARTOP));
  
  return pStartOp;
}

/*----------------------------------------------------------------------------*/
/* delete                                                                     */
/*----------------------------------------------------------------------------*/
void StartOp::operator delete(void* aAddress)
{
  StartOp* pStartOp = static_cast<StartOp*>(aAddress);
  
  HeapMem::vFree(pStartOp);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
StartOp::StartOp(OpSpec* pOpSpec, Pipeline* pPipeline)
: Op(CN_STARTOP,pOpSpec,pPipeline)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STARTOP,_FOOTER_);
  
  m_bAckJobEnd = false;
  
  m_fpCall = StartOp::eCall;
  
  _checkInitialised_(CN_STARTOP);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_STARTOP,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
StartOp::~StartOp(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STARTOP,_FOOTER_);
  
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* parse & new                                                                */
/*----------------------------------------------------------------------------*/
Op* StartOp::pNew(OpSpec* pOpSpec, Pipeline* pPipeline)
{
  StartOp* pStartOp = new StartOp(pOpSpec,pPipeline);
  
  return pStartOp;
}

/*----------------------------------------------------------------------------*/
/* call loop                                                                  */
/*----------------------------------------------------------------------------*/
eOR StartOp::eCall(void* aThisOp)
{
  eOR eCallAgain = ((StartOp*)aThisOp)->eStartLoop();
  
  return eCallAgain;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* op loop                                                                    */
/*----------------------------------------------------------------------------*/
eOR StartOp::eStartLoop(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STARTOP,_FOOTER_);
  
  for (;;) {
    
    Operand* pOperand = pPopQueue();
    
    //! HACK: TaskLoop_Op
    if (! pOperand) {
      //ASSERT(bIsLocked());
      //return OR_BIDE;
      continue;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
    switch (pOperand->eClass()) {
        
      case CN_ENDMARKER:
        AM::vDeleteOperand(pOperand);
        goto L_EXIT;
        break;
        
      case CN_CANCELMARKER:
        // pipe out the cancel marker
        vTrace("Query being cancelled\n"_Stz);
        m_pPipeline->vCancelling(true);
        vPipeOut(pOperand);
        break;
        
      case CN_STARTMARKER:
        vPipeOut(pOperand);
        break;
        
      default:
        ASSERT(0);
        
    }
  }
    
  L_EXIT:
  return OR_EXIT;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__aFooterFunction(StartOp);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
