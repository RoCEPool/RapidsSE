/*============================================================================*/
/* Steps_RegOperandOoO.cpp -- Register Operand Out-of_Order step group        */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "eYX.h"

#include "OpSpec.hpp"
#include "Op.hpp"
#include "Job.hpp"
#include "Node_I.hpp"
#include "LogicUnit.hpp"
#include "StageSpec.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_OperandOoO(void)
{
  vAddStep("OperandOoO[]"_Stz,             &Step::vOperandOoOGet);
  vAddStep("OperandOoO[]\\clear"_Stz,      &Step::vOperandOoOGetAndClear);
  vAddStep("OperandOoO[].delete"_Stz,      &Step::vOperandOoODelete);
  vAddStep("OperandOoO[].isNull"_Stz,      &Step::vOperandOoOIsNull);
  vAddStep("OperandOoO[].next"_Stz,        &Step::vOperandOoONext);
  vAddStep("OperandOoO[].pipeToOutput"_Stz,&Step::vOperandOoOPipeToOutput);
  vAddStep("OperandOoO[].set"_Stz,         &Step::vOperandOoOSet);
}

/*----------------------------------------------------------------------------*/
/* OperandOoO[].delete -- operand delete                                      */
/*----------------------------------------------------------------------------*/
void Step::vOperandOoODelete(void)
{
  ENTRY();
  
  AM::vDeleteOperand(*m_p_TaskLogicUnit->m_xpOperandOoO);
  
  m_p_TaskLogicUnit->m_xpOperandOoO = nullptr;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* OperandOoO[].get -- fetch operand                                          */
/*----------------------------------------------------------------------------*/
void Step::vOperandOoOGet(void)
{
  ENTRY();
  
  m_pDataStack->vPushPtr(*m_p_TaskLogicUnit->m_xpOperandOoO);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* OperandOoO[]\clear -- fetch operand and set to nullptr                     */
/*----------------------------------------------------------------------------*/
void Step::vOperandOoOGetAndClear(void)
{
  ENTRY();
  
  m_pDataStack->vPushPtr(*m_p_TaskLogicUnit->m_xpOperandOoO);
  
  m_p_TaskLogicUnit->m_xpOperandOoO = nullptr;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* OperandOoO[].isNull -- is operand null ?                                   */
/*----------------------------------------------------------------------------*/
void Step::vOperandOoOIsNull(void)
{
  ENTRY();
  
  m_pDataStack->vPushInt(m_p_TaskLogicUnit->m_xpOperandOoO.bIsNull() ? 1 : 0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* OperandOoO[].Next -- get next OoO operand                                  */
/*----------------------------------------------------------------------------*/
void Step::vOperandOoONext(void)
{
  ENTRY();
  
  ASSERT(m_p_TaskLogicUnit->m_xpOperandOoO.bIsNull());
  
  // revised job/op model
    Op* pOp = *m_pOp;

  m_p_TaskLogicUnit->m_xpOperandOoO = pOp->pPopOoO();
  
  if (m_p_TaskLogicUnit->m_xpOperandOoO.bIsNull())
    m_pDataStack->vPushInt(false);
  else
    m_pDataStack->vPushInt(true);
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* OperandOoO[].pipeToOutput -- append operand onto the pipeline job buffer queue*/
/*----------------------------------------------------------------------------*/
void Step::vOperandOoOPipeToOutput(void)
{
  ENTRY();
  
  ASSERT(*m_p_TaskLogicUnit->m_xpOperandOoO);
  
  // revised job/op model
    Op* pOp = *m_pOp;

  pOp->vPipeToOutput(*m_p_TaskLogicUnit->m_xpOperandOoO);
  
  m_p_TaskLogicUnit->m_xpOperandOoO = nullptr;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* OperandOoO[].set -- set operand                                            */
/*----------------------------------------------------------------------------*/
void Step::vOperandOoOSet(void)
{
  ENTRY();
  
  Operand* pOperand = _pOperand(m_pDataStack->pPopPtr(__func__));
  ASSERT(! pOperand || m_p_TaskLogicUnit->m_xpOperandOoO.bIsNull());
  m_p_TaskLogicUnit->m_xpOperandOoO = _pOperand(pOperand);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
