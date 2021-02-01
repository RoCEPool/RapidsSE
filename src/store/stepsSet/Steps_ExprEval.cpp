/*============================================================================*/
/* Steps_ExprEval.cpp -- Expression Evaluator step group                      */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "StepTiming.hpp"

#include "Node_I.hpp"
#include "Response.hpp"
#include "Result.hpp"
#include "Job.hpp"
#include "LogicUnit.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_ExprEval(void)
{
  vAddStep("exprEval[]"_Stz,                 &Step::vExprEvalGet);
  vAddStep("exprEval[].delete"_Stz,          &Step::vExprEvalDelete);
  vAddStep("exprEval[].aggregate"_Stz,       &Step::vExprEvalAggregate);
  vAddStep("exprEval[].filter"_Stz,          &Step::vExprEvalFilter);
  vAddStep("exprEval[].new"_Stz,             &Step::vExprEvalNew);
  vAddStep("exprEval[].project"_Stz,         &Step::vExprEvalProject);
  vAddStep("exprEval[].set"_Stz,             &Step::vExprEvalSet);
}

/*----------------------------------------------------------------------------*/
/* exprEval[] -- return exprEval[]                                            */
/*----------------------------------------------------------------------------*/
void Step::vExprEvalGet(void)
{
  ENTRY();

  m_pDataStack->vPushPtr(*( m_pJob->pLogicUnit_job() )->m_xpExprEval);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* exprEval[].delete -- delete the expr evaluator                             */
/*----------------------------------------------------------------------------*/
void Step::vExprEvalDelete(void)
{
  ENTRY();

  ExprEval* pExprEval = *( m_pJob->pLogicUnit_job() )->m_xpExprEval;
  ( m_pJob->pLogicUnit_job() )->m_xpExprEval = nullptr;
  delete pExprEval;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* exprEval[].aggregate -- evaluate aggregate                                 */
/*----------------------------------------------------------------------------*/
void Step::vExprEvalAggregate(void)
{
  ENTRY();

  ExprEval* pExprEval = *( m_pJob->pLogicUnit_job() )->m_xpExprEval;

  StreamDef* pStreamDefOut = _pStreamDef(m_pDataStack->pPopPtr(__func__));
  StreamDef* pStreamDefIn = _pStreamDef(m_pDataStack->pPopPtr(__func__));

  s4K sProjectColumns;
  m_pDataStack->vPopStr(sProjectColumns, __func__);

  eEC eError = pExprEval->eEvaluate(pStreamDefIn, pStreamDefOut, sProjectColumns);

  if ( ! pExprEval->bHasAggregate()  && ! pExprEval->bHasGroupBy() )
    eError = EC_INVALID_COMMAND;

  m_pDataStack->vPushInt(eError);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* exprEval[].filter -- evaluate filter                                       */
/*----------------------------------------------------------------------------*/
void Step::vExprEvalFilter(void)
{
  ENTRY();

  ExprEval* pExprEval = *( m_pJob->pLogicUnit_job() )->m_xpExprEval;

  StreamDef* pStreamDefOut = _pStreamDef(m_pDataStack->pPopPtr(__func__));
  StreamDef* pStreamDefIn = _pStreamDef(m_pDataStack->pPopPtr(__func__));

  s4K sProjectColumns;
  m_pDataStack->vPopStr(sProjectColumns, __func__);

  eEC eError = pExprEval->eEvaluate(pStreamDefIn, pStreamDefOut, sProjectColumns);

  if (pExprEval->bHasAggregate())
    eError = EC_INVALID_COMMAND;

  m_pDataStack->vPushInt(eError);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* exprEval[].new -- new an expr evaluator                                    */
/*----------------------------------------------------------------------------*/
void Step::vExprEvalNew(void)
{
  ENTRY();

  ( m_pJob->pLogicUnit_job() )->m_xpExprEval = new ExprEval;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* exprEval[].project -- evaluate project columns                             */
/*----------------------------------------------------------------------------*/
void Step::vExprEvalProject(void)
{
  ENTRY();

  ExprEval* pExprEval = *( m_pJob->pLogicUnit_job() )->m_xpExprEval;

  StreamDef* pStreamDefOut = _pStreamDef(m_pDataStack->pPopPtr(__func__));
  StreamDef* pStreamDefIn = _pStreamDef(m_pDataStack->pPopPtr(__func__));

  s4K sProjectColumns;
  m_pDataStack->vPopStr(sProjectColumns, __func__);

  eEC eError = pExprEval->eEvaluate(pStreamDefIn, pStreamDefOut, sProjectColumns);

  if (pExprEval->bHasAggregate())
    eError = EC_INVALID_COMMAND;

  m_pDataStack->vPushInt(eError);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* exprEval[].set -- set exprEval[]                                           */
/*----------------------------------------------------------------------------*/
void Step::vExprEvalSet(void)
{
  ENTRY();

  ( m_pJob->pLogicUnit_job() )->m_xpExprEval = _pExprEval(m_pDataStack->pPopPtr(__func__));

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
