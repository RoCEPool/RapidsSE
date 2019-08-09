/*============================================================================*/
/* OperandQueue.cpp -- queue of buffers to be processed by an op              */
/*============================================================================*/

#include "Operators.h"

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
OperandQueue* Operators::_pOperandQueue(void* aAddr)
{
  OperandQueue* pOperandQueue = reinterpret_cast<OperandQueue*>(aAddr);
  
  RASSERT_TYPE(pOperandQueue,CN_OPERANDQUEUE);
  
  return pOperandQueue;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* OperandQueue::operator new(size_t)
{
  void* pObject = HeapMem::pAllocate(CN_OPERANDQUEUE);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void OperandQueue::operator delete(void* aAddr)
{
  OperandQueue* pOperandQueue = reinterpret_cast<OperandQueue*>(aAddr);
  
  HeapMem::vFree(pOperandQueue);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
OperandQueue::OperandQueue(void)
: ListNL_t<Operand>(CN_OPERANDQUEUE)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPERANDQUEUE,_FOOTER_);
  
  _checkInitialised_(CN_OPERANDQUEUE);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_OPERANDQUEUE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
OperandQueue::~OperandQueue(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPERANDQUEUE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* append thread to end of queue                                              */
/*----------------------------------------------------------------------------*/
void OperandQueue::vAppendOperand(Operand* pOperand)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPERANDQUEUE,_FOOTER_);
  
  ListNL_t<Operand>::vAppend(pOperand);
}

/*----------------------------------------------------------------------------*/
/* peek at first thread at top of queue                                       */
/*----------------------------------------------------------------------------*/
Operand* OperandQueue::pPeekOperand(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPERANDQUEUE,_FOOTER_);
  
  Operand* pOperand = ListNL_t<Operand>::pHead();

  return pOperand;
}

/*----------------------------------------------------------------------------*/
/* pop first thread from top of queue                                         */
/*----------------------------------------------------------------------------*/
Operand* OperandQueue::pPopQueue(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPERANDQUEUE,_FOOTER_);
  
  Operand* pOperand = ListNL_t<Operand>::pPop();

  return pOperand;
}

/*----------------------------------------------------------------------------*/
/* get number of threads in queue                                             */
/*----------------------------------------------------------------------------*/
i8 OperandQueue::nQueuedOperands(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPERANDQUEUE,_FOOTER_);

  i8 nOperands = ListNL_t<Operand>::nObjects();

  return nOperands;
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(OperandQueue);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
