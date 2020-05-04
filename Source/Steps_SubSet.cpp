/*============================================================================*/
/* Steps_Subset.cpp -- Subset step group                                      */
/*============================================================================*/

#include "Steps.h"

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
  
  if (m_pRegSet->m_xpSubset.bIsNull())
    m_pDataStack->vPushPtr(nullptr);
  else
    m_pDataStack->vPushPtr(*m_pRegSet->m_xpSubset);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Subset[].next -- get next subset                                           */
/*----------------------------------------------------------------------------*/
void Step::vSubsetNext(void)
{
  ENTRY();
  
  Operand* pNextOperand = nullptr;
  
  if (*m_pRegSet->m_xpSubset)
    pNextOperand = m_pRegSet->m_xpSubset->pNextItem();
  
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
  
  if (*m_pRegSet->m_xpSubset) {
    if (m_pRegSet->m_xpSubset->eClass() == CN_CRWBUFFER) {
      CrwBuffer* pCrwBuffer = _pCrwBuffer(*m_pRegSet->m_xpSubset);
      pNextOperand = pCrwBuffer->pNextBuffer();
      //m_pRegSet->m_xpSubset = pNextOperand;
    }
    else
      if (m_pRegSet->m_xpSubset->eClass() == CN_CSVBUFFER) {
      CsvBuffer* pCsvBuffer = _pCsvBuffer(*m_pRegSet->m_xpSubset);
      pNextOperand = pCsvBuffer->pNextBuffer();
      //m_pRegSet->m_xpSubset = pNextOperand;
    }
    else {
      vTrace("Subset class %s\n"_Stz, aCN[m_pRegSet->m_xpSubset->eClass()]);
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
  
  m_pRegSet->m_xpSubset = pOperand;
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
