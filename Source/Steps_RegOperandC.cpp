/*============================================================================*/
/* Steps_OperandC.cpp -- Operand C step group                                 */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_RegOperandC(void)
{
  vAddStep("OperandC[].pipeout"_Stz,        &Step::vRegOperandCPipeOut);
}

/*----------------------------------------------------------------------------*/
/* OperandC[].pipeOut -- append operand onto next operator in pipeline        */
/*----------------------------------------------------------------------------*/
void Step::vRegOperandCPipeOut(void)
{
  ENTRY(); // ENTRY("vRegOperandCPipeOut"_Stz);
  
  ASSERT(*m_pRegSet->m_xpOperandC);
  
  Op* pOp = m_pTask->m_pJob->m_pOp; //  pJob_Universal->m_pOp;
  
  pOp->vPipeOut(*m_pRegSet->m_xpOperandC);
  
  m_pRegSet->m_xpOperandC = nullptr;
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
