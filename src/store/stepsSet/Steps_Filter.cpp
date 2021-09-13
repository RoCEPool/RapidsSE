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
#include "StreamDefRht.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Filter(void)
{
  vAddStep("Filter[]"_Stz,                 &Step::vFilterGet);
  vAddStep("Filter[].delete"_Stz,          &Step::vFilterDelete);
  vAddStep("Filter[].evaluate"_Stz,        &Step::vFilterEvaluate);
  vAddStep("Filter[].evaluateIndexScan"_Stz, &Step::vFilterEvaluateIndexScan);
  vAddStep("Filter[].evaluateIndexJoin"_Stz, &Step::vFilterEvaluateIndexJoin);
  vAddStep("Filter[].evaluateLoopJoin"_Stz,  &Step::vFilterEvaluateLoopJoin);
  vAddStep("Filter[].new"_Stz,             &Step::vFilterNew);
  vAddStep("Filter[].set"_Stz,             &Step::vFilterSet);
}

/*----------------------------------------------------------------------------*/
/* Filter[] -- return Filter[]                                            */
/*----------------------------------------------------------------------------*/
void Step::vFilterGet(void)
{
  ENTRY();

  m_pDataStack->vPushPtr(*m_p_TaskLogicUnit->m_xpFilter);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Filter[].delete -- delete the expr evaluator                               */
/*----------------------------------------------------------------------------*/
void Step::vFilterDelete(void)
{
  ENTRY();

  ExprEval* pExprEval = *m_p_TaskLogicUnit->m_xpFilter;
  m_p_TaskLogicUnit->m_xpFilter = nullptr;
  delete pExprEval;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Filter[].evaluate -- evaluate expression with column ordinals              */
/*----------------------------------------------------------------------------*/
void Step::vFilterEvaluate(void)
{
  ENTRY();

  ExprEval* pExprEval = *m_p_TaskLogicUnit->m_xpFilter;

  StreamDef* pStreamDefOut = _pStreamDef(m_pDataStack->pPopPtr(__func__));
  StreamDef* pStreamDefIn = _pStreamDef(m_pDataStack->pPopPtr(__func__));

  s4K sProjectColumns;
  m_pDataStack->vPopStr(sProjectColumns, __func__);

  eEC eError = pExprEval->eEvaluate(pStreamDefIn, pStreamDefOut, sProjectColumns);

  m_pDataStack->vPushInt(eError);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Filter[].evaluateIndexJoin -- evaluate expression for IndexJoin          */
/*----------------------------------------------------------------------------*/
void Step::vFilterEvaluateIndexJoin(void)
{
  ENTRY();


  ExprEval* pExprEval = *m_p_TaskLogicUnit->m_xpFilter;

  IndexTree* pIndexTree = _pIndexTree(m_pDataStack->pPopPtr(__func__));
  StreamDef* pStreamDefOut = _pStreamDef(m_pDataStack->pPopPtr(__func__));
  StreamDef* pStreamDefRht = _pStreamDef(m_pDataStack->pPopPtr(__func__));
  StreamDef* pStreamDefLft = _pStreamDef(m_pDataStack->pPopPtr(__func__));

  s4K sJoinExpr;
  m_pDataStack->vPopStr(sJoinExpr, __func__);

  eEC eError = pExprEval->eEvaluateIndexJoin(pStreamDefLft, pStreamDefRht,pStreamDefOut,
                                             pIndexTree->pIndexDef(), sJoinExpr);

  m_pDataStack->vPushInt(eError);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Filter[].evaluateIndexScan -- evaluate expression for index scan         */
/*----------------------------------------------------------------------------*/
void Step::vFilterEvaluateIndexScan(void)
{
  ENTRY();

  ExprEval* pExprEval = *m_p_TaskLogicUnit->m_xpFilter;

  IndexTree* pIndexTree = _pIndexTree(m_pDataStack->pPopPtr(__func__));

  s4K sIndexRange;
  m_pDataStack->vPopStr(sIndexRange, __func__);

  eEC eError = pExprEval->eEvaluateIndexScan(pIndexTree->pIndexDef(), sIndexRange);

  m_pDataStack->vPushInt(eError);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Filter[].evaluateLoopJoin -- evaluate expression for loop join           */
/*----------------------------------------------------------------------------*/
void Step::vFilterEvaluateLoopJoin(void)
{
  ENTRY();


  ExprEval* pExprEval = *m_p_TaskLogicUnit->m_xpFilter;

  StreamDef*    pStreamDefOut = _pStreamDef(m_pDataStack->pPopPtr(__func__));
  StreamDefRht* pStreamDefRht = _pStreamDefRht(m_pDataStack->pPopPtr(__func__));
  StreamDef*    pStreamDefLft = _pStreamDef(m_pDataStack->pPopPtr(__func__));


  s4K sJoinExpr;
  m_pDataStack->vPopStr(sJoinExpr, __func__);

  eEC eError = pExprEval->eEvaluateLoopJoin(pStreamDefLft, pStreamDefRht, pStreamDefOut, sJoinExpr);

  m_pDataStack->vPushInt(eError);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Filter[].new -- new an expr evaluator                                    */
/*----------------------------------------------------------------------------*/
void Step::vFilterNew(void)
{
  ENTRY();

  m_p_TaskLogicUnit->m_xpFilter = new ExprEval;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Filter[].set -- set Filter[]                                           */
/*----------------------------------------------------------------------------*/
void Step::vFilterSet(void)
{
  ENTRY();

  m_p_TaskLogicUnit->m_xpFilter = _pExprEval(m_pDataStack->pPopPtr(__func__));

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
