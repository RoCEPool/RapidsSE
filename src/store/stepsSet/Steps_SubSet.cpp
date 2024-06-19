/*============================================================================*/
/* Steps_Subset.cpp -- Subset step group                                      */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "RowCoder.hpp"
#include "HeadLine.hpp"
#include "Crw.hpp"
#include "CrwPacker.hpp"
#include "CrwBuffer.hpp"
#include "Job.hpp"
#include "Node_I.hpp"
#include "LogicUnit.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Subset(void)
{
  vAddStep("Subset[]"_Stz,            &Step::vSubsetGet);
  vAddStep("Subset[].next"_Stz,       &Step::vSubsetNext);
  vAddStep("Subset[].set"_Stz,        &Step::vSubsetSet);
}

/*----------------------------------------------------------------------------*/
/* Subset[] -- get subset                                                     */
/*----------------------------------------------------------------------------*/
void Step::vSubsetGet(void)
{
  ENTRY();
  
  if (m_pLogicUnit->m_xpSubset.bIsNull())
    m_oDataStack.vPushPtr(nullptr);
  else
    m_oDataStack.vPushPtr(*m_pLogicUnit->m_xpSubset);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Subset[].next -- get next subset                                           */
/*----------------------------------------------------------------------------*/
void Step::vSubsetNext(void)
{
  ENTRY();
  
  Operand* pNextOperand = nullptr;
  
  if (*m_pLogicUnit->m_xpSubset)
    pNextOperand = m_pLogicUnit->m_xpSubset->pNextItem();
  
  m_oDataStack.vPushPtr(pNextOperand);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Subset[].set -- set operand                                               */
/*----------------------------------------------------------------------------*/
void Step::vSubsetSet(void)
{
  ENTRY();
  
  Operand* pOperand = _pOperand(m_oDataStack.pPopPtr(__func__));
  
  if (pOperand && CN_CRWBUFFER == pOperand->eGetClassNo()) {
    CrwBuffer* pCrwBuffer = _pCrwBuffer(pOperand);
    UNUSED(pCrwBuffer);
  }
  
  m_pLogicUnit->m_xpSubset = pOperand;
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
