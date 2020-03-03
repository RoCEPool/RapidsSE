/*============================================================================*/
/* Steps_Operand.cpp -- Operand step group                                    */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_RegOperandB(void)
{
  vAddStep("OperandB[].appendClose"_Stz,    &Step::vOperandBAppendClose);
  vAddStep("OperandB[].get"_Stz,            &Step::vOperandBGet);
  vAddStep("OperandB[].isNull"_Stz,         &Step::vOperandBIsNull);
  vAddStep("OperandB[].pipeout"_Stz,        &Step::vOperandBPipeOut);
  vAddStep("OperandB[].set"_Stz,            &Step::vOperandBSet);
}

/*----------------------------------------------------------------------------*/
/* OperandB[].get -- fetch operand                                             */
/*----------------------------------------------------------------------------*/
void Step::vOperandBAppendClose(void)
{
  ENTRY(); // ENTRY("vOperandBAppendClose"_Stz);
  
  CrwBuffer* pCrwBuffer = _pCrwBuffer(*m_pRegSet->m_xpOperandB);
  pCrwBuffer->vAppendClose();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* OperandB[].get -- fetch operand                                             */
/*----------------------------------------------------------------------------*/
void Step::vOperandBGet(void)
{
  ENTRY(); // ENTRY("vOperandBGet"_Stz);
  
  m_pDataStack->vPushPtr(*m_pRegSet->m_xpOperandB);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* OperandB[].isNull -- is operandB null ?                                    */
/*----------------------------------------------------------------------------*/
void Step::vOperandBIsNull(void)
{
  ENTRY(); // ENTRY("vOperandBIsNull"_Stz);
  
  m_pDataStack->vPushInt(m_pRegSet->m_xpOperandB.bIsNull() ? 1 : 0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* OperandB[].pipeOut -- append operand onto next operator in pipeline         */
/*----------------------------------------------------------------------------*/
void Step::vOperandBPipeOut(void)
{
  ENTRY(); // ENTRY("vOperandBPipeOut"_Stz);
  
  ASSERT(*m_pRegSet->m_xpOperandA);
  
  Op* pOp = m_pTask->m_pJob->m_pOp; //  pJob_Universal->m_pOp;
  
  pOp->vPipeOut(*m_pRegSet->m_xpOperandB);
  
  m_pRegSet->m_xpOperandB = nullptr;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* OperandB[].set -- set operand                                               */
/*----------------------------------------------------------------------------*/
void Step::vOperandBSet(void)
{
  ENTRY(); // ENTRY("vOperandBSet"_Stz);
  
  ASSERT(m_pRegSet->m_xpOperandB.bIsNull());
  m_pRegSet->m_xpOperandB = _pOperand(m_pDataStack->pPopPtr(__func__));
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
