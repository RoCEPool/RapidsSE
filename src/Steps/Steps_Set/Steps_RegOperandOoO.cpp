/*============================================================================*/
/* Steps_RegOperandOoO.cpp -- Register Operand Out-of_Order step group        */
/*============================================================================*/

#include "Steps.h"

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
  
  AM::vDeleteOperand(*m_pLogicUnit->m_xpOperandOoO);
  
  m_pLogicUnit->m_xpOperandOoO = nullptr;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* OperandOoO[].get -- fetch operand                                          */
/*----------------------------------------------------------------------------*/
void Step::vRegOperandOoOGet(void)
{
  ENTRY();
  
  m_pDataStack->vPushPtr(*m_pLogicUnit->m_xpOperandOoO);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* OperandOoO[].isNull -- is operand null ?                                   */
/*----------------------------------------------------------------------------*/
void Step::vRegOperandOoOIsNull(void)
{
  ENTRY();
  
  m_pDataStack->vPushInt(m_pLogicUnit->m_xpOperandOoO.bIsNull() ? 1 : 0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* OperandOoO[].Next -- get next OoO operand                                  */
/*----------------------------------------------------------------------------*/
void Step::vRegOperandNextOoO(void)
{
  ENTRY();
  
  ASSERT(m_pLogicUnit->m_xpOperandOoO.bIsNull());
  
  m_pLogicUnit->m_xpOperandOoO = m_pTask->m_pJob->m_pOp->pPopQueueOoO();
  
  if (m_pLogicUnit->m_xpOperandOoO.bIsNull())
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
  
  ASSERT(*m_pLogicUnit->m_xpOperandOoO);
  
  Op* pOp = m_pTask->m_pJob->m_pOp; //  pJob_Distributed->m_pOp;

  pOp->vPipeOut_nextOp(*m_pLogicUnit->m_xpOperandOoO,m_pTask);
  
  m_pLogicUnit->m_xpOperandOoO = nullptr;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* OperandOoO[].pipeToJob -- append operand onto the pipeline job buffer queue*/
/*----------------------------------------------------------------------------*/
void Step::vRegOperandOoOPipeToJob(void)
{
  ENTRY();
  
  ASSERT(*m_pLogicUnit->m_xpOperandOoO);
  
  Op* pOp = m_pTask->m_pJob->m_pOp; //  pJob_Distributed->m_pOp;
  
  pOp->vPipeToJob(*m_pLogicUnit->m_xpOperandOoO);
  
  m_pLogicUnit->m_xpOperandOoO = nullptr;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* OperandOoO[].set -- set operand                                            */
/*----------------------------------------------------------------------------*/
void Step::vRegOperandOoOSet(void)
{
  ENTRY();
  
  Operand* pOperand = _pOperand(m_pDataStack->pPopPtr(__func__));
  ASSERT(! pOperand || m_pLogicUnit->m_xpOperandOoO.bIsNull());
  m_pLogicUnit->m_xpOperandOoO = _pOperand(pOperand);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
