/*============================================================================*/
/* Steps_Trace.cpp - trace step group                                         */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"
#include "eTC.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Trace(void)
{
  vAddStep("traceOff"_Stz,          &Step::vTraceOff);
  vAddStep("traceOn"_Stz,           &Step::vTraceOn);
  vAddStep("tracePoint"_Stz,        &Step::vTracePoint);
  vAddStep("trace!"_Stz,            &Step::vTraceSet);
  vAddStep("trace.setComponent"_Stz,&Step::vTraceSetComponent);
  vAddStep("trace.getComponent"_Stz,&Step::vTraceGetComponent);
}

/*----------------------------------------------------------------------------*/
/* turn tracing off                                                           */
/*----------------------------------------------------------------------------*/
void Step::vTraceOff(void)
{
  ENTRY();
  
  m_iTraceMask = 0;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* turn tracing on                                                            */
/*----------------------------------------------------------------------------*/
void Step::vTraceOn(void)
{
  ENTRY();
  
  m_iTraceMask = C_TRACE_STEPS;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* trace point                                                                */
/*----------------------------------------------------------------------------*/
void Step::vTracePoint(void)
{
  ENTRY();
  
  s256 sPointName;
  
  m_pDataStack->vPopStr(sPointName,__func__);
  
  if (AM::bTracepoint())
    Task::vTracePoint(sPointName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* set tracing                                                                */
/*----------------------------------------------------------------------------*/
void Step::vTraceSet(void)
{
  ENTRY();
  
  i8 iTrace = m_pDataStack->iPopInt(__func__);
  
  m_iTraceMask = iTrace;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* trace.setComponent                                                         */
/* Pushes an eEC error code and (optional) error message onto data stack.     */
/*----------------------------------------------------------------------------*/
void Step::vTraceSetComponent(void)
{
  ENTRY();

  s256 sKey;
  SenCell xSenCell;
  bool bVal;

  m_pDataStack->vPopStr(sKey, __func__);
  m_pDataStack->vPopSenCell(xSenCell, __func__);

  if (xSenCell.bIsIntVal()) {
    bVal = (bool) xSenCell.iGetColVal();
  } else {
    bVal = xSenCell.szStz()->bSame("true"_Stz);
  }

  s256 sErrMsg;
  eEC  eErrCode = EC_NONE;

  eTC eComponent = eFromEnumTC(sKey);

  if (eComponent == TC_NONE) {
    eErrCode = EC_INVALID_TRACE_NAME;
    sErrMsg.vSprint("%s: %s"_Stz, aErrorMsg[eErrCode]);
    // Push an error message and error code onto the stack
    m_pDataStack->vPushStr(sErrMsg);
    m_pDataStack->vPushInt(eErrCode);
    return;
  }

  AM::TRACE::vTraceComponent(eComponent, bVal);
  m_pDataStack->vPushInt(eErrCode);

  EXIT();
}


/*----------------------------------------------------------------------------*/
/* trace.getComponent : Pushes a boolean onto the stack                       */
/*----------------------------------------------------------------------------*/
void Step::vTraceGetComponent(void)
{
  ENTRY();

  s256 sKey;

  m_pDataStack->vPopStr(sKey, __func__);
  eTC eComponent = eFromEnumTC(sKey);

  if (eComponent == TC_NONE || eComponent >= TC_ENUMS) {
    m_pDataStack->vPushBol(false);
  }

  m_pDataStack->vPushBol(AM::TRACE::bTraceComponent(eComponent));

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
