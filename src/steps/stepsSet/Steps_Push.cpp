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
 
  vAddStep("pushFipVal"_Stz,      &Step::vPushFipVal);
  vAddStep("pushIntVal"_Stz,      &Step::vPushIntVal);
  vAddStep("pushPtrVal"_Stz,      &Step::vPushPtrVal);
  vAddStep("pushStzVal"_Stz,      &Step::vPushStzVal);
}

/*----------------------------------------------------------------------------*/
/* i -- copy loop index from return stack onto data stack                     */
/*----------------------------------------------------------------------------*/
void Step::vPushI(void)
{
  ENTRY();
  
  TypedCell* aLoopIndex_I = m_pReturnStack->aTopVal();
  
  m_pDataStack->vPushVal(aLoopIndex_I);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* j -- copy 2nd loop index from return stack onto data stack                 */
/*----------------------------------------------------------------------------*/
void Step::vPushJ(void)
{
  ENTRY();
  
  AnyVal* aLoopIndex_J = (AnyVal*)m_pReturnStack->aPeekVal(3);
  
  m_pDataStack->vPushVal((TypedCell*) aLoopIndex_J);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* nullPtr -- push a nullptr                                                  */
/*----------------------------------------------------------------------------*/
void Step::vPushNullPtr(void)
{
  ENTRY();
  
  m_pDataStack->vPushPtr(nullptr);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* pushFip -- push fipval arg onto stack                                      */
/*----------------------------------------------------------------------------*/
void Step::vPushFipVal(void)
{
  ENTRY();
  
  if (m_pTask->m_iTraceMask) {
    ASSERT(m_pTask->m_iTraceMask);
  }
  
  TypedCell* aTypedCell = _aTypedCell(& m_aInxPtr->m_xEmbVal);
  
  m_pDataStack->vPushFip(aTypedCell->aFip());
  
  m_pTask->vAdjustFIP(aTypedCell->nStackUsage());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* pushInt -- push intval arg onto stack                                      */
/*----------------------------------------------------------------------------*/
void Step::vPushIntVal(void)
{
  ENTRY();
  
  if (m_pTask->m_iTraceMask) {
    ASSERT(m_pTask->m_iTraceMask);
  }
  
//  IntVal* aIntVal = _aIntVal(& m_aInxPtr->m_xEmbVal);
//
//  m_pDataStack->vPushInt(aIntVal->iInt());
//
//  m_pTask->vAdjustFIP(aIntVal->nStackUsage());
  
  TypedCell* aTypedCell = _aTypedCell(& m_aInxPtr->m_xEmbVal);
  
  m_pDataStack->vPushInt(aTypedCell->iInt());
  
  m_pTask->vAdjustFIP(aTypedCell->nStackUsage());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* pushPtr -- push ptrval arg onto stack                                      */
/*----------------------------------------------------------------------------*/
void Step::vPushPtrVal(void)
{
  ENTRY();
  
  if (m_pTask->m_iTraceMask) {
    ASSERT(m_pTask->m_iTraceMask);
  }
  
  TypedCell* aTypedCell = _aTypedCell(& m_aInxPtr->m_xEmbVal);
  
  m_pDataStack->vPushPtr(aTypedCell->pObj());
  
  m_pTask->vAdjustFIP(aTypedCell->nStackUsage());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* pushStz -- push strval arg onto stack                                      */
/*----------------------------------------------------------------------------*/
void Step::vPushStzVal(void)
{
  ENTRY();
  
  if (m_pTask->m_iTraceMask) {
    ASSERT(m_pTask->m_iTraceMask);
  }
  
  TypedCell* aTypedCell = _aTypedCell(& m_aInxPtr->m_xEmbVal);
  
//  if (aStzVal->szStz()->bContains("/liteOp/_PulseMarker"_Stz)) {
//    vTrace("vPushStzVal: %s"_Stz,aStzVal->szStz());
//    Resources::vDebugPoint();
//  }
  
  m_pDataStack->vPushStz(aTypedCell->szStz());
  
  m_pTask->vAdjustFIP(aTypedCell->nStackUsage());
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
