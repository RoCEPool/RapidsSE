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
  vAddStep("LogicUnit[].compile"_Stz,    &Step::vLogicUnitCompile);
  vAddStep("LogicUnit[].newAggGby"_Stz,  &Step::vLogicUnitNewAggGby);  //! TEMP : to go
  vAddStep("LogicUnit[].newAggCpy"_Stz,  &Step::vLogicUnitNewAggCpy);  //! TEMP : to go
  vAddStep("LogicUnit[].source@"_Stz,    &Step::vLogicUnitSourceFetch);
  vAddStep("LogicUnit[].source!"_Stz,    &Step::vLogicUnitSourceStore);
  
  vAddStep("LogicUnit[].callAgain@"_Stz, &Step::vLogicUnitCallAgainFetch);
  vAddStep("LogicUnit[].callAgain!"_Stz, &Step::vLogicUnitCallAgainStore);
  vAddStep("LogicUnit[].handler@"_Stz,   &Step::vLogicUnitHandlerFetch);
  vAddStep("LogicUnit[].handler!"_Stz,   &Step::vLogicUnitHandlerStore);
  
  vAddStep("LogicUnit[].filter"_Stz,     &Step::vLogicUnitFilter);
}

/*----------------------------------------------------------------------------*/
/* LogicUnit[].compile -- compile an expression                               */
/*----------------------------------------------------------------------------*/
void Step::vLogicUnitCompile(void)
{
  ENTRY();
  
  bool b = m_pLogicUnit->bCompile();
  
  m_pDataStack->vPushInt((i8)b);
    
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LogicUnit[].newAggCpy -- new a copy aggregate result expression            */
/*----------------------------------------------------------------------------*/
void Step::vLogicUnitNewAggCpy(void)
{
  ENTRY();
  
  m_pLogicUnit->vGenerateAggCpy();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LogicUnit[].newAggGby -- new a group by aggregate expression               */
/*----------------------------------------------------------------------------*/
void Step::vLogicUnitNewAggGby(void)
{
  ENTRY();
  
  m_pLogicUnit->vGenerateAggGby();
  
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
/* LogicUnit[].filter -- filter an rsb buffer                                 */
/*----------------------------------------------------------------------------*/
void Step::vLogicUnitFilter(void)
{
  ENTRY();

  RsbBuffer* pRsbBuffer = _pRsbBuffer(* m_pLogicUnit->m_xpOperandA);
  
  i8 bCopy = m_pLogicUnit->iFilter(pRsbBuffer->pRowSlab());

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
