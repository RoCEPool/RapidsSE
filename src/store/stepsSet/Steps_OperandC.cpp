/*============================================================================*/
/* Steps_OperandC.cpp -- Operand C step group                                 */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "ePX.h"

#include "Node_I.hpp"
#include "Op.hpp"
#include "Job.hpp"
#include "LogicUnit.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_OperandC(void)
{
  vAddStep("OperandC[]"_Stz,             &Step::vOperandCGet);
  vAddStep("OperandC[]\\clear"_Stz,      &Step::vOperandCGetAndClear);
  vAddStep("OperandC[].collect"_Stz,     &Step::vOperandCCollect);
  vAddStep("OperandC[].get"_Stz,         &Step::vOperandCGet);
  vAddStep("OperandC[].set"_Stz,         &Step::vOperandCSet);
}

/*----------------------------------------------------------------------------*/
/* OperandC[].collect -- collect operands                                     */
/*----------------------------------------------------------------------------*/
void Step::vOperandCCollect(void)
{
  ENTRY();
    
  Operand* pOperand = _pOperand(m_pDataStack->pPopPtr(__func__));

  Operand* pCollectingTail = nullptr;
  
  if (! m_pJob->pLogicUnit_job()->m_xpOperandC.bIsNull())
    pCollectingTail = *m_pJob->pLogicUnit_job()->m_xpOperandC;

  if (pCollectingTail) {
    DataBuffer* pCollect_dataBuffer = _pDataBuffer(pCollectingTail);
    DataBuffer* pOperand_dataBuffer = _pDataBuffer(pOperand);
    
    pCollect_dataBuffer->vNextLinkedBuffer(pOperand_dataBuffer);
    pOperand_dataBuffer->vPrevLinkedBuffer(pCollect_dataBuffer);
    
    pCollectingTail = pOperand;
  }
  else {
    pCollectingTail = pOperand;
  }

  m_pJob->pLogicUnit_job()->m_xpOperandC = pCollectingTail;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* OperandC[].get -- fetch operand                                            */
/*----------------------------------------------------------------------------*/
void Step::vOperandCGet(void)
{
  ENTRY();
  
  m_pDataStack->vPushPtr(*m_pJob->pLogicUnit_job()->m_xpOperandC);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* OperandC[].getAndClear -- fetch operand and set to nullptr                  */
/*----------------------------------------------------------------------------*/
void Step::vOperandCGetAndClear(void)
{
  ENTRY();
  
  m_pDataStack->vPushPtr(*m_pJob->pLogicUnit_job()->m_xpOperandC);
  
  m_pJob->pLogicUnit_job()->m_xpOperandC = nullptr;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* OperandC[].set -- set operand                                              */
/*----------------------------------------------------------------------------*/
void Step::vOperandCSet(void)
{
  ENTRY();
  
  Operand* pOperand = _pOperand(m_pDataStack->pPopPtr(__func__));
  
  ASSERT(! pOperand || m_pJob->pLogicUnit_job()->m_xpOperandC.bIsNull());
  
  m_pJob->pLogicUnit_job()->m_xpOperandC = _pOperand(pOperand);

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
