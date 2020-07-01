/*============================================================================*/
/* Steps_RegSet.cpp - reg set step group                                      */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_RegSet(void)
{
  vAddStep("regset::callAgain@"_Stz,   &Step::vRegSetCallAgainFetch);
  vAddStep("regset::callAgain!"_Stz,   &Step::vRegSetCallAgainStore);
  vAddStep("regset::handler@"_Stz,     &Step::vRegSetHandlerFetch);
  vAddStep("regset::handler!"_Stz,     &Step::vRegSetHandlerStore);
}

/*----------------------------------------------------------------------------*/
/* regset::callAgain@ --                                                      */
/*----------------------------------------------------------------------------*/
void Step::vRegSetCallAgainFetch(void)
{
  ENTRY();
  
  m_pDataStack->vPushInt(m_pRegSet->m_bCallAgain);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* regset::callAgain! --                                                      */
/*----------------------------------------------------------------------------*/
void Step::vRegSetCallAgainStore(void)
{
  ENTRY();

  i8 iCallAgain = m_pDataStack->iPopInt(__func__);
  
  m_pRegSet->m_bCallAgain = (bool)iCallAgain;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* regset::handler@ --                                                        */
/*----------------------------------------------------------------------------*/
void Step::vRegSetHandlerFetch(void)
{
  ENTRY();
  
  m_pDataStack->vPushPtr(*m_pRegSet->m_xpHandler);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* regset::handler! --                                                        */
/*----------------------------------------------------------------------------*/
void Step::vRegSetHandlerStore(void)
{
  ENTRY();

  Word* pHandler = _pWord(m_pDataStack->pPopPtr(__func__));
  
  m_pRegSet->m_xpHandler = pHandler;
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
