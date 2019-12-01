/*============================================================================*/
/* Steps_Subset.cpp -- Subset step group                                      */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Subset(void)
{
  vAddStep("Subset[]@"_Stz,           &Step::vSubsetFetch);
  vAddStep("Subset[].next"_Stz,       &Step::vSubsetNext);
  vAddStep("Subset[].nextBuffer"_Stz, &Step::vSubsetNextBuffer);
  vAddStep("Subset[].set"_Stz,        &Step::vSubsetSet);
}

/*----------------------------------------------------------------------------*/
/* Subset[]@ -- fetch subset                                                  */
/*----------------------------------------------------------------------------*/
void Step::vSubsetFetch(void)
{
  ENTRY(); // ENTRY("vSubsetNext"_Stz);
  
  m_pDataStack->vPushPtr(*m_pRegSet->m_xpSubset);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Subset[].next -- get next subset                                           */
/*----------------------------------------------------------------------------*/
void Step::vSubsetNext(void)
{
  ENTRY(); // ENTRY("vSubsetNext"_Stz);
  
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
  ENTRY(); // ENTRY("vSubsetNext"_Stz);
  
  Operand* pNextOperand = nullptr;
  
  if (*m_pRegSet->m_xpSubset) {
    if (m_pRegSet->m_xpSubset->eClass() == CN_CRWBUFFER) {
      CrwBuffer* pCrwBuffer = _pCrwBuffer(*m_pRegSet->m_xpSubset);
      pNextOperand = pCrwBuffer->pNextBuffer();
      m_pRegSet->m_xpSubset = pNextOperand;
    }
    else
      if (m_pRegSet->m_xpSubset->eClass() == CN_CSVBUFFER) {
      CsvBuffer* pCsvBuffer = _pCsvBuffer(*m_pRegSet->m_xpSubset);
      pNextOperand = pCsvBuffer->pNextBuffer();
      m_pRegSet->m_xpSubset = pNextOperand;
    }
    else {
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
  ENTRY(); // ENTRY("vOperandSet"_Stz);
  
  Object* pPtrObject;
  m_pDataStack->vPopPtr(pPtrObject,__func__);
  
  m_pRegSet->m_xpSubset = _pOperand(pPtrObject);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
