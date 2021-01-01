/*============================================================================*/
/* Steps_RegOperandOoO.cpp -- Register Operand Out-of_Order step group        */
/*============================================================================*/

#include "BASE.h"
#include "STORE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "StepTiming.hpp"


/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_RegOperandOoO(void)
{
  vAddStep("OperandOoO[]"_Stz,             &Step::vRegOperandOoOGet);
  vAddStep("OperandOoO[].delete"_Stz,      &Step::vRegOperandOoODelete);
  vAddStep("OperandOoO[].isNull"_Stz,      &Step::vRegOperandOoOIsNull);
  vAddStep("OperandOoO[].next"_Stz,        &Step::vRegOperandNextOoO);
  vAddStep("OperandOoO[].pipeOut"_Stz,     &Step::vRegOperandOoOPipeOut);
  vAddStep("OperandOoO[].pipeToJob"_Stz,   &Step::vRegOperandOoOPipeToJob);
  vAddStep("OperandOoO[].set"_Stz,         &Step::vRegOperandOoOSet);
}

/*----------------------------------------------------------------------------*/
/* OperandOoO[].delete -- operand delete                                      */
/*----------------------------------------------------------------------------*/
void Step::vRegOperandOoODelete(void)
{
  ENTRY();
  
  AM::vDeleteOperand(*pLogicUnit_task()->m_xpOperandOoO);
  
  pLogicUnit_task()->m_xpOperandOoO = nullptr;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* OperandOoO[].get -- fetch operand                                          */
/*----------------------------------------------------------------------------*/
void Step::vRegOperandOoOGet(void)
{
  ENTRY();
  
  m_pDataStack->vPushPtr(*pLogicUnit_task()->m_xpOperandOoO);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* OperandOoO[].isNull -- is operand null ?                                   */
/*----------------------------------------------------------------------------*/
void Step::vRegOperandOoOIsNull(void)
{
  ENTRY();
  
  m_pDataStack->vPushInt(pLogicUnit_task()->m_xpOperandOoO.bIsNull() ? 1 : 0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* OperandOoO[].Next -- get next OoO operand                                  */
/*----------------------------------------------------------------------------*/
void Step::vRegOperandNextOoO(void)
{
  ENTRY();
  
  ASSERT(pLogicUnit_task()->m_xpOperandOoO.bIsNull());
  
  pLogicUnit_task()->m_xpOperandOoO = m_pJob->m_pOp->pPopQueueOoO();
  
  if (pLogicUnit_task()->m_xpOperandOoO.bIsNull())
    m_pDataStack->vPushInt(false);
  else
    m_pDataStack->vPushInt(true);
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* OperandOoO[].pipeOut -- append operand onto next operator in pipeline      */
/*----------------------------------------------------------------------------*/
void Step::vRegOperandOoOPipeOut(void)
{
  ENTRY();
  
  ASSERT(*pLogicUnit_task()->m_xpOperandOoO);
  
  Op* pOp = m_pJob->m_pOp; //  pJob_Distributed->m_pOp;

  pOp->vPipeOut_nextOp(*pLogicUnit_task()->m_xpOperandOoO,this);
  
  pLogicUnit_task()->m_xpOperandOoO = nullptr;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* OperandOoO[].pipeToJob -- append operand onto the pipeline job buffer queue*/
/*----------------------------------------------------------------------------*/
void Step::vRegOperandOoOPipeToJob(void)
{
  ENTRY();
  
  ASSERT(*pLogicUnit_task()->m_xpOperandOoO);
  
  Op* pOp = m_pJob->m_pOp; //  pJob_Distributed->m_pOp;
  
  pOp->vPipeToJob(*pLogicUnit_task()->m_xpOperandOoO);
  
  pLogicUnit_task()->m_xpOperandOoO = nullptr;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* OperandOoO[].set -- set operand                                            */
/*----------------------------------------------------------------------------*/
void Step::vRegOperandOoOSet(void)
{
  ENTRY();
  
  Operand* pOperand = _pOperand(m_pDataStack->pPopPtr(__func__));
  ASSERT(! pOperand || pLogicUnit_task()->m_xpOperandOoO.bIsNull());
  pLogicUnit_task()->m_xpOperandOoO = _pOperand(pOperand);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
