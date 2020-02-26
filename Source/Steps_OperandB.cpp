/*============================================================================*/
/* Steps_Operand.cpp -- Operand step group                                    */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_OperandB(void)
{
  vAddStep("OperandB[].get"_Stz,            &Step::vOperandBGet);
  vAddStep("OperandB[].pipeout"_Stz,        &Step::vOperandBPipeOut);
  vAddStep("OperandB[].set"_Stz,            &Step::vOperandBSet);
}

/*----------------------------------------------------------------------------*/
/* Operand[].get -- fetch operand                                             */
/*----------------------------------------------------------------------------*/
void Step::vOperandBGet(void)
{
  ENTRY(); // ENTRY("vOperandBGet"_Stz);
  
  m_pDataStack->vPushPtr(*m_pRegSet->m_xpOperandB);
  
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
/* Operand[].set -- set operand                                               */
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
