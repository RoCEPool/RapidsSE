/*============================================================================*/
/* Steps_ExprEval.cpp -- Expression Evaluator step group                      */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "Node_I.hpp"
#include "Job.hpp"
#include "LogicUnit.hpp"
#include "IndexTree.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_ExprEval(void)
{
  vAddStep("exprEval[]"_Stz,                 &Step::vExprEvalGet);
  vAddStep("exprEval[].delete"_Stz,          &Step::vExprEvalDelete);
  vAddStep("exprEval[].evaluate"_Stz,        &Step::vExprEvalEvaluate);
  vAddStep("exprEval[].evaluateIndexScan"_Stz, &Step::vExprEvalEvaluateIndexScan);
  vAddStep("exprEval[].new"_Stz,             &Step::vExprEvalNew);
  vAddStep("exprEval[].set"_Stz,             &Step::vExprEvalSet);
}

/*----------------------------------------------------------------------------*/
/* exprEval[] -- return exprEval[]                                            */
/*----------------------------------------------------------------------------*/
void Step::vExprEvalGet(void)
{
  ENTRY();

  m_pDataStack->vPushPtr(*m_pJob->pLogicUnit_job()->m_xpExprEval);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* exprEval[].delete -- delete the expr evaluator                             */
/*----------------------------------------------------------------------------*/
void Step::vExprEvalDelete(void)
{
  ENTRY();

  ExprEval* pExprEval = *m_pJob->pLogicUnit_job()->m_xpExprEval;
  m_pJob->pLogicUnit_job()->m_xpExprEval = nullptr;
  delete pExprEval;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* exprEval[].evaluate -- evaluate expression with column ordinals            */
/*----------------------------------------------------------------------------*/
void Step::vExprEvalEvaluate(void)
{
  ENTRY();

  ExprEval* pExprEval = *m_pJob->pLogicUnit_job()->m_xpExprEval;

  StreamDef* pStreamDefOut = _pStreamDef(m_pDataStack->pPopPtr(__func__));
  StreamDef* pStreamDefIn = _pStreamDef(m_pDataStack->pPopPtr(__func__));

  s4K sProjectColumns;
  m_pDataStack->vPopStr(sProjectColumns, __func__);

  eEC eError = pExprEval->eEvaluate(pStreamDefIn, pStreamDefOut, sProjectColumns);

  m_pDataStack->vPushInt(eError);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* exprEval[].evaluateIndexScan -- evaluate expression for index scan         */
/*----------------------------------------------------------------------------*/
void Step::vExprEvalEvaluateIndexScan(void)
{
  ENTRY();

  ExprEval* pExprEval = *m_pJob->pLogicUnit_job()->m_xpExprEval;

  IndexTree* pIndexTree = _pIndexTree(m_pDataStack->pPopPtr(__func__));

  s4K sIndexRange;
  m_pDataStack->vPopStr(sIndexRange, __func__);

  eEC eError = pExprEval->eEvaluateIndexScan(pIndexTree->pIndexDef(), sIndexRange);

  m_pDataStack->vPushInt(eError);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* exprEval[].new -- new an expr evaluator                                    */
/*----------------------------------------------------------------------------*/
void Step::vExprEvalNew(void)
{
  ENTRY();

  m_pJob->pLogicUnit_job()->m_xpExprEval = new ExprEval;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* exprEval[].set -- set exprEval[]                                           */
/*----------------------------------------------------------------------------*/
void Step::vExprEvalSet(void)
{
  ENTRY();

  m_pJob->pLogicUnit_job()->m_xpExprEval = _pExprEval(m_pDataStack->pPopPtr(__func__));

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
