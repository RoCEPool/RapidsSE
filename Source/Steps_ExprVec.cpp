/*============================================================================*/
/* Steps_ExprVec.cpp -- Expression Evaluation Vector step group               */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_ExprVec(void)
{
  vAddStep("ExprVec[]"_Stz,         &Step::vExprVecGet);
  vAddStep("ExprVec[].delete"_Stz,  &Step::vExprVecDelete);
  vAddStep("ExprVec[].evalAll"_Stz, &Step::vExprVecEvalAll);
  vAddStep("ExprVec[].evalOne"_Stz, &Step::vExprVecEvalOne);
  vAddStep("ExprVec[].get"_Stz,     &Step::vExprVecGet);
  vAddStep("ExprVec[].new"_Stz,     &Step::vExprVecNew);
}

/*----------------------------------------------------------------------------*/
/* ExprVec[].delete -- delete exprVec                                         */
/*----------------------------------------------------------------------------*/
void Step::vExprVecDelete(void)
{
  ENTRY();
    
  ExprVec* pExprVev = *m_pRegSet->m_xpExprVec;
  
  m_pRegSet->m_xpExprVec = nullptr;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ExprVec[].evalAll -- evaluate all expressions                              */
/*----------------------------------------------------------------------------*/
void Step::vExprVecEvalAll(void)
{
  ENTRY();
    
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ExprVec[].evalOne -- evaluate one expression                               */
/*----------------------------------------------------------------------------*/
void Step::vExprVecEvalOne(void)
{
  ENTRY();
    
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ExprVec[].get -- get exprVec                                               */
/*----------------------------------------------------------------------------*/
void Step::vExprVecGet(void)
{
  ENTRY();
    
  m_pDataStack->vPushPtr(*m_pRegSet->m_xpExprVec);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task[].new -- new an expression evaluation vector                          */
/*----------------------------------------------------------------------------*/
void Step::vExprVecNew(void)
{
  ENTRY();
  
  m_pRegSet->m_xpExprVec = new ExprVec;
    
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

