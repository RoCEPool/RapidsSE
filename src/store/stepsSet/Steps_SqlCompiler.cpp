/*============================================================================*/
/* Steps_SqlCompiler.cpp - sql compiler step group                            */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"
#include "Grid.hpp"
#include "Node.hpp"

#include "InfoSet.hpp"

#include "SkipRoots.hpp"
#include "EntBuffer.hpp"
#include "IndexTree.hpp"
#include "TablePtn.hpp"
#include "Table.hpp"

#include "OpSpec.hpp"
#include "StageSpec.hpp"
#include "PipelineSpec.hpp"
#include "Pipeline.hpp"
#include "Backend.hpp"
#include "Job.hpp"
#include "Node_I.hpp"
#include "LogicUnit.hpp"

#include "PlanNode.hpp"
#include "RootNode.hpp"
#include "PlanDefinitionParser.hpp"
#include "SqlPlanner.hpp"
#include "ShadowRoot.hpp"
#include "ShadowPlan.hpp"
#include "PlanOperand.hpp"

#include "AggregateSplit.hpp"
#include "NameValidator.hpp"
#include "ExprValidator.hpp"
#include "SetOpValidator.hpp"
#include "PostProject.hpp"
#include "ExprPushdown.hpp"
#include "ProjValidation.hpp"
#include "CorrBySquash.hpp"
#include "JoinOptimisation.hpp"
#include "PartitionPlan.hpp"
#include "StageMarkers.hpp"
#include "MainSubquery.hpp"

#include "store/parser/shadowNodes/planObjects/planMarkers/statementMarkers/dml/ExplainMarker.hpp"
#include "ExplainOp.hpp"

#include "GraphValidator.hpp"

#include "SqlException.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_SqlCompiler(void)
{
  // Ancilliary steps used in the compilation pipeline
  vAddStep("sqlCompiler::deletePlan"_Stz,         &Step::vSqlCompilerDeletePlan);
  vAddStep("sqlCompiler::doExplain"_Stz,          &Step::vSqlCompilerDoExplain);
  vAddStep("sqlCompiler::doExplainTliterate"_Stz, &Step::vSqlCompilerDoExplainTliterate);
  vAddStep("sqlCompiler::explainFormat"_Stz,      &Step::vSqlCompilerExplainFormat);
  vAddStep("sqlCompiler::hasExplain"_Stz,         &Step::vSqlCompilerHasExplain);

  // Stages in the compilation pipeline (in alphabetical order)
  vAddStep("sqlCompiler::aggregateSplit"_Stz,     &Step::vSqlCompilerAggregateSplit);
  vAddStep("sqlCompiler::corrBySquash"_Stz,       &Step::vSqlCompilerCorrBySquash);
  vAddStep("sqlCompiler::explainPlan"_Stz,        &Step::vSqlCompilerExplainPlan);
  vAddStep("sqlCompiler::exprValidate"_Stz,       &Step::vSqlCompilerExprValidate);
  vAddStep("sqlCompiler::exprPushdown"_Stz,       &Step::vSqlCompilerExprPushdown);
  vAddStep("sqlCompiler::joinOptimisation"_Stz,   &Step::vSqlCompilerJoinOptimisation);
  vAddStep("sqlCompiler::nameValidate"_Stz,       &Step::vSqlCompilerNameValidate);
  vAddStep("sqlCompiler::parser"_Stz,             &Step::vSqlCompilerParser);
  vAddStep("sqlCompiler::partitionPlan"_Stz,      &Step::vSqlCompilerPartitionPlan);
  vAddStep("sqlCompiler::partitionExplain"_Stz,   &Step::vSqlCompilerPartitionExplain);
  vAddStep("sqlCompiler::postProject"_Stz,        &Step::vSqlCompilerPostProject);
  vAddStep("sqlCompiler::projValidation"_Stz,     &Step::vSqlCompilerProjValidation);
  vAddStep("sqlCompiler::setOpValidate"_Stz,      &Step::vSqlCompilerSetOpValidate);
  vAddStep("sqlCompiler::stageMarkers"_Stz,       &Step::vSqlCompilerStageMarkers);
  vAddStep("sqlCompiler::transliterate"_Stz,      &Step::vSqlCompilerTransliterate);
}

/*----------------------------------------------------------------------------*/
/* sqlCompiler::aggregateSplit -- Split ordinal expressions that have multiple*/
/* aggregate terms into child ProjectOps                                      */
/*----------------------------------------------------------------------------*/
void Step::vSqlCompilerAggregateSplit(void) {
  ENTRY();

  /**
   *  The purpose of this stage is to handle a limitation of the Relational Logic
   *  Unit and generally make the transliteration easier. The RLU currently cannot
   *  handle expressions that involve the calculation and combination of two
   *  aggregations. e.g. the ordinal expression "SUM($1) / COUNT($1)" involves
   *  calculating the two aggregates and then dividing one into the other.
   *
   *  This multi-stage approach needs to be simplified in a series of single stages.
   *  Since we know this will only occur in ProjectOps and AggregateOps (since other
   *  operators need to use the projected columns) we can do this by checking each
   *  ProjectOp & AggregateOp and adding a ProjectOp underneath them that contain
   *  the individual expression calculations.
   *
   *  e.g. for the example above we add a ProjectOp with two additional columns of
   *  SUM($1) and COUNT($1). The above ordinal expression is then implemented as
   *  $1 / $2, which is supported by the RLU and transliteration.
   */


  PlanOperand* pPlanOperand = _pPlanOperand(m_pDataStack->pPopPtr(__func__));
  Parser::RootNode* pPlanRoot = Parser::_pRootNode(pPlanOperand->pPlanNode());
  UNUSED(pPlanRoot);
  ASSERT(ShadowRoot::bIsExactType(pPlanRoot));



  try {
    AggregateSplit::vAggregateSplit(pPlanRoot);

  } catch (Parser::SqlException& e) {
    Grid* pGrid = AM::pGrid();
    InfoSet* pInfoSet = _pInfoSet(*m_pLogicUnit->m_xpInfoSet);
    Row* pRow = pInfoSet->pAllotRow(C_VARGS_BEGIN,
                                    pGrid->pThisNode()->szNodeNm(),
                                    EC_SEMANTIC_ERROR,
                                    0,
                                    0,
                                    e.szError(),
                                    C_VARGS_END);
    UNUSED(pRow);
    delete pPlanRoot;
    delete pPlanOperand;
    m_pDataStack->vPushPtr(nullptr);
    goto L_Exit;
  }


  // TODO: eventually we want the received plan graph to be immutable,
  //       so we would send a different version. But for now we will just
  //       manipulate and send the same plan graph
  m_pDataStack->vPushPtr(pPlanOperand);

  L_Exit:
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* sqlCompiler::corrBySquash -- Squash ProjectOps for CORRESPONDING BY clauses*/
/*----------------------------------------------------------------------------*/
void Step::vSqlCompilerCorrBySquash(void) {
  ENTRY();

  /**
   *  This stage is relevant if the query has a SetOp (UNION, INTERSECT, EXCEPT)
   *  and the SetOp has a CORRESPONDING BY clause. In that case the children of
   *  the SetOp will be ProjectOps. This stage will push these ProjectOps down
   *  as far as possible into the children. This will simplify the transliteration
   *  and also any possible double projections from occurring.
   *
   *  e.g. given the query:
   *    SELECT * FROM nation UNION ALL CORRESPONDING SELECT n_nationkey FROM nation;
   *  Then the first child query (SELECT * FROM nation) is projecting all 4 columns
   *  when the union only needs the n_nationkey column. So the CORRESPONDING clause
   *  will cause a ProjectOp to be placed as a child of the union, above the first
   *  child query. This first child query will have a query plan looking like this:
   *
   *           UnionOp
   *           /      \
   *   ProjectOp[*]   ProjectOp
   *   ProjectOp      TableScanOp
   *   TableScanOp
   *
   *   The ProjectOp marked with [*] is the effect of the CORRESPONDING clause.
   *   This stage will look for this condition and eliminate any redundant ProjectOps
   *   immediately below the CORRESPONDING BY ProjectOp.
   *
   *   Also refer to this document for implementation details, challenges and constraints
   *   via some use cases:
   *     Design/Design Discussions/Optimiser-SquashingCorrBy.key
   */


  PlanOperand* pPlanOperand = _pPlanOperand(m_pDataStack->pPopPtr(__func__));
  Parser::RootNode* pPlanRoot = Parser::_pRootNode(pPlanOperand->pPlanNode());
  UNUSED(pPlanRoot);
  ASSERT(ShadowRoot::bIsExactType(pPlanRoot));



  try {
    CorrBySquash::vSearchAndSquash(pPlanRoot);

  } catch (Parser::SqlException& e) {
    Grid* pGrid = AM::pGrid();
    InfoSet* pInfoSet = _pInfoSet(*m_pLogicUnit->m_xpInfoSet);
    Row* pRow = pInfoSet->pAllotRow(C_VARGS_BEGIN,
                                    pGrid->pThisNode()->szNodeNm(),
                                    EC_SEMANTIC_ERROR,
                                    0,
                                    0,
                                    e.szError(),
                                    C_VARGS_END);
    UNUSED(pRow);
    delete pPlanRoot;
    delete pPlanOperand;
    m_pDataStack->vPushPtr(nullptr);
    goto L_Exit;
  }


  // TODO: eventually we want the received plan graph to be immutable,
  //       so we would send a different version. But for now we will just
  //       manipulate and send the same plan graph
  m_pDataStack->vPushPtr(pPlanOperand);

  L_Exit:
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* sqlCompiler::deletePlan -- Simply delete the plan in the operand           */
/*----------------------------------------------------------------------------*/
void Step::vSqlCompilerDeletePlan(void) {
  ENTRY();

  /**
   * Simply delete the plan in the operand.
   * Used to help cleanup after transliterate.
   * Necessary because of the ability to explain the transliterate step.
   */

  // Get the plan operand
  PlanOperand *pPlanOperand = _pPlanOperand(m_pDataStack->pPopPtr(__func__));
  Parser::RootNode* pRootNode = Parser::_pRootNode(pPlanOperand->pPlanNode());
  ASSERT(ShadowRoot::bIsExactType(pRootNode));
  delete pRootNode;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* sqlCompiler::doExplain -- (potentially) create the explain of the plan     */
/*----------------------------------------------------------------------------*/
void Step::vSqlCompilerDoExplain(void) {
  ENTRY();

  /**
   * This is not a strict stage in the sql compiler pipeline. Instead this
   * function is called after every stage is invoked if vSqlCompilerHasExplain()
   * returns true.
   *
   * vSqlCompilerHasExplain() will return true if the plan is topped by an ExplainOp.
   * But not every stage in the plan needs to be explained. So this function first
   * determines if the current stage needs to be explained and if so it creates
   * the rows that describe the plan at this time. Multiple stages can be explained.
   *
   * Finally, if we are explaining the plan then we also need to stop the plan from
   * being executed, so this function also determines when that is. We can't
   * terminate the pipeline early if we haven't explained a stage that the user
   * has requested.
   *
   * This function takes two parameters off the data stack - the name of the
   * current stage that was just processed, and the plan itself.
   */

  // Get the compiler stage
  s256 sCompilerStage;
  m_pDataStack->vPopStr(sCompilerStage, __func__);
  ASSERT(sCompilerStage.nLength() > 0);

  // Get the plan operand
  PlanOperand *pPlanOperand = _pPlanOperand(m_pDataStack->pPopPtr(__func__));
  Parser::RootNode* pRootNode = Parser::_pRootNode(pPlanOperand->pPlanNode());
  UNUSED(pRootNode);
  ASSERT(ShadowRoot::bIsExactType(pRootNode));

  // get the streamSet for explain
  InfoSet *pInfoSet = _pInfoSet(*(m_pLogicUnit)->m_xpStreamSet);

  // Find the explainOp. First get the /__root__/plan node
  QueryNode* pPlanRoot = ShadowRoot::pPlanRoot(pRootNode);
  ASSERT(pPlanRoot);
  ASSERT(ShadowPlan::bIsExactType(pPlanRoot));

  // Get the main subquery
  QueryNode* pMainSubqueryMarker = ShadowPlan::pMainSubquery(pPlanRoot);
  ASSERT(pMainSubqueryMarker);
  ASSERT(MainSubquery::bIsOfType(pMainSubqueryMarker));
  ASSERT(pMainSubqueryMarker->nChildren() == 1);

  QueryNode* pExplainMarker = _pQueryNode(pMainSubqueryMarker->pFirstChild());
  ASSERT(pExplainMarker);
  ASSERT(ExplainMarker::bIsExactType(pExplainMarker));

  QueryNode* pExplainOp = _pQueryNode(pExplainMarker->pFirstChild());
  ASSERT(pExplainOp);
  ASSERT(ExplainOp::bIsExactType(pExplainOp));

  // Determine if this stage needs to be explained. The stage range list
  // may indicate otherwise.
  if (ExplainOp::bExplainStage(pExplainOp)) {
    // This stage does need explaining!
    // Don't explain the explain node, but do explain each child under it.
    s48K sExplainBuf;

    PlanNodeIter* pIter = pExplainOp->pNewChildrenIter();
    while (pIter->bHasNext()) {
      PlanNode* pExplainable = pIter->pNext();
      Parser::PlanObject::vExplain(pExplainable, pRootNode, pExplainOp, sExplainBuf, 0);
    }
    delete pIter;

    i8 iStageNum = ExplainOp::iStageCount(pExplainOp);

    Row *pRow = pInfoSet->pAllotRow(C_VARGS_BEGIN,
                                    iStageNum,
                                    sCompilerStage.szStz(),
                                    sExplainBuf.szStz(),
                                    C_VARGS_END);
    UNUSED(pRow);
  }

  // Do we need to exit after this stage?
  if (ExplainOp::bExitEarly(pExplainOp)) {
    m_pLogicUnit->m_bCompilerToExit = true;
    delete pRootNode;
  }

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* sqlCompiler::doExplainTliterate -- (potentially) create the explain of the plan     */
/*----------------------------------------------------------------------------*/
void Step::vSqlCompilerDoExplainTliterate(void) {
  ENTRY();

  /**
   * This function is called if we have transliterated a plan and that plan
   * has an explain operator. We simply want to put the transliterated pipeline
   * command in the row set.
   */

  // Get the compiler stage
  s256 sCompilerStage;
  m_pDataStack->vPopStr(sCompilerStage, __func__);
  ASSERT(sCompilerStage.nLength() > 0);

  // Get the pipeline command
  s48K sPipelineCmd;
  m_pDataStack->vPopStr(sPipelineCmd, __func__);
  ASSERT(sPipelineCmd.nLength() > 0);

  // Get the plan operand
  PlanOperand *pPlanOperand = _pPlanOperand(m_pDataStack->pPopPtr(__func__));
  Parser::RootNode* pRootNode = Parser::_pRootNode(pPlanOperand->pPlanNode());
  UNUSED(pRootNode);
  ASSERT(ShadowRoot::bIsExactType(pRootNode));

  // get the streamSet for explain
  InfoSet *pInfoSet = _pInfoSet(*(m_pLogicUnit)->m_xpStreamSet);

  // Find the explainOp
  QueryNode* pPlanRoot = ShadowRoot::pPlanRoot(pRootNode);
  ASSERT(pPlanRoot);
  ASSERT(ShadowPlan::bIsExactType(pPlanRoot));

  // Get the main subquery
  QueryNode* pMainSubqueryMarker = ShadowPlan::pMainSubquery(pPlanRoot);
  ASSERT(pMainSubqueryMarker);
  ASSERT(MainSubquery::bIsOfType(pMainSubqueryMarker));
  ASSERT(pMainSubqueryMarker->nChildren() == 1);

  QueryNode* pExplainMarker = _pQueryNode(pMainSubqueryMarker->pFirstChild());
  ASSERT(pExplainMarker);
  ASSERT(ExplainMarker::bIsExactType(pExplainMarker));

  QueryNode* pExplainOp = _pQueryNode(pExplainMarker->pFirstChild());
  ASSERT(pExplainOp);
  ASSERT(ExplainOp::bIsExactType(pExplainOp));

  // Determine if this stage needs to be explained. The stage range list
  // may indicate otherwise.
  if (ExplainOp::bExplainStage(pExplainOp)) {
    // This stage does need explaining!

    i8 iStageNum = ExplainOp::iStageCount(pExplainOp);

    Row *pRow = pInfoSet->pAllotRow(C_VARGS_BEGIN,
                                    iStageNum,
                                    sCompilerStage.szStz(),
                                    sPipelineCmd.szStz(),
                                    C_VARGS_END);
    UNUSED(pRow);
  }

  // We always delete the plan's root node at this point
  delete pRootNode;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* sqlCompiler::explainFormat -- Return the tabledef for the explain format   */
/*----------------------------------------------------------------------------*/
void Step::vSqlCompilerExplainFormat(void) {
  ENTRY();

  /**
   * Given that vSqlCompilerHasExplain() has returned true, this function returns
   * the string constant that is looked up to get the table definition for the
   * rows that are returned from an EXPLAIN query.
   *
   * This function looks up the format property of the ExplainOp and returns
   * the appropriate tabledef type. All returned string constants must come
   * from C_TDEF_*.
   */

  // Get the plan operand
  PlanOperand *pPlanOperand = _pPlanOperand(m_pDataStack->pPopPtr(__func__));
  Parser::RootNode* pRootNode = Parser::_pRootNode(pPlanOperand->pPlanNode());
  UNUSED(pRootNode);
  ASSERT(ShadowRoot::bIsExactType(pRootNode));

  // Find the explainOp
  QueryNode* pPlanRoot = ShadowRoot::pPlanRoot(pRootNode);
  ASSERT(pPlanRoot);
  ASSERT(ShadowPlan::bIsExactType(pPlanRoot));

  // Get the main subquery
  QueryNode* pMainSubqueryMarker = ShadowPlan::pMainSubquery(pPlanRoot);
  ASSERT(pMainSubqueryMarker);
  ASSERT(MainSubquery::bIsOfType(pMainSubqueryMarker));
  ASSERT(pMainSubqueryMarker->nChildren() == 1);

  QueryNode* pExplainMarker = _pQueryNode(pMainSubqueryMarker->pFirstChild());
  ASSERT(pExplainMarker);
  ASSERT(ExplainMarker::bIsExactType(pExplainMarker));

  QueryNode* pExplainOp = _pQueryNode(pExplainMarker->pFirstChild());
  ASSERT(pExplainOp);
  ASSERT(ExplainOp::bIsExactType(pExplainOp));

  // Get the tabledef string for the current explain format
  Stz* szTableDef = ExplainOp::szTableDef(pExplainOp);
  ASSERT(szTableDef);

  m_pDataStack->vPushStz(szTableDef);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* sqlCompiler::explainPlan -- explain plan stage                             */
/*----------------------------------------------------------------------------*/
void Step::vSqlCompilerExplainPlan(void)
{
  ENTRY();

  /**
   * The purpose of this stage in the pipeline is to be a marker for the last
   * point in the SQL compilation pipeline before the plan graph is
   * transliterated into a pipeline statement. If an query is being explained
   * then this is the point that it happens.
   *
   * However the explain does not actually occur here. Because we can ask for
   * an explain of many or all stages from the parser to this stage the
   * SQL compilation pipeline calls the above vSqlCompilerDoExplain() step after
   * running the step for each stage of the pipeline. So we don't need to do
   * anything specific here. Instead this empty stage is basically a name
   * marker for the last step prior to transliteration.
   *
   * The plan is not modified in this stage.
   */

  EXIT();
}


/*----------------------------------------------------------------------------*/
/* sqlCompiler::exprPushdown -- Push expressions down as far as possible      */
/*----------------------------------------------------------------------------*/
void Step::vSqlCompilerExprPushdown(void)
{
  ENTRY();

  /**
   * The purpose of this stage is to ensure that all expressions in sql operators
   * above a ProjectOp or AggregateOp (so a HAVING clause's FilterOp or a
   * ORDER BY's OrderOp) are pushed down into a ProjectOp or AggregateOp to be
   * calculated, if required.
   *
   * e.g. consider this statement:
   *   SELECT col1, SUM(col2) FROM t GROUP BY col1 HAVING AVG(col3) > 0;
   * In this statement neither AVG(col3) or col3 is available or can be calculated
   * because the AggregateOp below it in the plan only projects [col1, SUM(col2)].
   * So we need it to also project [AVG(col3) > 0] as well so the FilterOp that implements
   * the HAVING clause can test it.
   *
   * After pushing down expressions we will then walk the graph and validate that
   * for all Sql operators that contain expressions those expressions can be entirely
   * represented in ordinal notation relative to the set of projected columns that
   * operator will receive.
   *
   * Note that in this stage there may be multiple ProjectOps in a given StatementMarker
   * section due to the PostProject stage running beforehand.
   *
   * The plan may be modified in this stage.
   */

  PlanOperand* pPlanOperand = _pPlanOperand(m_pDataStack->pPopPtr(__func__));
  Parser::RootNode* pPlanRoot = Parser::_pRootNode(pPlanOperand->pPlanNode());
  UNUSED(pPlanRoot);
  ASSERT(ShadowRoot::bIsExactType(pPlanRoot));


  try {
    ExprPushdown::vPushdownExprs(pPlanRoot);

  } catch (Parser::SqlException& e) {
    Grid* pGrid = AM::pGrid();
    InfoSet* pInfoSet = _pInfoSet(*m_pLogicUnit->m_xpInfoSet);
    Row* pRow = pInfoSet->pAllotRow(C_VARGS_BEGIN,
                                    pGrid->pThisNode()->szNodeNm(),
                                    EC_SEMANTIC_ERROR,
                                    0,
                                    0,
                                    e.szError(),
                                    C_VARGS_END);
    UNUSED(pRow);
    delete pPlanRoot;
    delete pPlanOperand;
    m_pDataStack->vPushPtr(nullptr);
    goto L_Exit;
  }


  // TODO: eventually we want the received plan graph to be immutable,
  //       so we would send a different version. But for now we will just
  //       manipulate and send the same plan graph
  m_pDataStack->vPushPtr(pPlanOperand);

  L_Exit:
  EXIT();
}


/*----------------------------------------------------------------------------*/
/* sqlCompiler::joinOptimisation -- temporary join optimisation via hinting   */
/*----------------------------------------------------------------------------*/
void Step::vSqlCompilerJoinOptimisation(void)
{
  ENTRY();

  /**
   * The purpose of this stage is to look for any join hints and apply them.
   *
   * The plan may be modified in this stage.
   */

  PlanOperand* pPlanOperand = _pPlanOperand(m_pDataStack->pPopPtr(__func__));
  Parser::RootNode* pPlanRoot = Parser::_pRootNode(pPlanOperand->pPlanNode());
  UNUSED(pPlanRoot);
  ASSERT(ShadowRoot::bIsExactType(pPlanRoot));


  try {
    JoinOptimisation::vOptimiseJoins(pPlanRoot);

  } catch (Parser::SqlException& e) {
    Grid* pGrid = AM::pGrid();
    InfoSet* pInfoSet = _pInfoSet(*m_pLogicUnit->m_xpInfoSet);
    Row* pRow = pInfoSet->pAllotRow(C_VARGS_BEGIN,
                                    pGrid->pThisNode()->szNodeNm(),
                                    EC_SEMANTIC_ERROR,
                                    0,
                                    0,
                                    e.szError(),
                                    C_VARGS_END);
    UNUSED(pRow);
    delete pPlanRoot;
    delete pPlanOperand;
    m_pDataStack->vPushPtr(nullptr);
    goto L_Exit;
  }


  // TODO: eventually we want the received plan graph to be immutable,
  //       so we would send a different version. But for now we will just
  //       manipulate and send the same plan graph
  m_pDataStack->vPushPtr(pPlanOperand);

  L_Exit:
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* sqlCompiler::exprValidate -- expression validation                         */
/*----------------------------------------------------------------------------*/
void Step::vSqlCompilerExprValidate(void)
{
  ENTRY();

  /**
   * At this point of the pipeline we have a basic plan with names that have
   * been successfully resolved and fully qualified. The plan is not yet executable
   * and follows a basic structure - there is either one ProjectOp or AggregateOp
   * (depending on whether there is a GROUP BY list), a FilterOp below this
   * Project/AggregateOp represents a WHERE clause and a FilterOp above this
   * Project/AggregateOp represents a HAVING clause.
   *
   * This stage checks the expressions of (almost) all operators in the plan
   * to ensure that they are semantically correct. They tend to focus on
   * aggregations and group by expressions, as well as the expressions that can
   * be found in the operators that sit above the Project/AggregateOp.
   *
   * Some expression validation was done at parse time because it was just easier
   * to do them then. e.g. validating that expressions in a WHERE clause cannot
   * contain aggregations.
   *
   * The plan is not modified in this stage.
   */

  PlanOperand* pPlanOperand = _pPlanOperand(m_pDataStack->pPopPtr(__func__));
  Parser::RootNode* pPlanRoot = Parser::_pRootNode(pPlanOperand->pPlanNode());
  UNUSED(pPlanRoot);
  ASSERT(ShadowRoot::bIsExactType(pPlanRoot));


  try {
    ExprValidator::vValidateExprs(pPlanRoot);

  } catch (Parser::SqlException& e) {
    Grid* pGrid = AM::pGrid();
    InfoSet* pInfoSet = _pInfoSet(*m_pLogicUnit->m_xpInfoSet);
    Row* pRow = pInfoSet->pAllotRow(C_VARGS_BEGIN,
                                    pGrid->pThisNode()->szNodeNm(),
                                    EC_SEMANTIC_ERROR,
                                    0,
                                    0,
                                    e.szError(),
                                    C_VARGS_END);
    UNUSED(pRow);
    delete pPlanRoot;
    delete pPlanOperand;
    m_pDataStack->vPushPtr(nullptr);
    goto L_Exit;
  }


  // TODO: eventually we want the received plan graph to be immutable,
  //       so we would send a different version. But for now we will just
  //       manipulate and send the same plan graph
  m_pDataStack->vPushPtr(pPlanOperand);

  L_Exit:
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* sqlCompiler::hasExplain -- does sql have to do an explain?                 */
/*----------------------------------------------------------------------------*/
void Step::vSqlCompilerHasExplain(void) {
  ENTRY();

  bool bExplain = false;

  // Get the compiler stage
  s256 sCompilerStage;
  m_pDataStack->vPopStr(sCompilerStage, __func__);
  ASSERT(sCompilerStage.nLength() > 0);

  // Get the plan operand
  PlanOperand *pPlanOperand = _pPlanOperand(m_pDataStack->pPopPtr(__func__));

  Parser::RootNode* pRootNode = Parser::_pRootNode(pPlanOperand->pPlanNode());
  UNUSED(pRootNode);
  ASSERT(ShadowRoot::bIsExactType(pRootNode));

  // Determine if there is an ExplainOp in this plan.
  QueryNode* pPlanRoot = ShadowRoot::pPlanRoot(pRootNode);

  // pPlanRoot should have a child which is a MainSubquery marker.
  // This marker may have one child being an ExplainMarker.
  // The ExplainMarker should have one child being the ExplainOp

  // Get the main subquery
  QueryNode* pMainSubqueryMarker = ShadowPlan::pMainSubquery(pPlanRoot);
  ASSERT(pMainSubqueryMarker);
  ASSERT(MainSubquery::bIsOfType(pMainSubqueryMarker));
  ASSERT(pMainSubqueryMarker->nChildren() == 1);

  QueryNode* pMarker = _pQueryNode(pMainSubqueryMarker->pFirstChild());
  if (pMarker && ExplainMarker::bIsExactType(pMarker)) {

    QueryNode* pExplainOp = _pQueryNode(pMarker->pFirstChild());
    if (pExplainOp && ExplainOp::bIsExactType(pExplainOp)) {
      bExplain = true;

      // Increment the number of compiler stages encountered
      ExplainOp::vIncStageCount(pExplainOp);

      // Record the names of the compiler stages encountered
      ExplainOp::vAddCompilerStageName(pExplainOp, sCompilerStage.szStz());
    }
  }

  m_pDataStack->vPushInt(bExplain);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* sqlCompiler::nameValidate -- name validation                               */
/*----------------------------------------------------------------------------*/
void Step::vSqlCompilerNameValidate(void)
{
  ENTRY();

  /**
   * At this stage of the pipeline we have a basic plan with a basic shape
   * that is not yet executable. The expressions may contain ColumnRefs that
   * are not fully qualified (i.e. use a catalog.schema.table.columnName
   * syntax) and these ColumnRefs may not be correct.
   *
   * This stage will try to resolve the ColumnRefs using the DataSource objects
   * (of which a TableScanOp is an example) that sit below them. Errors will
   * be returned for columns that are unresolvable or ambiguous. If a ColumnRef
   * can be resolved then it will also then be fully qualified too (i.e. the
   * catalog/schema/table names will be added to the ColumnRef).
   *
   * Therefore the plan is modified in this stage.
   */

  PlanOperand* pPlanOperand = _pPlanOperand(m_pDataStack->pPopPtr(__func__));
  Parser::RootNode* pPlanRoot = Parser::_pRootNode(pPlanOperand->pPlanNode());
  UNUSED(pPlanRoot);
  ASSERT(ShadowRoot::bIsExactType(pPlanRoot));


  // Walk the graph and validate that each name can be resolved uniquely.
  // i.e. each table is known and unambiguous, each column refers to tables
  // that are being used, etc.
  // Fill in the catalog/schema/table fields of the columnRef and tableRefs
  // where applicable.
  try {
    NameValidator::vValidateNames(pPlanRoot);

  } catch (Parser::SqlException& e) {
    Grid* pGrid = AM::pGrid();
    InfoSet* pInfoSet = _pInfoSet(*m_pLogicUnit->m_xpInfoSet);
    Row* pRow = pInfoSet->pAllotRow(C_VARGS_BEGIN,
                                    pGrid->pThisNode()->szNodeNm(),
                                    EC_SEMANTIC_ERROR,
                                    0,
                                    0,
                                    e.szError(),
                                    C_VARGS_END);
    UNUSED(pRow);
    delete pPlanRoot;
    delete pPlanOperand;
    m_pDataStack->vPushPtr(nullptr);
    goto L_Exit;
  }

  // At this point all names in the graph should be fully resolved
  // i.e. they have catalog/schema/table names.

  // TODO: eventually we want the received plan graph to be immutable,
  //       so we would send a different version. But for now we will just
  //       manipulate and send the same plan graph
  m_pDataStack->vPushPtr(pPlanOperand);

L_Exit:
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* sqlCompiler::parser -- sql parser                                          */
/*----------------------------------------------------------------------------*/
void Step::vSqlCompilerParser(void)
{
  ENTRY();

  /**
   * This stage of the plan is all about producing an initial basic plan graph.
   * The graph is not yet executable and it may not be correct (e.g. incorrect
   * names, bad semantics, etc).
   *
   * The parser will generate one ProjectOp for a basic SELECT statement. If the
   * statement contains a GROUP BY clause then it will produce an AggregateOp
   * instead. So the select list can be found by looking for either the ProjectOp
   * or AggregateOp.
   *
   * The basic structure of the plan is as follows (for a SELECT statement):
   *
   *    StatementMarker            (denotes the type of statement that follows)
   *    SetOp                      (represents UNION/INTERSECT/EXCEPT)
   *    LimitOp
   *    OrderOp
   *    FilterOp                   (represents the HAVING clause)
   *    ProjectOp or AggregateOp   (represents the SELECT list and GROUP BY)
   *    FilterOp                   (represents WHERE clause)
   *    JoinOp
   *    DataSource                 (e.g. TableScanOp etc)
   *
   * A more complicated query that contains multiple distinct sub parts are
   * delineated by further StatementMarkers. Examples of this could include
   * an INSERT..SELECT statement, CREATE TABLE AS SELECT, a UNION of two
   * SELECT statements, a SELECT statement with subqueries, etc.
   *
   * Here's an example of a UNIONed SELECT statement:
   *
   *                   StatementMarker           (indicates it is a SELECT statement)
   *                   UnionOp
   *    StatementMarker       StatementMarker
   *    LimitOp               LimitOp
   *    OrderOp               OrderOp
   *    FilterOp              FilterOp
   *    ProjectOp             ProjectOp
   *    FilterOp              FilterOp
   *    JoinOp                JoinOp
   *    DataSource            DataSource
   *
   * Note that there is a StatementMarker for the top level unioned statement
   * as well as for each of the children. This allows us to walk the graph
   * between StatementMarkers easily, that way the same algorithms can focus on
   * a single SELECT statement and not be concerned about the shape of the overall
   * query.
   *
   */

  PlanOperand* pPlanOperand;

  // Get the original SQL statement entered
  s4K sSqlStatement;
  m_pDataStack->vPopStr(sSqlStatement,__func__);
  
  //vTrace("vSqlCompilerParser got : %s\n"_Stz,sSqlStatement.szStz());

  // Read in and parse the plan graph definitions
  const Parser::PlanNode* pGraphTemplate = Parser::PlanDefinitionParser::pGraphTemplate();
  RASSERT(pGraphTemplate);

  // Parse the SQL command.
  Parser::SqlPlanner* pPlanner = new Parser::SqlPlanner();
  Parser::PlanNode* pPlanRoot;

  try {
    pPlanRoot = pPlanner->plan(sSqlStatement.szStz(), pGraphTemplate);

  } catch (Parser::SyntaxException& e) {
    //vTrace("SyntaxException at %lld:%lld : %s\n"_Stz, e.iLine(), e.iColumn(), e.szError());
    InfoSet* pInfoSet = _pInfoSet(*m_pLogicUnit->m_xpInfoSet);
    Grid* pGrid = AM::pGrid();
    Row* pRow = pInfoSet->pAllotRow(C_VARGS_BEGIN,
                                    pGrid->pThisNode()->szNodeNm(),
                                    EC_SYNTAX_ERROR,
                                    e.iLine(),
                                    e.iColumn(),
                                    e.szError(),
                                    C_VARGS_END);
    UNUSED(pRow);
    
    m_pDataStack->vPushPtr(nullptr);
    goto L_Exit;

  } catch (Parser::SemanticException& e) {
    //vTrace("SemanticException: %s\n"_Stz, e.szError());
    Grid* pGrid = AM::pGrid();
    InfoSet* pInfoSet = _pInfoSet(*m_pLogicUnit->m_xpInfoSet);
    Row* pRow = pInfoSet->pAllotRow(C_VARGS_BEGIN,
                                    pGrid->pThisNode()->szNodeNm(),
                                    EC_SEMANTIC_ERROR,
                                    0,
                                    0,
                                    e.szError(),
                                    C_VARGS_END);
    UNUSED(pRow);
    m_pDataStack->vPushPtr(nullptr);
    goto L_Exit;
  }

  ASSERT(pPlanRoot);

  pPlanOperand = new PlanOperand(pPlanRoot);
  m_pDataStack->vPushPtr(pPlanOperand);

  //vTrace("  antlr(%s) -> RootNode %p\n"_Stz,
  //       sSqlStatement.szStz(),pPlanNode_root);

L_Exit:
  delete pPlanner;
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* sqlCompiler::partitionExplain -- explain the partitioned plan stage        */
/*----------------------------------------------------------------------------*/
void Step::vSqlCompilerPartitionExplain(void)
{
  ENTRY();

  /**
   * The purpose of this stage in the pipeline is to be a marker for the last
   * point in the SQL compilation pipeline before the plan graph is
   * transliterated into a pipeline statement. If a partitioned query is being
   * explained then this is the point that it happens.
   *
   * However the explain does not actually occur here. Because we can ask for
   * an explain of many or all stages from the parser to this stage the
   * SQL compilation pipeline calls the above vSqlCompilerDoExplain() step after
   * running the step for each stage of the pipeline. So we don't need to do
   * anything specific here. Instead this empty stage is basically a name
   * marker for the last step prior to transliteration.
   *
   * The plan is not modified in this stage.
   */

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* sqlCompiler::partitionPlan -- parallelise the plan based on partitions     */
/*----------------------------------------------------------------------------*/
void Step::vSqlCompilerPartitionPlan(void)
{
  ENTRY();

  /**
   * The purpose of this stage in the pipeline is to...
   *
   * The plan is heavily modified in this stage.
   */

  PlanOperand* pPlanOperand = _pPlanOperand(m_pDataStack->pPopPtr(__func__));
  Parser::RootNode* pRootNode = Parser::_pRootNode(pPlanOperand->pPlanNode());
  ASSERT(ShadowRoot::bIsExactType(pRootNode));


  try {
    // Partition the plan
    PartitionPlan::vPartitionPlan(pRootNode);

  } catch (Parser::SqlException& e) {
    Grid* pGrid = AM::pGrid();
    InfoSet* pInfoSet = _pInfoSet(*m_pLogicUnit->m_xpInfoSet);
    Row* pRow = pInfoSet->pAllotRow(C_VARGS_BEGIN,
                                    pGrid->pThisNode()->szNodeNm(),
                                    EC_SEMANTIC_ERROR,
                                    0,
                                    0,
                                    e.szError(),
                                    C_VARGS_END);
    UNUSED(pRow);
    delete pRootNode;
    delete pPlanOperand;
    m_pDataStack->vPushPtr(nullptr);
    goto L_Exit;
  }

#ifdef WITH_ASSERT_MACRO
  // Ensure the graph syntax is still valid after this major change
  GraphValidator::vValidatePlan(pRootNode);
#endif

  // TODO: eventually we want the received plan graph to be immutable,
  //       so we would send a different version. But for now we will just
  //       manipulate and send the same plan graph
  m_pDataStack->vPushPtr(pPlanOperand);

  L_Exit:
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* sqlCompiler::PostProject -- Ensure only selected cols are being returned   */
/*----------------------------------------------------------------------------*/
void Step::vSqlCompilerPostProject(void)
{
  ENTRY();

  /**
   * This stage solves the issue of there being an expression in an ORDER BY
   * or HAVING clause that is not in the select list.
   *   e.g. SELECT col1 FROM t ORDER BY col2;
   *
   * In the above example we need to ensure that col2 is being generated
   * (refer to the exprPushdown stage for that) but that only col1 is being
   * returned. We do that by adding another ProjectOp on top of the plan,
   * if required.
   *
   * The plan may be modified in this stage.
   * At the end of this stage we may have multiple ProjectOps in the same
   * statement section of the plan. Prior to this stage there was only ever
   * one ProjectOp or AggregateOp (which is derived from ProjectOp anyway).
   */

  PlanOperand* pPlanOperand = _pPlanOperand(m_pDataStack->pPopPtr(__func__));
  Parser::RootNode* pRootNode = Parser::_pRootNode(pPlanOperand->pPlanNode());
  UNUSED(pRootNode);
  ASSERT(ShadowRoot::bIsExactType(pRootNode));


  try {
    // Add the post projection, if required
    PostProject::vCheckPostProject(pRootNode);

  } catch (Parser::SqlException& e) {
    Grid* pGrid = AM::pGrid();
    InfoSet* pInfoSet = _pInfoSet(*m_pLogicUnit->m_xpInfoSet);
    Row* pRow = pInfoSet->pAllotRow(C_VARGS_BEGIN,
                                    pGrid->pThisNode()->szNodeNm(),
                                    EC_SEMANTIC_ERROR,
                                    0,
                                    0,
                                    e.szError(),
                                    C_VARGS_END);
    UNUSED(pRow);
    delete pRootNode;
    delete pPlanOperand;
    m_pDataStack->vPushPtr(nullptr);
    goto L_Exit;
  }


  // TODO: eventually we want the received plan graph to be immutable,
  //       so we would send a different version. But for now we will just
  //       manipulate and send the same plan graph
  m_pDataStack->vPushPtr(pPlanOperand);

  L_Exit:
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* sqlCompiler::projValidation -- Check column projections are valid          */
/*----------------------------------------------------------------------------*/
void Step::vSqlCompilerProjValidation(void)
{
  ENTRY();

  /**
   * The purpose of this stage is to ensure that all expressions in SQL operators
   * are supplied the right column definitions by the SQL operator below them.
   *
   * i.e., an expression in an arbitrary SQL operator can be converted into an
   * ordinal representation and that ordinal expression is "fully ordinal" - it
   * does not have any ColumnRefs at all. This means that the expression only
   * uses the projected columns from operators below it.
   *
   * The plan is not modified in this stage.
   */

  PlanOperand* pPlanOperand = _pPlanOperand(m_pDataStack->pPopPtr(__func__));
  Parser::RootNode* pPlanRoot = Parser::_pRootNode(pPlanOperand->pPlanNode());
  UNUSED(pPlanRoot);
  ASSERT(ShadowRoot::bIsExactType(pPlanRoot));


  try {
    ProjValidation::vValidateExprVisibility(pPlanRoot);

  } catch (Parser::SqlException& e) {
    Grid* pGrid = AM::pGrid();
    InfoSet* pInfoSet = _pInfoSet(*m_pLogicUnit->m_xpInfoSet);
    Row* pRow = pInfoSet->pAllotRow(C_VARGS_BEGIN,
                                    pGrid->pThisNode()->szNodeNm(),
                                    EC_SEMANTIC_ERROR,
                                    0,
                                    0,
                                    e.szError(),
                                    C_VARGS_END);
    UNUSED(pRow);
    delete pPlanRoot;
    delete pPlanOperand;
    m_pDataStack->vPushPtr(nullptr);
    goto L_Exit;
  }


  // TODO: eventually we want the received plan graph to be immutable,
  //       so we would send a different version. But for now we will just
  //       manipulate and send the same plan graph
  m_pDataStack->vPushPtr(pPlanOperand);

  L_Exit:
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* sqlCompiler::setOpValidate -- SetOp simplication & validation              */
/*----------------------------------------------------------------------------*/
void Step::vSqlCompilerSetOpValidate(void)
{
  ENTRY();

  /**
   * This stage of the plan is about looking for set operators (UNION,
   * INTERSECT, etc) and handling their CORRESPONDING BY list if applicable.
   * Then we can validate that the semantics of the set operation are valid.
   * Each set operator should exist in a separate statement section, bounded
   * by StatementMarkers.
   */

  PlanOperand* pPlanOperand = _pPlanOperand(m_pDataStack->pPopPtr(__func__));
  Parser::RootNode* pPlanRoot = Parser::_pRootNode(pPlanOperand->pPlanNode());
  UNUSED(pPlanRoot);
  ASSERT(ShadowRoot::bIsExactType(pPlanRoot));



  try {
    if (SetOpValidator::bHasSetOps(pPlanRoot)) {
      SetOpValidator::vSimplifySetOps(pPlanRoot);
      SetOpValidator::vValidateSetOps(pPlanRoot);
    }

  } catch (Parser::SqlException& e) {
    Grid* pGrid = AM::pGrid();
    InfoSet* pInfoSet = _pInfoSet(*m_pLogicUnit->m_xpInfoSet);
    Row* pRow = pInfoSet->pAllotRow(C_VARGS_BEGIN,
                                    pGrid->pThisNode()->szNodeNm(),
                                    EC_SEMANTIC_ERROR,
                                    0,
                                    0,
                                    e.szError(),
                                    C_VARGS_END);
    UNUSED(pRow);
    delete pPlanRoot;
    delete pPlanOperand;
    m_pDataStack->vPushPtr(nullptr);
    goto L_Exit;
  }

  // TODO: eventually we want the received plan graph to be immutable,
  //       so we would send a different version. But for now we will just
  //       manipulate and send the same plan graph
  m_pDataStack->vPushPtr(pPlanOperand);

  L_Exit:
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* sqlCompiler::stageMarkers -- Convert StatementMarkers to StageMarkers      */
/*----------------------------------------------------------------------------*/
void Step::vSqlCompilerStageMarkers(void)
{
  ENTRY();

  /**
   * This stage of the plan is about removing StatementMarkers (used by the compiler)
   * and inserting StageMarkers (used by the transliteration). StageMarkers tell
   * the transliterator when to create a new stage in the pipeline statement.
   */

  PlanOperand* pPlanOperand = _pPlanOperand(m_pDataStack->pPopPtr(__func__));
  Parser::RootNode* pPlanRoot = Parser::_pRootNode(pPlanOperand->pPlanNode());
  UNUSED(pPlanRoot);
  ASSERT(ShadowRoot::bIsExactType(pPlanRoot));

  try {
    StageMarkers::vStageMarkers(pPlanRoot);

  } catch (Parser::SqlException& e) {
    Grid* pGrid = AM::pGrid();
    InfoSet* pInfoSet = _pInfoSet(*m_pLogicUnit->m_xpInfoSet);
    Row* pRow = pInfoSet->pAllotRow(C_VARGS_BEGIN,
                                    pGrid->pThisNode()->szNodeNm(),
                                    EC_SEMANTIC_ERROR,
                                    0,
                                    0,
                                    e.szError(),
                                    C_VARGS_END);
    UNUSED(pRow);
    delete pPlanRoot;
    delete pPlanOperand;
    m_pDataStack->vPushPtr(nullptr);
    goto L_Exit;
  }

  // TODO: eventually we want the received plan graph to be immutable,
  //       so we would send a different version. But for now we will just
  //       manipulate and send the same plan graph
  m_pDataStack->vPushPtr(pPlanOperand);

  L_Exit:
  EXIT();
}


/*----------------------------------------------------------------------------*/
/* sqlCompiler::transliterate -- plan to pipeline transliterate               */
/*----------------------------------------------------------------------------*/
void Step::vSqlCompilerTransliterate(void)
{
  ENTRY();
  
  //vTrace("\nStep::vSqlCompilerTransliterate\n"_Stz);

  PlanOperand* pPlanOperand = _pPlanOperand(m_pDataStack->pPopPtr(__func__));
  Parser::RootNode* pPlanRoot = Parser::_pRootNode(pPlanOperand->pPlanNode());
  ASSERT(ShadowRoot::bIsExactType(pPlanRoot));


  // Walk the graph and produce a pipeline statement.
  //bool bTransliterate = Dynamo::bPlanToPipeline(sPipelineStmt,pPlanRoot);

  s48K sPipelineStmt;

  Backend* pBackend = new Backend(pPlanRoot);
  bool bTransliterate = pBackend->bTransliterate(sPipelineStmt);
  ASSERT(bTransliterate);
  UNUSED(bTransliterate);

  delete pBackend;

  // return the pipeline
  m_pDataStack->vPushStr(sPipelineStmt);

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
