/*============================================================================*/
/* Steps_OperandB.cpp -- Operand step group                                   */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "ePX.h"

#include "RowCoder.hpp"
#include "HeadLine.hpp"

#include "Crw.hpp"
#include "CrwPacker.hpp"
#include "CrwBuffer.hpp"
#include "Node_I.hpp"
#include "OpSpec.hpp"
#include "Op.hpp"
#include "Job.hpp"
#include "LogicUnit.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_OperandB(void)
{
  vAddStep("OperandB[]"_Stz,                &Step::vOperandBGet);
  vAddStep("OperandB[].appendClose"_Stz,    &Step::vOperandBAppendClose);
  vAddStep("OperandB[].delete"_Stz,         &Step::vOperandBDelete);
  vAddStep("OperandB[].get"_Stz,            &Step::vOperandBGet);
  vAddStep("OperandB[].isNotNull"_Stz,      &Step::vOperandBIsNotNull);
  vAddStep("OperandB[].isNull"_Stz,         &Step::vOperandBIsNull);
  vAddStep("OperandB[].pipeOut"_Stz,        &Step::vOperandBPipeOut);
  vAddStep("OperandB[].set"_Stz,            &Step::vOperandBSet);
}

/*----------------------------------------------------------------------------*/
/* OperandB[].appendClose -- append close                                     */
/*----------------------------------------------------------------------------*/
void Step::vOperandBAppendClose(void)
{
  ENTRY();
  
  CrwBuffer* pCrwBuffer = _pCrwBuffer(*( m_pJob->pLogicUnit_job() )->m_xpOperandB);
  pCrwBuffer->vAppendClose();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* OperandB[].delete -- operand delete                                        */
/*----------------------------------------------------------------------------*/
void Step::vOperandBDelete(void)
{
  ENTRY();
  
  AM::vDeleteOperand(*( m_pJob->pLogicUnit_job() )->m_xpOperandB);
  
  ( m_pJob->pLogicUnit_job() )->m_xpOperandB = nullptr;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* OperandB[].get -- fetch operand                                             */
/*----------------------------------------------------------------------------*/
void Step::vOperandBGet(void)
{
  ENTRY();
  
  m_pDataStack->vPushPtr(*( m_pJob->pLogicUnit_job() )->m_xpOperandB);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* OperandB[].isNotNull -- is operandB not null ?                             */
/*----------------------------------------------------------------------------*/
void Step::vOperandBIsNotNull(void)
{
  ENTRY();
  
  m_pDataStack->vPushInt(( m_pJob->pLogicUnit_job() )->m_xpOperandB.bIsNull() ? 0 : 1);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* OperandB[].isNull -- is operandB null ?                                    */
/*----------------------------------------------------------------------------*/
void Step::vOperandBIsNull(void)
{
  ENTRY();
  
  m_pDataStack->vPushInt(( m_pJob->pLogicUnit_job() )->m_xpOperandB.bIsNull() ? 1 : 0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* OperandB[].pipeOut -- append operand onto next operator in pipeline         */
/*----------------------------------------------------------------------------*/
void Step::vOperandBPipeOut(void)
{
  ENTRY();
  
  ASSERT(*( m_pJob->pLogicUnit_job() )->m_xpOperandB);
  
  Op* pOp = m_pJob->m_pOp; //  pJob_Distributed->m_pOp;
  
  pOp->vPipeOut_nextOp(*( m_pJob->pLogicUnit_job() )->m_xpOperandB,this);
  
  ( m_pJob->pLogicUnit_job() )->m_xpOperandB = nullptr;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* OperandB[].set -- set operand                                               */
/*----------------------------------------------------------------------------*/
void Step::vOperandBSet(void)
{
  ENTRY();
  
  ASSERT(( m_pJob->pLogicUnit_job() )->m_xpOperandB.bIsNull());
  ( m_pJob->pLogicUnit_job() )->m_xpOperandB = _pOperand(m_pDataStack->pPopPtr(__func__));
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
