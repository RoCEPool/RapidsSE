/*============================================================================*/
/* Steps_Subset.cpp -- Subset step group                                      */
/*============================================================================*/

#include "BASE.h"
#include "STORE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "StepTiming.hpp"


/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Subset(void)
{
  vAddStep("Subset[]"_Stz,            &Step::vSubsetGet);
  vAddStep("Subset[].next"_Stz,       &Step::vSubsetNext);
  vAddStep("Subset[].nextBuffer"_Stz, &Step::vSubsetNextBuffer);
  vAddStep("Subset[].set"_Stz,        &Step::vSubsetSet);
}

/*----------------------------------------------------------------------------*/
/* Subset[] -- get subset                                                     */
/*----------------------------------------------------------------------------*/
void Step::vSubsetGet(void)
{
  ENTRY();
  
  if (pLogicUnit_task()->m_xpSubset.bIsNull())
    m_pDataStack->vPushPtr(nullptr);
  else
    m_pDataStack->vPushPtr(*pLogicUnit_task()->m_xpSubset);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Subset[].next -- get next subset                                           */
/*----------------------------------------------------------------------------*/
void Step::vSubsetNext(void)
{
  ENTRY();
  
  Operand* pNextOperand = nullptr;
  
  if (*pLogicUnit_task()->m_xpSubset)
    pNextOperand = pLogicUnit_task()->m_xpSubset->pNextItem();
  
  m_pDataStack->vPushPtr(pNextOperand);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Subset[].nextBuffer -- get next subset buffer                              */
/*----------------------------------------------------------------------------*/
void Step::vSubsetNextBuffer(void)
{
  ENTRY();
  
  Operand* pNextOperand = nullptr;
  
  if (*pLogicUnit_task()->m_xpSubset) {
    if (pLogicUnit_task()->m_xpSubset->eClass() == CN_CRWBUFFER) {
      CrwBuffer* pCrwBuffer = _pCrwBuffer(*pLogicUnit_task()->m_xpSubset);
      pNextOperand = pCrwBuffer->pNextBuffer();
      //pLogicUnit_task()->m_xpSubset = pNextOperand;
    }
    else
      if (pLogicUnit_task()->m_xpSubset->eClass() == CN_CSVBUFFER) {
      CsvBuffer* pCsvBuffer = _pCsvBuffer(*pLogicUnit_task()->m_xpSubset);
      pNextOperand = pCsvBuffer->pNextBuffer();
      //pLogicUnit_task()->m_xpSubset = pNextOperand;
    }
    else {
      vTrace("Subset class %s\n"_Stz, aCN[pLogicUnit_task()->m_xpSubset->eClass()]);
      ASSERT(0);
    }
  }
  
  m_pDataStack->vPushPtr(pNextOperand);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Subset[].set -- set operand                                               */
/*----------------------------------------------------------------------------*/
void Step::vSubsetSet(void)
{
  ENTRY();
  
  Operand* pOperand = _pOperand(m_pDataStack->pPopPtr(__func__));
  
  if (pOperand && CN_CRWBUFFER == pOperand->eClass()) {
    CrwBuffer* pCrwBuffer = _pCrwBuffer(pOperand);
    vDebugPoint();
  }
  
  pLogicUnit_task()->m_xpSubset = pOperand;
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
