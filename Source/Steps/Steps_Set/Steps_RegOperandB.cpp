/*============================================================================*/
/* Steps_Operand.cpp -- Operand step group                                    */
/*============================================================================*/

#include "Steps.h"

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
  
  CrwBuffer* pCrwBuffer = _pCrwBuffer(*m_pLogicUnit->m_xpOperandB);
  pCrwBuffer->vAppendClose();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* OperandB[].delete -- operand delete                                        */
/*----------------------------------------------------------------------------*/
void Step::vRegOperandBDelete(void)
{
  ENTRY();
  
  AM::vDeleteOperand(*m_pLogicUnit->m_xpOperandB);
  
  m_pLogicUnit->m_xpOperandB = nullptr;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* OperandB[].get -- fetch operand                                             */
/*----------------------------------------------------------------------------*/
void Step::vRegOperandBGet(void)
{
  ENTRY();
  
  m_pDataStack->vPushPtr(*m_pLogicUnit->m_xpOperandB);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* OperandB[].isNotNull -- is operandB not null ?                             */
/*----------------------------------------------------------------------------*/
void Step::vRegOperandBIsNotNull(void)
{
  ENTRY();
  
  m_pDataStack->vPushInt(m_pLogicUnit->m_xpOperandB.bIsNull() ? 0 : 1);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* OperandB[].isNull -- is operandB null ?                                    */
/*----------------------------------------------------------------------------*/
void Step::vRegOperandBIsNull(void)
{
  ENTRY();
  
  m_pDataStack->vPushInt(m_pLogicUnit->m_xpOperandB.bIsNull() ? 1 : 0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* OperandB[].pipeOut -- append operand onto next operator in pipeline         */
/*----------------------------------------------------------------------------*/
void Step::vRegOperandBPipeOut(void)
{
  ENTRY();
  
  ASSERT(*m_pLogicUnit->m_xpOperandB);
  
  Op* pOp = m_pTask->m_pJob->m_pOp; //  pJob_Distributed->m_pOp;
  
  pOp->vPipeOut_nextOp(*m_pLogicUnit->m_xpOperandB,m_pTask);
  
  m_pLogicUnit->m_xpOperandB = nullptr;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* OperandB[].set -- set operand                                               */
/*----------------------------------------------------------------------------*/
void Step::vRegOperandBSet(void)
{
  ENTRY();
  
  ASSERT(m_pLogicUnit->m_xpOperandB.bIsNull());
  m_pLogicUnit->m_xpOperandB = _pOperand(m_pDataStack->pPopPtr(__func__));
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
