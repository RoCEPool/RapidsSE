/*============================================================================*/
/* Steps_LogicUnit.cpp -- LogicUnit step group                                */
/*============================================================================*/

#include "Steps.h"

extern DebugFlags gxDebugFlags;;

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_LogicUnit(void)
{
  vAddStep("LogicUnit[].genCode"_Stz,    &Step::vLogicUnitGenCode);
  
  vAddStep("LogicUnit[].source@"_Stz,    &Step::vLogicUnitSourceFetch);
  vAddStep("LogicUnit[].source!"_Stz,    &Step::vLogicUnitSourceStore);
  
  vAddStep("LogicUnit[].callAgain@"_Stz, &Step::vLogicUnitCallAgainFetch);
  vAddStep("LogicUnit[].callAgain!"_Stz, &Step::vLogicUnitCallAgainStore);
  vAddStep("LogicUnit[].handler@"_Stz,   &Step::vLogicUnitHandlerFetch);
  vAddStep("LogicUnit[].handler!"_Stz,   &Step::vLogicUnitHandlerStore);
  
  vAddStep("LogicUnit[].transform"_Stz,  &Step::vLogicUnitTransform);
}

/*----------------------------------------------------------------------------*/
/* LogicUnit[].genCode -- generate code for given test                        */
/*----------------------------------------------------------------------------*/
void Step::vLogicUnitGenCode(void)
{
  ENTRY();
  
  i8 iTestNo = m_pDataStack->iPopInt(__func__);
      
  m_pLogicUnit->vGenCode(iTestNo);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LogicUnit[].source@ -- fetch source for expression evaluator               */
/*----------------------------------------------------------------------------*/
void Step::vLogicUnitSourceFetch(void)
{
  ENTRY();
    
  s2K sSource = m_pLogicUnit->szSource();

  m_pDataStack->vPushStr(sSource);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LogicUnit[].source! -- store source for expression evaluator               */
/*----------------------------------------------------------------------------*/
void Step::vLogicUnitSourceStore(void)
{
  ENTRY();
    
  s2K sSource;
  m_pDataStack->vPopStr(sSource,__func__);
  
  m_pLogicUnit->vSource(sSource);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LogicUnit[].callAgain@ --                                                  */
/*----------------------------------------------------------------------------*/
void Step::vLogicUnitCallAgainFetch(void)
{
  ENTRY();
  
  m_pDataStack->vPushInt(m_pLogicUnit->m_bCallAgain);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LogicUnit[].callAgain! --                                                  */
/*----------------------------------------------------------------------------*/
void Step::vLogicUnitCallAgainStore(void)
{
  ENTRY();

  i8 iCallAgain = m_pDataStack->iPopInt(__func__);
  
  m_pLogicUnit->m_bCallAgain = (bool)iCallAgain;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LogicUnit[].handler@ --                                                    */
/*----------------------------------------------------------------------------*/
void Step::vLogicUnitHandlerFetch(void)
{
  ENTRY();
  
  m_pDataStack->vPushPtr(*m_pLogicUnit->m_xpHandler);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LogicUnit[].handler! --                                                    */
/*----------------------------------------------------------------------------*/
void Step::vLogicUnitHandlerStore(void)
{
  ENTRY();

  Word* pHandler = _pWord(m_pDataStack->pPopPtr(__func__));
  
  m_pLogicUnit->m_xpHandler = pHandler;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LogicUnit[].transform -- transform                                         */
/*----------------------------------------------------------------------------*/
void Step::vLogicUnitTransform(void)
{
  ENTRY();
  
  i8 bResult = m_pLogicUnit->iTransform();
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
