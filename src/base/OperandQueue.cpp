/*============================================================================*/
/* OperandQueue.cpp -- queue of buffers to be processed by an op              */
/*============================================================================*/

#include "BASE.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(BASE,OperandQueue,CN_OPERANDQUEUE)
__aNewFunction(OperandQueue,CN_OPERANDQUEUE)
__vDeleteFunction(OperandQueue,CN_OPERANDQUEUE)
__aFooterFunction(OperandQueue)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
OperandQueue::OperandQueue(void)
: Queue(CN_OPERANDQUEUE)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPERANDQUEUE,_FOOTER_);

  m_bLatch = false;

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
i8 OperandQueue::iAppendOperand(Operand* pOperand)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPERANDQUEUE,_FOOTER_);

  if (m_bLatch) {
    vTrace("Queue latched: not accepting operand\n"_Stz);
    return Queue::nItems();
  }
  else
    return Queue::iAppend(pOperand);
}

/*----------------------------------------------------------------------------*/
/* latch the queue                                                            */
/*----------------------------------------------------------------------------*/
void OperandQueue::vLatch(bool bLatch)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPERANDQUEUE,_FOOTER_);

  m_bLatch = bLatch;
}

/*----------------------------------------------------------------------------*/
/* peek at operands and see if any Operand                                    */
/*----------------------------------------------------------------------------*/
bool OperandQueue::bPeekQueue(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPERANDQUEUE,_FOOTER_);
  
  Operand* pOperand = _pOperand(Queue::pPeek());
  
  return pOperand ? true : false;
}

/*----------------------------------------------------------------------------*/
/* peek at operands and see if any OoO Operand                                */
/*----------------------------------------------------------------------------*/
bool OperandQueue::bPeekOoO(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPERANDQUEUE,_FOOTER_);
  
  Operand* pOperand = _pOperand(Queue::pPeek());
  
  while (pOperand) {
    
    // is it an Out-of-Order Operand ?
    if (AM::OPERAND::bIsOoO(pOperand)) {
      return true;
    }
    
    pOperand = pOperand->pNextItem();
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/* peek at operands and return Operand without popping it off the queue       */
/*----------------------------------------------------------------------------*/
Operand* OperandQueue::pPeekQueue(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPERANDQUEUE,_FOOTER_);

  Operand* pOperand = _pOperand(Queue::pPeek());

  return pOperand;
}

/*----------------------------------------------------------------------------*/
/* peek at operands and return any OoO Operand without popping it off the queue*/
/*----------------------------------------------------------------------------*/
Operand* OperandQueue::pPeekOoO(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPERANDQUEUE,_FOOTER_);

  Operand* pOperand = _pOperand(Queue::pPeek());

  while (pOperand) {

    // is it an Out-of-Order Operand ?
    if (AM::OPERAND::bIsOoO(pOperand)) {
      return pOperand;
    }

    pOperand = pOperand->pNextItem();
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* pop first operand from top of queue                                        */
/*----------------------------------------------------------------------------*/
Operand* OperandQueue::pPopQueue(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPERANDQUEUE,_FOOTER_);
  
  Operand* pOperand = _pOperand(Queue::pPop());

  return pOperand;
}

/*----------------------------------------------------------------------------*/
/* pop first OoO operand from top of queue                                    */
/*----------------------------------------------------------------------------*/
Operand* OperandQueue::pPopOoO(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPERANDQUEUE,_FOOTER_);
  
  Operand* pOperand = _pOperand(Queue::pPeek());
  
  while (pOperand) {
    
    // is it a CANCELMARKER ?
    if (AM::OPERAND::bIsOoO(pOperand)) {
      Operand* pOperandPopped = _pOperand(Queue::pPop());
      return pOperandPopped;
    }
    
    pOperand = pOperand->pNextItem();
  }
  
  return pOperand;
}

/*----------------------------------------------------------------------------*/
/* get number of threads in queue                                             */
/*----------------------------------------------------------------------------*/
i8 OperandQueue::nQueuedOperands(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPERANDQUEUE,_FOOTER_);

  i8 nOperands = Queue::nItems();

  return nOperands;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
