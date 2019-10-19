/*============================================================================*/
/* Steps_Push.cpp - push step group                                           */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Push(void)
{
  vAddStep("i"_Stz,               &Step::vPushI);
  vAddStep("j"_Stz,               &Step::vPushJ);
  vAddStep("nullPtr"_Stz,         &Step::vPushNullPtr);
  vAddStep("pushValue"_Stz,       &Step::vPushValue);
}

/*----------------------------------------------------------------------------*/
/* i -- copy loop index from return stack onto data stack                     */
/*----------------------------------------------------------------------------*/
void Step::vPushI(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vPushI"_Stz);
  
  ForthValue* aLoopIndex_I = pForth->m_pReturnStack->aPeekValue();
  
  pForth->m_pDataStack->vPushValue(aLoopIndex_I);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* j -- copy 2nd loop index from return stack onto data stack                 */
/*----------------------------------------------------------------------------*/
void Step::vPushJ(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vPushJ"_Stz);
  
  ForthValue* aLoopIndex_J = pForth->m_pReturnStack->aPeekValue(3);
  
  pForth->m_pDataStack->vPushValue(aLoopIndex_J);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* nullPtr -- push a nullptr                                                  */
/*----------------------------------------------------------------------------*/
void Step::vPushNullPtr(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vPushNullPtr"_Stz);
  
  PtrVal xPtrVal;
  xPtrVal = (Object*)nullptr;
  
  pForth->m_pDataStack->vPushValue(xPtrVal);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* pushValue -- push value arg onto stack                                     */
/*----------------------------------------------------------------------------*/
void Step::vPushValue(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vPushValue"_Stz);
  
  if (mpTask->m_iTracing) {
    ASSERT(mpTask->m_iTracing);
  }
  
  ForthValue* aValue = &mpTask->m_pForth->m_aInxPtr->m_xEmbValue;
  
  pForth->m_pDataStack->vPushValue(aValue);
  
  mpTask->vAdjustFIP(aValue->nSize());
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
