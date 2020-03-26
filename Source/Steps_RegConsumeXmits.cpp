/*============================================================================*/
/* Steps_RegConsumeXmits.cpp -- Register ConsumeXmits step group              */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_RegConsumeXmits(void)
{
  vAddStep("ConsumeXmits[]"_Stz,          &Step::vRegConsumeXmitsGet);
  vAddStep("ConsumeXmits[].cancel"_Stz,   &Step::vRegConsumeXmitsCancel);
  vAddStep("ConsumeXmits[].delete"_Stz,   &Step::vRegConsumeXmitsDelete);
  vAddStep("ConsumeXmits[].exit"_Stz,     &Step::vRegConsumeXmitsExit);
  vAddStep("ConsumeXmits[].isNull"_Stz,   &Step::vRegConsumeXmitsIsNull);
  vAddStep("ConsumeXmits[].new"_Stz,      &Step::vRegConsumeXmitsNew);
}

/*----------------------------------------------------------------------------*/
/* ConsumeXmits[].cancel -- handle consume of cancel marker                   */
/*----------------------------------------------------------------------------*/
void Step::vRegConsumeXmitsCancel(void)
{
  ENTRY(); // ENTRY("vRegConsumeXmitsCancel"_Stz);
  
  ConsumeXmits* pConsumeXmits = _pConsumeXmits(*m_pRegSet->m_xpConsumeXmits);
 
  pConsumeXmits->vIncExit();
  
  m_pDataStack->vPushInt(pConsumeXmits->bExitDone());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ConsumeXmits[].delete -- operand delete                                    */
/*----------------------------------------------------------------------------*/
void Step::vRegConsumeXmitsDelete(void)
{
  ENTRY(); // ENTRY("vRegConsumeXmitsDelete"_Stz);
  
  ConsumeXmits* pConsumeXmits = _pConsumeXmits(*m_pRegSet->m_xpConsumeXmits);
  delete pConsumeXmits;
  
  m_pRegSet->m_xpConsumeXmits = nullptr;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ConsumeXmits[].exit -- handle consume of exit marker                       */
/*----------------------------------------------------------------------------*/
void Step::vRegConsumeXmitsExit(void)
{
  ENTRY(); // ENTRY("vRegConsumeXmitsExit"_Stz);
  
  ConsumeXmits* pConsumeXmits = _pConsumeXmits(*m_pRegSet->m_xpConsumeXmits);
  
  pConsumeXmits->vIncExit();
  
  m_pDataStack->vPushInt(pConsumeXmits->bExitDone());
  
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

/*----------------------------------------------------------------------------*/
/* ConsumeXmits[].new -- new a consume xmits                                  */
/*----------------------------------------------------------------------------*/
void Step::vRegConsumeXmitsNew(void)
{
  ENTRY(); // ENTRY("vRegConsumeXmitsNew"_Stz);
  
  ASSERT (m_pRegSet->m_xpConsumeXmits.bIsNull());
  
  Op* pOp = m_pTask->m_pJob->m_pOp;
  m_pRegSet->m_xpConsumeXmits = new ConsumeXmits(pOp->nLeftEosMarkers());
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
