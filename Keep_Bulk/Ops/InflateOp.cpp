/*============================================================================*/
/* InflateOp.cpp -- Inflate                                                   */
/*============================================================================*/

#include "DataOperators.h"

OpDesc __Op_Inflate (
  CN_INFLATEOP,         // class number
  OK_SINGLE,            // op kind
  "Inflate"_Stz,        // first word of prompt command
  "Inf"_Stz,            // prefix
                   
  false,                // paritioned
  false,                // compiler op

  true,                 // accepts CrwBuffer?
  true,                 // accepts EntBuffer?
  true,                 // accepts IrwBuffer?
  false,                // accepts SrtBuffer?
                   
  true,                 // accepts CrwBuffer?
  true,                 // accepts EntBuffer?
  true,                 // accepts IrwBuffer?
  false,                // accepts SrtBuffer?
                     
  CN_NONE,              // what kind of output
                   
  InflateOp::pNew,      // function which parses the command and new's an op
  InflateOp::eCall,     // function which calls the op loop
  InflateOp::eDrain,    // function which calls the drain loop

  "inflate"_Stz
);

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
InflateOp* DataOperators::_pInflateOp(void* aAddr)
{
  InflateOp* pInflateOp = reinterpret_cast<InflateOp*>(aAddr);
  
  return pInflateOp;
}

/*----------------------------------------------------------------------------*/
/* new                                                                        */
/*----------------------------------------------------------------------------*/
void* InflateOp::operator new(size_t)
{
  InflateOp* pInflateOp =
    static_cast<InflateOp*>(HeapMem::pAllocate(CN_INFLATEOP));
  
  return pInflateOp;
}

/*----------------------------------------------------------------------------*/
/* delete                                                                     */
/*----------------------------------------------------------------------------*/
void InflateOp::operator delete(void* aAddress)
{
  InflateOp* pInflateOp = static_cast<InflateOp*>(aAddress);
  
  HeapMem::vFree(pInflateOp);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
InflateOp::InflateOp(OpSpec* pOpSpec, Pipeline* pPipeline)
: Op(CN_INFLATEOP,pOpSpec,pPipeline)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INFLATEOP,_FOOTER_);
  
  m_fpCall = InflateOp::eCall;
  
  _checkInitialised_(CN_INFLATEOP);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_INFLATEOP,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
InflateOp::~InflateOp(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INFLATEOP,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* parse & new                                                                */
/*----------------------------------------------------------------------------*/
Op* InflateOp::pNew(OpSpec* pOpSpec, Pipeline* pPipeline)
{  
  InflateOp* pInflateOp = new InflateOp(pOpSpec,pPipeline);
  
  return pInflateOp;
}

/*----------------------------------------------------------------------------*/
/* call loop                                                                  */
/*----------------------------------------------------------------------------*/
eOR InflateOp::eCall(void* aThisOp)
{
  eOR eCallAgain = ((InflateOp*)aThisOp)->eOpLoop();
  
  return eCallAgain;
}

/*----------------------------------------------------------------------------*/
/* call drain loop                                                            */
/*----------------------------------------------------------------------------*/
eOR InflateOp::eDrain(void* aThisOp)
{
  eOR eCallAgain = ((InflateOp*)aThisOp)->eDrainLoop();
  
  return eCallAgain;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* op loop                                                                    */
/*----------------------------------------------------------------------------*/
eOR InflateOp::eOpLoop(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INFLATEOP,_FOOTER_);
  
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
          vPipeOut(pOperand);
          goto L_EXIT;
        }
        else
          delete (EndMarker*)pOperand;
        break;
        
      // -- -- -- -- -- --
      
      case CN_CRWBUFFER: {
        CrwBuffer* pCrwBuffer = _pCrwBuffer(pOperand) ;
        //pCrwBuffer->vShow();
        IrwBuffer* pIrwBuffer = pCrwBuffer->pToIrwBuffer();
        vPipeOut(pIrwBuffer);
        break;
      }
        
      case CN_ENTBUFFER: {
        EntBuffer* pEntBuffer = _pEntBuffer(pOperand) ;
        pEntBuffer->vShow();
        IrwBuffer* pIrwBuffer = pEntBuffer->pToIrwBuffer();
        vPipeOut(pIrwBuffer);
        break;
      }
        
      case CN_INTBUFFER: {
        IntBuffer* pIntBuffer = _pIntBuffer(pOperand) ;
        pIntBuffer->vShow();
        vPipeOut(pOperand);
        break;
      }
      
      case CN_REFBUFFER: {
        RefBuffer* pRefBuffer = _pRefBuffer(pOperand) ;
        //pRefBuffer->vShow();
        vPipeOut(pOperand);
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
/* drain loop                                                                 */
/*----------------------------------------------------------------------------*/
eOR InflateOp::eDrainLoop(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INFLATEOP,_FOOTER_);
  
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

__aFooterFunction(InflateOp);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
