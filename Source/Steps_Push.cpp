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
 
  vAddStep("pushIntVal"_Stz,      &Step::vPushIntVal);
  vAddStep("pushPtrVal"_Stz,      &Step::vPushPtrVal);
  vAddStep("pushStrVal"_Stz,      &Step::vPushStrVal);
}

/*----------------------------------------------------------------------------*/
/* i -- copy loop index from return stack onto data stack                     */
/*----------------------------------------------------------------------------*/
void Step::vPushI(void)
{
  ENTRY(); // ENTRY("vPushI"_Stz);
  
  AnyVal* aLoopIndex_I = m_pReturnStack->aTopVal();
  
  m_pDataStack->vPushVal((AnyVal*) aLoopIndex_I);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* j -- copy 2nd loop index from return stack onto data stack                 */
/*----------------------------------------------------------------------------*/
void Step::vPushJ(void)
{
  ENTRY(); // ENTRY("vPushJ"_Stz);
  
  AnyVal* aLoopIndex_J = m_pReturnStack->aPeekVal(3);
  
  m_pDataStack->vPushVal((AnyVal*) aLoopIndex_J);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* nullPtr -- push a nullptr                                                  */
/*----------------------------------------------------------------------------*/
void Step::vPushNullPtr(void)
{
  ENTRY(); // ENTRY("vPushNullPtr"_Stz);
  
  m_pDataStack->vPushPtr(nullptr);
  
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
  
  AnyVal* aAnyVal = (AnyVal*) & m_aInxPtr->m_xEmbValue;
  
  m_pDataStack->vPushVal((AnyVal*) aAnyVal);
  
  m_pTask->vAdjustFIP(aAnyVal->nStackUsage());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* pushInt -- push intval arg onto stack                                      */
/*----------------------------------------------------------------------------*/
void Step::vPushIntVal(void)
{
  ENTRY(); // ENTRY("vPushInt"_Stz);
  
  if (m_pTask->m_iTraceMask) {
    ASSERT(m_pTask->m_iTraceMask);
  }
  
  IntVal* aIntVal = _aIntVal(& m_aInxPtr->m_xEmbValue);
  
  m_pDataStack->vPushInt(aIntVal->iInt());
  
  m_pTask->vAdjustFIP(aIntVal->nStackUsage());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* pushPtr -- push ptrval arg onto stack                                      */
/*----------------------------------------------------------------------------*/
void Step::vPushPtrVal(void)
{
  ENTRY(); // ENTRY("vPushPtr"_Stz);
  
  if (m_pTask->m_iTraceMask) {
    ASSERT(m_pTask->m_iTraceMask);
  }
  
  PtrVal* aPtrVal = _aPtrVal(& m_aInxPtr->m_xEmbValue);
  
  m_pDataStack->vPushPtr(aPtrVal->pObj());
  
  m_pTask->vAdjustFIP(aPtrVal->nStackUsage());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* pushStr -- push strval arg onto stack                                      */
/*----------------------------------------------------------------------------*/
void Step::vPushStrVal(void)
{
  ENTRY(); // ENTRY("vPushStr"_Stz);
  
  if (m_pTask->m_iTraceMask) {
    ASSERT(m_pTask->m_iTraceMask);
  }
  
  StzVal* aStzVal = _aStzVal(& m_aInxPtr->m_xEmbValue);
  
  m_pDataStack->vPushStz(aStzVal->szStz());
  
  m_pTask->vAdjustFIP(aStzVal->nStackUsage());
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
