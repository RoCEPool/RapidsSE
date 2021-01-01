/*============================================================================*/
/* Steps_Operand.cpp -- Operand step group                                    */
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
void Step::vAddSteps_RegOperandB(void)
{
  vAddStep("OperandB[]"_Stz,                &Step::vRegOperandBGet);
  vAddStep("OperandB[].appendClose"_Stz,    &Step::vRegOperandBAppendClose);
  vAddStep("OperandB[].delete"_Stz,         &Step::vRegOperandBDelete);
  vAddStep("OperandB[].get"_Stz,            &Step::vRegOperandBGet);
  vAddStep("OperandB[].isNotNull"_Stz,      &Step::vRegOperandBIsNotNull);
  vAddStep("OperandB[].isNull"_Stz,         &Step::vRegOperandBIsNull);
  vAddStep("OperandB[].pipeOut"_Stz,        &Step::vRegOperandBPipeOut);
  vAddStep("OperandB[].set"_Stz,            &Step::vRegOperandBSet);
}

/*----------------------------------------------------------------------------*/
/* OperandB[].appendClose -- append close                                     */
/*----------------------------------------------------------------------------*/
void Step::vRegOperandBAppendClose(void)
{
  ENTRY();
  
  CrwBuffer* pCrwBuffer = _pCrwBuffer(*pLogicUnit_task()->m_xpOperandB);
  pCrwBuffer->vAppendClose();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* OperandB[].delete -- operand delete                                        */
/*----------------------------------------------------------------------------*/
void Step::vRegOperandBDelete(void)
{
  ENTRY();
  
  AM::vDeleteOperand(*pLogicUnit_task()->m_xpOperandB);
  
  pLogicUnit_task()->m_xpOperandB = nullptr;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* OperandB[].get -- fetch operand                                             */
/*----------------------------------------------------------------------------*/
void Step::vRegOperandBGet(void)
{
  ENTRY();
  
  m_pDataStack->vPushPtr(*pLogicUnit_task()->m_xpOperandB);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* OperandB[].isNotNull -- is operandB not null ?                             */
/*----------------------------------------------------------------------------*/
void Step::vRegOperandBIsNotNull(void)
{
  ENTRY();
  
  m_pDataStack->vPushInt(pLogicUnit_task()->m_xpOperandB.bIsNull() ? 0 : 1);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* OperandB[].isNull -- is operandB null ?                                    */
/*----------------------------------------------------------------------------*/
void Step::vRegOperandBIsNull(void)
{
  ENTRY();
  
  m_pDataStack->vPushInt(pLogicUnit_task()->m_xpOperandB.bIsNull() ? 1 : 0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* OperandB[].pipeOut -- append operand onto next operator in pipeline         */
/*----------------------------------------------------------------------------*/
void Step::vRegOperandBPipeOut(void)
{
  ENTRY();
  
  ASSERT(*pLogicUnit_task()->m_xpOperandB);
  
  Op* pOp = m_pJob->m_pOp; //  pJob_Distributed->m_pOp;
  
  pOp->vPipeOut_nextOp(*pLogicUnit_task()->m_xpOperandB,this);
  
  pLogicUnit_task()->m_xpOperandB = nullptr;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* OperandB[].set -- set operand                                               */
/*----------------------------------------------------------------------------*/
void Step::vRegOperandBSet(void)
{
  ENTRY();
  
  ASSERT(pLogicUnit_task()->m_xpOperandB.bIsNull());
  pLogicUnit_task()->m_xpOperandB = _pOperand(m_pDataStack->pPopPtr(__func__));
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
