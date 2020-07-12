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
  vAddStep("Expr[].compile"_Stz,   &Step::vExprCompile);
  vAddStep("Expr[].newAggGby"_Stz, &Step::vExprNewAggGby);  //! TEMP : to go
  vAddStep("Expr[].newAggCpy"_Stz, &Step::vExprNewAggCpy);  //! TEMP : to go
  vAddStep("Expr[].source@"_Stz,   &Step::vExprSourceFetch);
  vAddStep("Expr[].source!"_Stz,   &Step::vExprSourceStore);
}

/*----------------------------------------------------------------------------*/
/* Expr[].compile -- compile an expression                                    */
/*----------------------------------------------------------------------------*/
void Step::vExprCompile(void)
{
  ENTRY();
  
  bool b = m_pRegSet->m_oCalculator.bCompile();
  
  m_pDataStack->vPushInt((i8)b);
    
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Expr[].newAggCpy -- new a copy aggregate result expression                 */
/*----------------------------------------------------------------------------*/
void Step::vExprNewAggCpy(void)
{
  ENTRY();
  
  m_pRegSet->m_oCalculator.vGenerateAggCpy();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Expr[].newAggGby -- new a group by aggregate expression                    */
/*----------------------------------------------------------------------------*/
void Step::vExprNewAggGby(void)
{
  ENTRY();
  
  m_pRegSet->m_oCalculator.vGenerateAggGby();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Expr[].source@ -- fetch source for expression evaluator                    */
/*----------------------------------------------------------------------------*/
void Step::vExprSourceFetch(void)
{
  ENTRY();
    
  s2K sSource = m_pRegSet->m_oCalculator.szSource();

  m_pDataStack->vPushStr(sSource);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Expr[].source! -- store source for expression evaluator                    */
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

