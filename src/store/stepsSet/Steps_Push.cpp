/*============================================================================*/
/* Steps_Push.cpp - push step group                                           */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"


/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Push(void)
{
  vAddStep("i"_Stz,               &Step::vPushI);
  vAddStep("j"_Stz,               &Step::vPushJ);
  vAddStep("nullPtr"_Stz,         &Step::vPushNullPtr);
 
  vAddStep("pushBcdVal"_Stz,      &Step::vPushBcdVal);
  vAddStep("pushBolVal"_Stz,      &Step::vPushBolVal);
  vAddStep("pushFipVal"_Stz,      &Step::vPushFipVal);
  vAddStep("pushFltVal"_Stz,      &Step::vPushFltVal);
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
  
  m_pDataStack->vPushItem(aLoopIndex_I);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* j -- copy 2nd loop index from return stack onto data stack                 */
/*----------------------------------------------------------------------------*/
void Step::vPushJ(void)
{
  ENTRY();
  
  TypedCell* aLoopIndex_J = m_pReturnStack->aPeekItem(3);
  
  m_pDataStack->vPushItem(aLoopIndex_J);
  
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
/* pushBcd -- push bcdval arg onto stack                                      */
/*----------------------------------------------------------------------------*/
void Step::vPushBcdVal(void)
{
  ENTRY();
  
  if (m_iTraceMask) {
    ASSERT(m_iTraceMask);
  }
    
  TypedCell* aTypedCell = _aTypedCell(& m_aInxPtr->m_xTypedValue);
  
  m_pDataStack->vPushBcd(aTypedCell->iBcd());
  
  vAdjustFIP(aTypedCell->nStackUsage());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* pushBol -- push bolval arg onto stack                                      */
/*----------------------------------------------------------------------------*/
void Step::vPushBolVal(void)
{
  ENTRY();
  
  if (m_iTraceMask) {
    ASSERT(m_iTraceMask);
  }
    
  TypedCell* aTypedCell = _aTypedCell(& m_aInxPtr->m_xTypedValue);
  
  m_pDataStack->vPushBol(aTypedCell->iBol());
  
  vAdjustFIP(aTypedCell->nStackUsage());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* pushFip -- push fipval arg onto stack                                      */
/*----------------------------------------------------------------------------*/
void Step::vPushFipVal(void)
{
  ENTRY();
  
  if (m_iTraceMask) {
    ASSERT(m_iTraceMask);
  }
  
  TypedCell* aTypedCell = _aTypedCell(& m_aInxPtr->m_xTypedValue);
  
  m_pDataStack->vPushFip(aTypedCell->aFip());
  
  vAdjustFIP(aTypedCell->nStackUsage());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* pushFlt -- push fltval arg onto stack                                      */
/*----------------------------------------------------------------------------*/
void Step::vPushFltVal(void)
{
  ENTRY();
  
  if (m_iTraceMask) {
    ASSERT(m_iTraceMask);
  }
  
  TypedCell* aTypedCell = _aTypedCell(& m_aInxPtr->m_xTypedValue);
  
  m_pDataStack->vPushFlt(aTypedCell->dFlt());
  
  vAdjustFIP(aTypedCell->nStackUsage());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* pushInt -- push intval arg onto stack                                      */
/*----------------------------------------------------------------------------*/
void Step::vPushIntVal(void)
{
  ENTRY();
  
  if (m_iTraceMask) {
    ASSERT(m_iTraceMask);
  }
    
  TypedCell* aTypedCell = _aTypedCell(& m_aInxPtr->m_xTypedValue);
  
  m_pDataStack->vPushInt(aTypedCell->iInt());
  
  vAdjustFIP(aTypedCell->nStackUsage());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* pushPtr -- push ptrval arg onto stack                                      */
/*----------------------------------------------------------------------------*/
void Step::vPushPtrVal(void)
{
  ENTRY();
  
  if (m_iTraceMask) {
    ASSERT(m_iTraceMask);
  }
  
  TypedCell* aTypedCell = _aTypedCell(& m_aInxPtr->m_xTypedValue);
  
  m_pDataStack->vPushPtr(aTypedCell->pObj());
  
  vAdjustFIP(aTypedCell->nStackUsage());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* pushStz -- push strval arg onto stack                                      */
/*----------------------------------------------------------------------------*/
void Step::vPushStzVal(void)
{
  ENTRY();
  
  if (m_iTraceMask) {
    ASSERT(m_iTraceMask);
  }
  
  TypedCell* aTypedCell = _aTypedCell(& m_aInxPtr->m_xTypedValue);
  
//  if (aStzVal->szStz()->bContains("/liteOp/_PulseMarker"_Stz)) {
//    vTrace("vPushStzVal: %s"_Stz,aStzVal->szStz());
//    BASE::vDebugPoint();
//  }
  
  m_pDataStack->vPushStz(aTypedCell->szStz());
  
  vAdjustFIP(aTypedCell->nStackUsage());
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
