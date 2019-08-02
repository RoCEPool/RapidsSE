/*============================================================================*/
/* ZedOp.cpp -- Zed Sink                                                      */
/*============================================================================*/

#include "PushDown.h"

OpDesc __Op_Zed (
  CN_ZEDOP,         // class number
  OK_SIMPLE,        // op kind
  "Zed"_Stz,        // first word of prompt command
  ZedOp::eCallLoop, // function which does the op
  nullptr,          // function which drains the operand queue
  
  "<help>"_Stz
);

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
ZedOp* PushDown::_pZedOp(void* aAddr)
{
  ZedOp* pZedOp = reinterpret_cast<ZedOp*>(aAddr);
  
  return pZedOp;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* ZedOp::operator new(size_t)
{
  ZedOp* pZedOp =
    static_cast<ZedOp*>(HeapMem::pAllocate(CN_ZEDOP));
  
  return pZedOp;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void ZedOp::operator delete(void* aAddress)
{
  ZedOp* pZedOp = static_cast<ZedOp*>(aAddress);
  
  HeapMem::vFree(pZedOp);
}

/*----------------------------------------------------------------------------*/
/* initialize object                                                          */
/*----------------------------------------------------------------------------*/
ZedOp::ZedOp(Stz* szOpLabel, Jobs::Job* pJob)
: Op(CN_ZEDOP,szOpLabel,pJob)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ZEDOP,_FOOTER_);
  
  _checkInitialised_(CN_ZEDOP);
  
  m_fpOpFun = ZedOp::eCallLoop;
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_ZEDOP,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* initialize object                                                          */
/*----------------------------------------------------------------------------*/
ZedOp::~ZedOp(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ZEDOP,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* call the zed loop function                                                 */
/*----------------------------------------------------------------------------*/
eOR ZedOp::eCallLoop(void* aThis)
{
  eOR eCallAgain = ((ZedOp*)aThis)->eZedLoop();
  
  return eCallAgain;
}

/*----------------------------------------------------------------------------*/
/* the fetch operator function                                                */
/*----------------------------------------------------------------------------*/
eOR ZedOp::eZedLoop(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ZEDOP,_FOOTER_);
  
  for (;;) {
    
    Operand* pOperand = pPopOperand();
    
    if (! pOperand) {
      ASSERT(bIsLocked());
      return OR_BIDE;
    }
    
    if (CN_ENDMARKER == pOperand->eClass()) {
      delete (EndMarker*)pOperand;
      break;
    }
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  return OR_EXIT;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__aFooterFunction(ZedOp);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
