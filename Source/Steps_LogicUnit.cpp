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
  vAddStep("LogicUnit[].compile"_Stz,   &Step::vExprCompile);
  vAddStep("LogicUnit[].newAggGby"_Stz, &Step::vExprNewAggGby);  //! TEMP : to go
  vAddStep("LogicUnit[].newAggCpy"_Stz, &Step::vExprNewAggCpy);  //! TEMP : to go
  vAddStep("LogicUnit[].source@"_Stz,   &Step::vExprSourceFetch);
  vAddStep("LogicUnit[].source!"_Stz,   &Step::vExprSourceStore);
}

/*----------------------------------------------------------------------------*/
/* LogicUnit[].compile -- compile an expression                               */
/*----------------------------------------------------------------------------*/
void Step::vExprCompile(void)
{
  ENTRY();
  
  bool b = m_pRegSet->m_oCalculator.bCompile();
  
  m_pDataStack->vPushInt((i8)b);
    
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LogicUnit[].newAggCpy -- new a copy aggregate result expression            */
/*----------------------------------------------------------------------------*/
void Step::vExprNewAggCpy(void)
{
  ENTRY();
  
  m_pRegSet->m_oCalculator.vGenerateAggCpy();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LogicUnit[].newAggGby -- new a group by aggregate expression               */
/*----------------------------------------------------------------------------*/
void Step::vExprNewAggGby(void)
{
  ENTRY();
  
  m_pRegSet->m_oCalculator.vGenerateAggGby();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LogicUnit[].source@ -- fetch source for expression evaluator               */
/*----------------------------------------------------------------------------*/
void Step::vExprSourceFetch(void)
{
  ENTRY();
    
  s2K sSource = m_pRegSet->m_oCalculator.szSource();

  m_pDataStack->vPushStr(sSource);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* LogicUnit[].source! -- store source for expression evaluator               */
/*----------------------------------------------------------------------------*/
void Step::vExprSourceStore(void)
{
  ENTRY();
    
  s2K sSource;
  m_pDataStack->vPopStr(sSource,__func__);
  
  m_pRegSet->m_oCalculator.vSource(sSource);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

