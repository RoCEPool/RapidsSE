/*============================================================================*/
/* Steps_OperandC.cpp -- Operand C step group                                 */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_RegOperandC(void)
{
  vAddStep("OperandC[]"_Stz,             &Step::vRegOperandCGet);
  vAddStep("OperandC[].collect"_Stz,     &Step::vRegOperandCCollect);
  vAddStep("OperandC[].get"_Stz,         &Step::vRegOperandCGet);
  vAddStep("OperandC[].pipeOut"_Stz,     &Step::vRegOperandCPipeOut);
  vAddStep("OperandC[].pipeOutSet"_Stz,  &Step::vRegOperandCPipeOutSet);
  vAddStep("OperandC[].set"_Stz,         &Step::vRegOperandCSet);
  vAddStep("OperandC[].setFirst"_Stz,    &Step::vRegOperandCSetFirst);
}

/*----------------------------------------------------------------------------*/
/* OperandC[].collect -- collect operands                                     */
/*----------------------------------------------------------------------------*/
void Step::vRegOperandCCollect(void)
{
  ENTRY();
    
  Operand* pOperand = _pOperand(m_pDataStack->pPopPtr(__func__));

  Operand* pCollectingTail = nullptr;
  
  if (! m_pLogicUnit->m_xpOperandC.bIsNull())
    pCollectingTail = *m_pLogicUnit->m_xpOperandC;

  if (pCollectingTail) {
    DataBuffer* pCollect_dataBuffer = _pDataBuffer(pCollectingTail);
    DataBuffer* pOperand_dataBuffer = _pDataBuffer(pOperand);
    
    pCollect_dataBuffer->vNextBuffer(pOperand_dataBuffer);
    pOperand_dataBuffer->vPrevBuffer(pCollect_dataBuffer);
    
    pCollectingTail = pOperand;
  }
  else {
    pCollectingTail = pOperand;
  }

  m_pLogicUnit->m_xpOperandC = pCollectingTail;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* OperandC[].get -- fetch operand                                            */
/*----------------------------------------------------------------------------*/
void Step::vRegOperandCGet(void)
{
  ENTRY();
  
  m_pDataStack->vPushPtr(*m_pLogicUnit->m_xpOperandC);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* OperandC[].pipeOut -- append operand onto next operator in pipeline        */
/*----------------------------------------------------------------------------*/
void Step::vRegOperandCPipeOut(void)
{
  ENTRY();
  
  ASSERT(*m_pLogicUnit->m_xpOperandC);
  
  Op* pOp = m_pTask->m_pJob->m_pOp; //  pJob_Distributed->m_pOp;

  pOp->vPipeOut_nextOp(*m_pLogicUnit->m_xpOperandC,m_pTask);
  
  m_pLogicUnit->m_xpOperandA = nullptr;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* OperandC[].pipeOut -- append operand onto next operator in pipeline        */
/*----------------------------------------------------------------------------*/
void Step::vRegOperandCPipeOutSet(void)
{
  ENTRY();
  
  if (m_pLogicUnit->m_xpOperandC.bIsNull())
    return;
    
  Op* pOp = m_pTask->m_pJob->m_pOp;
  
  pOp->vPipeOut_nextOp(*m_pLogicUnit->m_xpOperandC,m_pTask,true);
  
  m_pLogicUnit->m_xpOperandC = nullptr;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* OperandC[].set -- set operand                                              */
/*----------------------------------------------------------------------------*/
void Step::vRegOperandCSet(void)
{
  ENTRY();
  
  Operand* pOperand = _pOperand(m_pDataStack->pPopPtr(__func__));
  ASSERT(! pOperand || m_pLogicUnit->m_xpOperandC.bIsNull());
  m_pLogicUnit->m_xpOperandC = _pOperand(pOperand);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* OperandC[].setFirst -- set to first buffer                                 */
/*----------------------------------------------------------------------------*/
void Step::vRegOperandCSetFirst(void)
{
  ENTRY();
  
  if (m_pLogicUnit->m_xpOperandC.bIsNull())
    return;

  Operand* pCollect = *m_pLogicUnit->m_xpOperandC;
  
  // move to first buffer
  DataBuffer* pCollect_dataBuffer = _pDataBuffer(pCollect);
  while (pCollect_dataBuffer->pPrevBuffer())
    pCollect_dataBuffer = _pDataBuffer(pCollect_dataBuffer->pPrevBuffer());
  
  Operand* pCollect_first = _pOperand(pCollect_dataBuffer);

  m_pLogicUnit->m_xpOperandC = pCollect_first;
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
