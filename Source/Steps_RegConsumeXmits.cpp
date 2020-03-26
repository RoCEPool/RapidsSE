/*============================================================================*/
/* Steps_RegConsumeXmits.cpp -- Register ConsumeXmits step group              */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_RegConsumeXmits(void)
{
  vAddStep("ConsumeXmits[]"_Stz,            &Step::vRegConsumeXmitsGet);
  vAddStep("ConsumeXmits[].delete"_Stz,     &Step::vRegConsumeXmitsDelete);
  vAddStep("ConsumeXmits[].isNull"_Stz,     &Step::vRegConsumeXmitsIsNull);
}

/*----------------------------------------------------------------------------*/
/* ConsumeXmits[].delete -- operand delete                                    */
/*----------------------------------------------------------------------------*/
void Step::vRegConsumeXmitsDelete(void)
{
  ENTRY(); // ENTRY("vRegConsumeXmitsDelete"_Stz);
  
  ConsumeXmits* pConsumeXmits = _pConsumeXmits(*m_pRegSet->m_xpConsumeXmits);
  delete pConsumeXmits;
  
  m_pRegSet->m_xpOperandOoO = nullptr;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ConsumeXmits[].get -- fetch operand                                        */
/*----------------------------------------------------------------------------*/
void Step::vRegConsumeXmitsGet(void)
{
  ENTRY(); // ENTRY("vRegConsumeXmitsGet"_Stz);
  
  m_pDataStack->vPushPtr(*m_pRegSet->m_xpConsumeXmits);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ConsumeXmits[].isNull -- is operand null ?                                 */
/*----------------------------------------------------------------------------*/
void Step::vRegConsumeXmitsIsNull(void)
{
  ENTRY(); // ENTRY("vRegConsumeXmitsIsNull"_Stz);
  
  m_pDataStack->vPushInt(m_pRegSet->m_xpConsumeXmits.bIsNull() ? 1 : 0);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
