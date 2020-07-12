/*============================================================================*/
/* Steps_LogicUnit.cpp - Logic Unit step group                                */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_LogicUnit(void)
{
  vAddStep("regset::callAgain@"_Stz,   &Step::vLogicUnitCallAgainFetch);
  vAddStep("regset::callAgain!"_Stz,   &Step::vLogicUnitCallAgainStore);
  vAddStep("regset::handler@"_Stz,     &Step::vLogicUnitHandlerFetch);
  vAddStep("regset::handler!"_Stz,     &Step::vLogicUnitHandlerStore);
}

/*----------------------------------------------------------------------------*/
/* regset::callAgain@ --                                                      */
/*----------------------------------------------------------------------------*/
void Step::vLogicUnitCallAgainFetch(void)
{
  ENTRY();
  
  m_pDataStack->vPushInt(m_pLogicUnit->m_bCallAgain);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* regset::callAgain! --                                                      */
/*----------------------------------------------------------------------------*/
void Step::vLogicUnitCallAgainStore(void)
{
  ENTRY();

  i8 iCallAgain = m_pDataStack->iPopInt(__func__);
  
  m_pLogicUnit->m_bCallAgain = (bool)iCallAgain;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* regset::handler@ --                                                        */
/*----------------------------------------------------------------------------*/
void Step::vLogicUnitHandlerFetch(void)
{
  ENTRY();
  
  m_pDataStack->vPushPtr(*m_pLogicUnit->m_xpHandler);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* regset::handler! --                                                        */
/*----------------------------------------------------------------------------*/
void Step::vLogicUnitHandlerStore(void)
{
  ENTRY();

  Word* pHandler = _pWord(m_pDataStack->pPopPtr(__func__));
  
  m_pLogicUnit->m_xpHandler = pHandler;
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
