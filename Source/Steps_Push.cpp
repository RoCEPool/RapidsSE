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
  ENTRY(); // ENTRY("vPushI"_Stz);
  
  StkVal* aLoopIndex_I = m_pReturnStack->aTopStkVal();
  
  m_pDataStack->vPushValue(aLoopIndex_I);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* j -- copy 2nd loop index from return stack onto data stack                 */
/*----------------------------------------------------------------------------*/
void Step::vPushJ(void)
{
  ENTRY(); // ENTRY("vPushJ"_Stz);
  
  StkVal* aLoopIndex_J = m_pReturnStack->aPeekStkVal(3);
  
  m_pDataStack->vPushValue(aLoopIndex_J);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* nullPtr -- push a nullptr                                                  */
/*----------------------------------------------------------------------------*/
void Step::vPushNullPtr(void)
{
  ENTRY(); // ENTRY("vPushNullPtr"_Stz);
  
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
  ENTRY(); // ENTRY("vPushValue"_Stz);
  
  if (m_pTask->m_iTraceMask) {
    ASSERT(m_pTask->m_iTraceMask);
  }
  
  StkVal* aStkVal = &m_aInxPtr->m_xEmbValue;
  
  m_pDataStack->vPushValue(aStkVal);
  
  m_pTask->vAdjustFIP(aStkVal->nStackBytes());
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
