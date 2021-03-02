/*============================================================================*/
/* Steps_RegOperandOoO.cpp -- Register Operand Out-of_Order step group        */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "ePX.h"

#include "OpSpec.hpp"
#include "Op.hpp"
#include "Response.hpp"
#include "Result.hpp"
#include "Job.hpp"
#include "Node_I.hpp"
#include "LogicUnit.hpp"
#include "StageSpec.hpp"

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
  vAddStep("OperandOoO[].pipeToOutput"_Stz,   &Step::vRegOperandOoOPipeToOutput);
  vAddStep("OperandOoO[].set"_Stz,         &Step::vRegOperandOoOSet);
}

/*----------------------------------------------------------------------------*/
/* OperandOoO[].delete -- operand delete                                      */
/*----------------------------------------------------------------------------*/
void Step::vRegOperandOoODelete(void)
{
  ENTRY();
  
  AM::vDeleteOperand(*( m_pJob->pLogicUnit_job() )->m_xpOperandOoO);
  
  ( m_pJob->pLogicUnit_job() )->m_xpOperandOoO = nullptr;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* OperandOoO[].get -- fetch operand                                          */
/*----------------------------------------------------------------------------*/
void Step::vRegOperandOoOGet(void)
{
  ENTRY();
  
  m_pDataStack->vPushPtr(*( m_pJob->pLogicUnit_job() )->m_xpOperandOoO);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* OperandOoO[].isNull -- is operand null ?                                   */
/*----------------------------------------------------------------------------*/
void Step::vRegOperandOoOIsNull(void)
{
  ENTRY();
  
  m_pDataStack->vPushInt(( m_pJob->pLogicUnit_job() )->m_xpOperandOoO.bIsNull() ? 1 : 0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* OperandOoO[].Next -- get next OoO operand                                  */
/*----------------------------------------------------------------------------*/
void Step::vRegOperandNextOoO(void)
{
  ENTRY();
  
  ASSERT(( m_pJob->pLogicUnit_job() )->m_xpOperandOoO.bIsNull());
  
  ( m_pJob->pLogicUnit_job() )->m_xpOperandOoO = m_pJob->m_pOp->pPopQueueOoO();
  
  if (( m_pJob->pLogicUnit_job() )->m_xpOperandOoO.bIsNull())
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
  
  ASSERT(*( m_pJob->pLogicUnit_job() )->m_xpOperandOoO);
  
  Op* pOp = m_pJob->m_pOp; //  pJob_Distributed->m_pOp;

  pOp->vPipeOut_nextOp(*( m_pJob->pLogicUnit_job() )->m_xpOperandOoO,this);
  
  ( m_pJob->pLogicUnit_job() )->m_xpOperandOoO = nullptr;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* OperandOoO[].pipeToOutput -- append operand onto the pipeline job buffer queue*/
/*----------------------------------------------------------------------------*/
void Step::vRegOperandOoOPipeToOutput(void)
{
  ENTRY();
  
  ASSERT(*( m_pJob->pLogicUnit_job() )->m_xpOperandOoO);
  
  Op* pOp = m_pJob->m_pOp; //  pJob_Distributed->m_pOp;
  
  pOp->vPipeToOutput(*( m_pJob->pLogicUnit_job() )->m_xpOperandOoO);
  
  ( m_pJob->pLogicUnit_job() )->m_xpOperandOoO = nullptr;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* OperandOoO[].set -- set operand                                            */
/*----------------------------------------------------------------------------*/
void Step::vRegOperandOoOSet(void)
{
  ENTRY();
  
  Operand* pOperand = _pOperand(m_pDataStack->pPopPtr(__func__));
  ASSERT(! pOperand || ( m_pJob->pLogicUnit_job() )->m_xpOperandOoO.bIsNull());
  ( m_pJob->pLogicUnit_job() )->m_xpOperandOoO = _pOperand(pOperand);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
