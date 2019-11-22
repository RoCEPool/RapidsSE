/*============================================================================*/
/* Steps_Subset.cpp -- Subset step group                                      */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Subset(void)
{
  vAddStep("Subset[]@"_Stz,          &Step::vSubsetFetch);
  vAddStep("Subset[].next"_Stz,      &Step::vSubsetNext);
  vAddStep("Subset[].set"_Stz,       &Step::vSubsetSet);
}

/*----------------------------------------------------------------------------*/
/* Subset[]@ -- fetch subset                                                  */
/*----------------------------------------------------------------------------*/
void Step::vSubsetFetch(void)
{
  ENTRY("vSubsetNext"_Stz);
  
  m_pDataStack->vPushPointer(*m_pRegSet->m_xpSubset);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Subset[].next -- get next subset                                           */
/*----------------------------------------------------------------------------*/
void Step::vSubsetNext(void)
{
  ENTRY("vSubsetNext"_Stz);
  
  Operand* pNextOperand = nullptr;
  
  if (*m_pRegSet->m_xpSubset)
    pNextOperand = m_pRegSet->m_xpSubset->pNextItem();
  
  m_pDataStack->vPushPointer(pNextOperand);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Subset[].set -- set operand                                               */
/*----------------------------------------------------------------------------*/
void Step::vSubsetSet(void)
{
  ENTRY("vOperandSet"_Stz);
  
  Object* pPtrObject;
  if ( ! m_pDataStack->bPopObject(pPtrObject) )
    THROW_DSTACK();
  
  m_pRegSet->m_xpSubset = _pOperand(pPtrObject);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
