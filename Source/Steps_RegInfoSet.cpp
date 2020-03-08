/*============================================================================*/
/* Steps_RegInfoSet.cpp -- Register InfoSet step group                        */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_RegInfoSet(void)
{
  vAddStep("InfoSet[].newInfoSet"_Stz,     &Step::vRegInfoSetNewInfoSet);
  vAddStep("InfoSet[].set"_Stz,            &Step::vRegInfoSetSet);
}

/*----------------------------------------------------------------------------*/
/* InfoSet[].newInfoSet -- new an infoset operand                             */
/*----------------------------------------------------------------------------*/
void Step::vRegInfoSetNewInfoSet(void)
{
  ENTRY(); // ENTRY("vRegInfoSetNewInfoSet"_Stz);
  
  ASSERT (m_pRegSet->m_xpInfoSet.bIsNull());
  
  m_pRegSet->m_xpInfoSet = new InfoSet();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* InfoSet[].set -- set InfoSet                                               */
/*----------------------------------------------------------------------------*/
void Step::vRegInfoSetSet(void)
{
  ENTRY(); // ENTRY("vRegInfoSetSet"_Stz);
  
  Operand* pOperand = _pOperand(m_pDataStack->pPopPtr(__func__));
  ASSERT(! pOperand || m_pRegSet->m_xpInfoSet.bIsNull());
  m_pRegSet->m_xpInfoSet = _pOperand(pOperand);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
