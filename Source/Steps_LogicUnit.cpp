/*============================================================================*/
/* Steps_LogicUnit.cpp -- LogicUnit step group                                */
/*============================================================================*/

#include "Steps.h"

extern DebugFlags gxDebugFlags;;

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Expr(void)
{
  vAddStep("LogicUnit[].compile"_Stz,   &Step::vLogicUnitCompile);
  vAddStep("LogicUnit[].newAggGby"_Stz, &Step::vLogicUnitNewAggGby);  //! TEMP : to go
  vAddStep("LogicUnit[].newAggCpy"_Stz, &Step::vLogicUnitNewAggCpy);  //! TEMP : to go
  vAddStep("LogicUnit[].source@"_Stz,   &Step::vLogicUnitSourceFetch);
  vAddStep("LogicUnit[].source!"_Stz,   &Step::vLogicUnitSourceStore);
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

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

