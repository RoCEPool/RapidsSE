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
#include "CrvBuffer.hpp"
#include "TablePtn.hpp"
#include "Table.hpp"

#include "OpSpec.hpp"
#include "StageSpec.hpp"
#include "PipelineSpec.hpp"
#include "Pipeline.hpp"
#include "Dynamo.hpp"
#include "Job.hpp"
#include "Node_I.hpp"
#include "LogicUnit.hpp"

#include "PlanNode.hpp"
#include "RootNode.hpp"
#include "PlanDefinitionParser.hpp"
#include "SqlPlanner.hpp"
#include "ShadowNode.hpp"
#include "ShadowRoot.hpp"
#include "PlanConstants.hpp"
#include "GraphNav.hpp"
#include "PlanOperand.hpp"

#include "NameValidator.hpp"
#include "ExprValidator.hpp"

#include "SqlException.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_SqlCompiler(void)
{
  vAddStep("sqlCompiler::exprValidate"_Stz,   &Step::vSqlCompilerExprValidate);
  vAddStep("sqlCompiler::nameValidate"_Stz,   &Step::vSqlCompilerNameValidate);
  vAddStep("sqlCompiler::parser"_Stz,         &Step::vSqlCompilerParser);
  vAddStep("sqlCompiler::transliterate"_Stz,  &Step::vSqlCompilerTransliterate);
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

  s48K sExplainBuf;

  PlanOperand* pPlanOperand = _pPlanOperand(m_pDataStack->pPopPtr(__func__));
  Parser::RootNode* pPlanRoot = Parser::_pRootNode(pPlanOperand->pPlanNode());
  UNUSED(pPlanRoot);


  try {
    ExprValidator::vValidateExprs(pPlanRoot);

  } catch (Parser::SqlException& e) {
    Grid* pGrid = AM::pGrid();
    InfoSet* pInfoSet = _pInfoSet(*( m_pJob->pLogicUnit_job() )->m_xpInfoSet);
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

  s48K sExplainBuf;

  PlanOperand* pPlanOperand = _pPlanOperand(m_pDataStack->pPopPtr(__func__));
  Parser::RootNode* pPlanRoot = Parser::_pRootNode(pPlanOperand->pPlanNode());
  UNUSED(pPlanRoot);

  // Walk the graph and validate that each name can be resolved uniquely.
  // i.e. each table is known and unambiguous, each column refers to tables
  // that are being used, etc.
  // Fill in the catalog/schema/table fields of the columnRef and tableRefs
  // where applicable.
  try {
    NameValidator::vValidateNames(pPlanRoot);

  } catch (Parser::SqlException& e) {
    Grid* pGrid = AM::pGrid();
    InfoSet* pInfoSet = _pInfoSet(*( m_pJob->pLogicUnit_job() )->m_xpInfoSet);
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

  Parser::ShadowRoot::vExplain(pPlanRoot, sExplainBuf, 0);
  vTrace("%s\n"_Stz, sExplainBuf.szStz());

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
    InfoSet* pInfoSet = _pInfoSet(*( m_pJob->pLogicUnit_job() )->m_xpInfoSet);
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
    InfoSet* pInfoSet = _pInfoSet(*( m_pJob->pLogicUnit_job() )->m_xpInfoSet);
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
/* sqlCompiler::transliterate -- plan to pipeline transliterate               */
/*----------------------------------------------------------------------------*/
void Step::vSqlCompilerTransliterate(void)
{
  ENTRY();
  
  //vTrace("\nStep::vSqlCompilerTransliterate\n"_Stz);

  PlanOperand* pPlanOperand = _pPlanOperand(m_pDataStack->pPopPtr(__func__));
  Parser::RootNode* pPlanRoot = Parser::_pRootNode(pPlanOperand->pPlanNode());

  s48K sPipelineStmt;

  // Walk the graph and produce a pipeline statement.
  bool bTransliterate = Dynamo::bPlanToPipeline(sPipelineStmt,pPlanRoot);
  ASSERT(bTransliterate);
  UNUSED(bTransliterate);

  // return the pipeline
  m_pDataStack->vPushStr(sPipelineStmt);

  delete pPlanOperand;
  delete pPlanRoot;

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
