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
: ListNL_t<Operand>(CN_OPERANDQUEUE)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPERANDQUEUE,_FOOTER_);
  
  _checkInitialised_(CN_OPERANDQUEUE);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_OPERANDQUEUE,_FOOTER_);
  
  vLog(EV_CONSTRUCT,"OperandQueue constructor"_Stz);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
OperandQueue::~OperandQueue(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPERANDQUEUE,_FOOTER_);
  
  vLog(EV_DESTRUCT,"OperandQueue destructor"_Stz);
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
/* peek at operands and see if any Operand                                    */
/*----------------------------------------------------------------------------*/
bool OperandQueue::bPeekQueue(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPERANDQUEUE,_FOOTER_);
  
  Operand* pOperand = ListNL_t<Operand>::pHead();
  
  return pOperand ? true : false;
}

/*----------------------------------------------------------------------------*/
/* peek at operands and see if any OoO Operand                                */
/*----------------------------------------------------------------------------*/
bool OperandQueue::bPeekQueueOoO(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPERANDQUEUE,_FOOTER_);
  
  Operand* pOperand = ListNL_t<Operand>::pHead();
  
  while (pOperand) {
    
    // is it an Out-of_Order Operand ?
    if (AM::bIsOperandOoO(pOperand)) {
      return true;
    }
    
    pOperand = pOperand->pNextItem();
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/* pop first operand from top of queue                                        */
/*----------------------------------------------------------------------------*/
Operand* OperandQueue::pPopQueue(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPERANDQUEUE,_FOOTER_);
  
  Operand* pOperand = ListNL_t<Operand>::pPop();

  return pOperand;
}

/*----------------------------------------------------------------------------*/
/* pop first OoO operand from top of queue                                    */
/*----------------------------------------------------------------------------*/
Operand* OperandQueue::pPopQueueOoO(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPERANDQUEUE,_FOOTER_);
  
  Operand* pOperand = ListNL_t<Operand>::pHead();
  
  while (pOperand) {
    
    // is it a CANCELMARKER ?
    if (AM::bIsOperandOoO(pOperand)) {
      vRemove(pOperand);
      return pOperand;
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

  i8 nOperands = ListNL_t<Operand>::nItems();

  return nOperands;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
