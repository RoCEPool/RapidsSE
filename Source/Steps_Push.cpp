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
void Step::vPushI(void)
{
  ENTRY("vPushI"_Stz);
  
  ForthValue* aLoopIndex_I = m_pReturnStack->aPeekValue();
  
  m_pDataStack->vPushValue(aLoopIndex_I);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* j -- copy 2nd loop index from return stack onto data stack                 */
/*----------------------------------------------------------------------------*/
void Step::vPushJ(void)
{
  ENTRY("vPushJ"_Stz);
  
  ForthValue* aLoopIndex_J = m_pReturnStack->aPeekValue(3);
  
  m_pDataStack->vPushValue(aLoopIndex_J);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* nullPtr -- push a nullptr                                                  */
/*----------------------------------------------------------------------------*/
void Step::vPushNullPtr(void)
{
  ENTRY("vPushNullPtr"_Stz);
  
  PtrVal xPtrVal;
  xPtrVal = (Object*)nullptr;
  
  m_pDataStack->vPushValue(xPtrVal);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* pushValue -- push value arg onto stack                                     */
/*----------------------------------------------------------------------------*/
void Step::vPushValue(void)
{
  ENTRY("vPushValue"_Stz);
  
  if (m_pTask->m_iTracing) {
    ASSERT(m_pTask->m_iTracing);
  }
  
  ForthValue* aValue = &m_aInxPtr->m_xEmbValue;
  
  m_pDataStack->vPushValue(aValue);
  
  m_pTask->vAdjustFIP(aValue->nSize());
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
