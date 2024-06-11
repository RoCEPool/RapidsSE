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
  
  TypeValueArea* aLoopIndex_I = m_oReturnStack.pStackHead();
  
  m_oDataStack.vPushTypeValueArea(aLoopIndex_I);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* j -- copy 2nd loop index from return stack onto data stack                 */
/*----------------------------------------------------------------------------*/
void Step::vPushJ(void)
{
  ENTRY();
  
  TypeValueArea* aLoopIndex_J = m_oReturnStack.aPeekItem(3);
  
  m_oDataStack.vPushTypeValueArea(aLoopIndex_J);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* nullPtr -- push a nullptr                                                  */
/*----------------------------------------------------------------------------*/
void Step::vPushNullPtr(void)
{
  ENTRY();
  
  m_oDataStack.vPushPtr(nullptr);
  
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
    
  TypeValueArea* aTypedCell = _aTypeValueArea(& m_aInxPtr->m_xTypedValue);
  
  m_oDataStack.vPushBcd(aTypedCell->iBcd());
  
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
    
  TypeValueArea* aTypedCell = _aTypeValueArea(& m_aInxPtr->m_xTypedValue);
  
  m_oDataStack.vPushBol(aTypedCell->iBol());
  
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
  
  TypeValueArea* aTypedCell = _aTypeValueArea(& m_aInxPtr->m_xTypedValue);
  
  m_oDataStack.vPushFip(aTypedCell->aFip());
  
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
  
  TypeValueArea* aTypedCell = _aTypeValueArea(& m_aInxPtr->m_xTypedValue);
  
  m_oDataStack.vPushFlt(aTypedCell->dFlt());
  
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
    
  TypeValueArea* aTypedCell = _aTypeValueArea(& m_aInxPtr->m_xTypedValue);
  
  m_oDataStack.vPushInt(aTypedCell->iInt());
  
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
  
  TypeValueArea* aTypedCell = _aTypeValueArea(& m_aInxPtr->m_xTypedValue);
  
  m_oDataStack.vPushPtr(aTypedCell->pObj());
  
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
  
  TypeValueArea* aTypedCell = _aTypeValueArea(& m_aInxPtr->m_xTypedValue);
  
//  if (aStzVal->szStz()->bContains_term("/liteOp/_PulseMarker"_Stz)) {
//    vTrace("vPushStzVal: %s"_Stz,aStzVal->szStz());
//    BASE::vDebugPoint();
//  }
  
  m_oDataStack.vPushStz(aTypedCell->szStz());
  
  vAdjustFIP(aTypedCell->nStackUsage());
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
