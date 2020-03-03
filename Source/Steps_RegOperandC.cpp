/*============================================================================*/
/* Steps_OperandC.cpp -- Operand C step group                                 */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_RegOperandC(void)
{
  vAddStep("OperandC[].collect"_Stz,  &Step::vRegOperandCCollect);
  vAddStep("OperandC[].pipeout"_Stz,  &Step::vRegOperandCPipeOut);
  vAddStep("OperandC[].setFirst"_Stz, &Step::vRegOperandCSetFirst);
}

/*----------------------------------------------------------------------------*/
/* OperandC[].collect -- collect operands                                     */
/*----------------------------------------------------------------------------*/
void Step::vRegOperandCCollect(void)
{
  ENTRY(); // ENTRY("vRegOperandCCollect"_Stz);
    
  Operand* pOperand = _pOperand(m_pDataStack->pPopPtr(__func__));

  Operand* pCollect = *m_pRegSet->m_xpOperandC;

  if (pCollect) {
    DataBuffer* pCollect_dataBuffer = _pDataBuffer(pCollect);
    DataBuffer* pOperand_dataBuffer = _pDataBuffer(pOperand);
    
    pCollect_dataBuffer->vNextBuffer(pOperand_dataBuffer);
    pOperand_dataBuffer->vPrevBuffer(pCollect_dataBuffer);
  }

  m_pRegSet->m_xpOperandC = pCollect;
  
  EXIT();
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

/*----------------------------------------------------------------------------*/
/* OperandC[].setFirst -- set to first buffer                                 */
/*----------------------------------------------------------------------------*/
void Step::vRegOperandCSetFirst(void)
{
  ENTRY(); // ENTRY("vRegOperandCSetFirst"_Stz);
  
  Operand* pCollect = *m_pRegSet->m_xpOperandC;

  if (! pCollect)
    return;
  
  // move to first buffer
  DataBuffer* pCollect_dataBuffer = _pDataBuffer(pCollect);
  while (pCollect_dataBuffer->pPrevBuffer())
    {;}
  
  
  Operand* pCollect_first = _pOperand(pCollect_dataBuffer);

  m_pRegSet->m_xpOperandC = pCollect_first;
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
