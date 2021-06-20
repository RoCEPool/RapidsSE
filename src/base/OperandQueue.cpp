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
#ifdef WITH_NEW_OPERAND_QUEUE
OperandQueue::OperandQueue(void)
: Queue()
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPERANDQUEUE,_FOOTER_);
  
  _checkInitialised_(CN_OPERANDQUEUE);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_OPERANDQUEUE,_FOOTER_);
  
  AM::LOG::vEvent(EV_CONSTRUCT,"OperandQueue constructor"_Stz);
}
/*----------------------------------------------------------------------------*/
#else
OperandQueue::OperandQueue(void)
: ListNL_t<Operand>(CN_OPERANDQUEUE)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPERANDQUEUE,_FOOTER_);
  
  _checkInitialised_(CN_OPERANDQUEUE);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_OPERANDQUEUE,_FOOTER_);
  
  AM::LOG::vEvent(EV_CONSTRUCT,"OperandQueue constructor"_Stz);
}
#endif

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
OperandQueue::~OperandQueue(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPERANDQUEUE,_FOOTER_);
  
  AM::LOG::vEvent(EV_DESTRUCT,"OperandQueue destructor"_Stz);
}

/*----------------------------------------------------------------------------*/
/* append thread to end of queue                                              */
/*----------------------------------------------------------------------------*/
#ifdef WITH_NEW_OPERAND_QUEUE
void OperandQueue::vAppendOperand(Operand* pOperand)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPERANDQUEUE,_FOOTER_);
  
  Queue::vAppend(pOperand);
}
/*----------------------------------------------------------------------------*/
#else
void OperandQueue::vAppendOperand(Operand* pOperand)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPERANDQUEUE,_FOOTER_);
  
  ListNL_t<Operand>::vAppend(pOperand);
}
#endif

/*----------------------------------------------------------------------------*/
/* peek at operands and see if any Operand                                    */
/*----------------------------------------------------------------------------*/
#ifdef WITH_NEW_OPERAND_QUEUE
bool OperandQueue::bPeekQueue(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPERANDQUEUE,_FOOTER_);
  
  Operand* pOperand = _pOperand(Queue::pPeek());
  
  return pOperand ? true : false;
}
/*----------------------------------------------------------------------------*/
#else
bool OperandQueue::bPeekQueue(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPERANDQUEUE,_FOOTER_);
  
  Operand* pOperand = ListNL_t<Operand>::pHead();
  
  return pOperand ? true : false;
}
#endif

/*----------------------------------------------------------------------------*/
/* peek at operands and see if any OoO Operand                                */
/*----------------------------------------------------------------------------*/
#ifdef WITH_NEW_OPERAND_QUEUE
bool OperandQueue::bPeekOoO(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPERANDQUEUE,_FOOTER_);
  
  Operand* pOperand = _pOperand(Queue::pPeek());
  
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
#else
bool OperandQueue::bPeekOoO(void)
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
#endif

/*----------------------------------------------------------------------------*/
/* pop first operand from top of queue                                        */
/*----------------------------------------------------------------------------*/
#ifdef WITH_NEW_OPERAND_QUEUE
Operand* OperandQueue::pPopQueue(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPERANDQUEUE,_FOOTER_);
  
  Operand* pOperand = _pOperand(Queue::pPop());

  return pOperand;
}
/*----------------------------------------------------------------------------*/
#else
Operand* OperandQueue::pPopQueue(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPERANDQUEUE,_FOOTER_);
  
  Operand* pOperand = ListNL_t<Operand>::pPop();

  return pOperand;
}
#endif

/*----------------------------------------------------------------------------*/
/* pop first OoO operand from top of queue                                    */
/*----------------------------------------------------------------------------*/
#ifdef WITH_NEW_OPERAND_QUEUE
Operand* OperandQueue::pPopOoO(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPERANDQUEUE,_FOOTER_);
  
  Operand* pOperand = _pOperand(Queue::pPeek());
  
  while (pOperand) {
    
    // is it a CANCELMARKER ?
    if (AM::bIsOperandOoO(pOperand)) {
      //vRemove(pOperand);
      RASSERT(0);
      return pOperand;
    }
    
    pOperand = pOperand->pNextItem();
  }
  
  return pOperand;
}
/*----------------------------------------------------------------------------*/
#else
Operand* OperandQueue::pPopOoO(void)
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
#endif

/*----------------------------------------------------------------------------*/
/* get number of threads in queue                                             */
/*----------------------------------------------------------------------------*/
#ifdef WITH_NEW_OPERAND_QUEUE
i8 OperandQueue::nQueuedOperands(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPERANDQUEUE,_FOOTER_);

  i8 nOperands = Queue::nItems();

  return nOperands;
}
/*----------------------------------------------------------------------------*/
#else
i8 OperandQueue::nQueuedOperands(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPERANDQUEUE,_FOOTER_);

  i8 nOperands = ListNL_t<Operand>::nItems();

  return nOperands;
}
#endif

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
