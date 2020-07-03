/*============================================================================*/
/* Steps_ExprVec.cpp -- Expression Evaluator step group                        */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Expr(void)
{
  vAddStep("Expr[].compile"_Stz,   &Step::vExprCompile);
  vAddStep("Expr[].delete"_Stz,    &Step::vExprDelete);
  vAddStep("Expr[].new"_Stz,       &Step::vExprNew);
  vAddStep("Expr[].source@"_Stz,   &Step::vExprSourceFetch);
  vAddStep("Expr[].source!"_Stz,   &Step::vExprSourceStore);
}

/*----------------------------------------------------------------------------*/
/* Expr.compile -- compile an expression                                      */
/*----------------------------------------------------------------------------*/
void Step::vExprCompile(void)
{
  ENTRY();
  
  bool b = m_pRegSet->m_xpExpr->bCompile();
  
  m_pDataStack->vPushInt((i8)b);
    
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Expr.delete -- delete an expression evaluator                              */
/*----------------------------------------------------------------------------*/
void Step::vExprDelete(void)
{
  ENTRY();
    
  Expr* pExpr = * m_pRegSet->m_xpExpr;
  
  m_pRegSet->m_xpExpr = nullptr;
  
  delete pExpr;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Expr.new -- new an expression evaluator                                    */
/*----------------------------------------------------------------------------*/
void Step::vExprNew(void)
{
  ENTRY();
    
  m_pRegSet->m_xpExpr = new Expr;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Expr.source@ -- fetch source for expression evaluator                      */
/*----------------------------------------------------------------------------*/
void Step::vExprSourceFetch(void)
{
  ENTRY();
    
  s2K sSource = m_pRegSet->m_xpExpr->szSource();

  m_pDataStack->vPushStr(sSource);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Expr.source! -- store source for expression evaluator                      */
/*----------------------------------------------------------------------------*/
void Step::vExprSourceStore(void)
{
  ENTRY();
    
  s2K sSource;
  m_pDataStack->vPopStr(sSource,__func__);
  
  m_pRegSet->m_xpExpr->vSource(sSource);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

