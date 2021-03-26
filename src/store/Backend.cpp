/*============================================================================*/
/* Backend.cpp -- Backend (plan to pipeline transliterate)                    */
/*============================================================================*/

#include "BASE.h"

#include "ePX.h"

#include "Grid.hpp"
#include "OpSpec.hpp"
#include "PipelineSpec.hpp"
#include "Pipeline.hpp"
#include "Backend.hpp"
#include "Job.hpp"
#include "StageSpec.hpp"

#include "TagConstants.h"

#include "SP_TknVecStr.hpp"

#include "SqlOperator.hpp"
#include "ShadowNode.hpp"
#include "ShadowRoot.hpp"
#include "StatementMarker.hpp"
#include "PlanConstants.hpp"
#include "GraphNav.hpp"
#include "PlanOperand.hpp"
#include "TableScanOp.hpp"
#include "QueryNode.hpp"

#include "TableRef.hpp"

#include "AggregateOp.hpp"
#include "SumExpr.hpp"

#include "ColumnRef.hpp"
#include "Expr.hpp"
#include "FilterOp.hpp"
#include "IntegerLiteral.hpp"
#include "Literal.hpp"
#include "ProjectedColumn.hpp"
#include "ProjectOp.hpp"

#include "ComparisonExpr.hpp"
#include "EqExpr.hpp"
#include "GeExpr.hpp"
#include "GtExpr.hpp"
#include "LeExpr.hpp"
#include "LtExpr.hpp"
#include "NeExpr.hpp"

#include "LogicExpr.hpp"
#include "AndExpr.hpp"
#include "IsNullExpr.hpp"
#include "NotExpr.hpp"
#include "OrExpr.hpp"
#include "XorExpr.hpp"

#include "MathematicalExpr.hpp"
#include "AdditionExpr.hpp"
#include "DivisionExpr.hpp"
#include "ModulusExpr.hpp"
#include "MultiplicationExpr.hpp"
#include "NegationExpr.hpp"
#include "SubtractionExpr.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,Backend,CN_BACKEND)
__aNewFunction(Backend,CN_BACKEND)
__vDeleteFunction(Backend,CN_BACKEND)
__aFooterFunction(Backend)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Backend::Backend(RootNode* pPlanRoot)
: Object(CN_BACKEND)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BACKEND,_FOOTER_);

  m_sPipeline = nullptr;
  m_pPlanRoot = pPlanRoot;
  
  _checkInitialised_(CN_BACKEND);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_BACKEND,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Backend::~Backend(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BACKEND,_FOOTER_);
  
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* transliterate plan into a pipeline statement                               */
/*----------------------------------------------------------------------------*/
bool Backend::bTransliterate(String* sPipeline) {
  _static_

  m_sPipeline = sPipeline;

//#ifdef WITH_TRACE_Backend
  vPrintPlanGraph(m_pPlanRoot);
//#endif

  char cStageStart   = 'A';
  char cStageCurrent = 'A';

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  m_sPipeline->vCopy(
      "pipeline unnamed\n"
      "begin\n"
      ""_Stz);

  m_sPipeline->vCatSprint(
      "  stage stage%c on origin :\n"
      "    start\n"
      "    | periodic once\n"
      "    | stageB\n"
      ""_Stz,
      cStageStart);

  ++ cStageCurrent;

  m_sPipeline->vCatSprint(
      "  stage stage%c on all partitions :\n"
      ""_Stz,
      cStageCurrent);


  // Walk the plan graph from the root, skipping the properties but descending
  // the children. We don't have to walk from the root, but if we don't then
  // the path string that is passed into our visitor function will be a relative
  // path and not an absolute one.
  // Let's also pass in the root node as an argument as it can be useful within
  // the visitor function.
  GraphNav::bGlobalWalkBottomUp(m_pPlanRoot, false, true, vVisitorPlan, (void *) m_sPipeline);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  m_sPipeline->vCatSprint(
      "    | stage%c\n"
      ""_Stz,
      cStageCurrent + 1);

  ++ cStageCurrent;

  // check if any aggregation in previous stage
  char* sAggreg = m_sPipeline->azStr();

  if (StrLib::bFind(&sAggreg, "aggreg")) {

   // now append the aggreg as the super aggregate in the last stage (on origin)
   s4K sAgg;
   StrLib::vCopyTill(sAgg.azChar(), sAggreg, '|');

    m_sPipeline->vCatSprint(
        "  stage stage%c on origin :\n"
        "    %s"
        "    | output\n"
        ""_Stz,
        cStageCurrent, sAgg.azChar());
  }
  else {
    m_sPipeline->vCatSprint(
        "  stage stage%c on origin :\n"
        "    output\n"
        ""_Stz,
        cStageCurrent);
  }
  m_sPipeline->vCatSprint(
      "    | sink\n"
      "    | stage%c\n"
      ""_Stz,
      cStageStart);

  m_sPipeline->vConcat(
      "end ;\n"
      ""_Stz);

  //vTrace("Pipeline: \n %s\n"_Stz, pAsPipelineStmt->azStr());

  return true;
}

/*----------------------------------------------------------------------------*/
/* print the plan graph                                                       */
/*----------------------------------------------------------------------------*/
void Backend::vPrintPlanGraph(PlanNode* pPlanRoot) {

  // Lambda visitor function to print the graph

  auto fVisitor = [](PlanNode *pNode, s256 &sPath, i8 iDepth, void *pvArg) -> GraphNav::eWalkCtrl {

    UNUSED(pvArg);
    
    vTrace("(%02lld) %s\t\t(%s)\n"_Stz, iDepth, sPath.szStz(), aCN[pNode->eGetClassNo()]);

    // Keep walking
    return GraphNav::WALK_CONTINUE;
  };

  GraphNav::bGlobalWalk(pPlanRoot, true, true, fVisitor, pPlanRoot);
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* append project Op to the pipeline                                          */
/*----------------------------------------------------------------------------*/
void Backend::vAppendExpr(String* sPipeline, QueryNode* pNode) {
  _static_

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // alias
  Stz *szAliasName = ProjectedColumn::szAliasName(pNode);

  if (szAliasName) {
    sPipeline->vCatSprint(
        " as %s"
        ""_Stz,
        szAliasName);
  }

  // get the expr node
  QueryNode *pExprNode = static_cast<QueryNode *>(GraphNav::pFindProperty("expr"_Stz, pNode));
  ASSERT(pExprNode);

  // Walk the expr node plan graph
  GraphNav::bGlobalWalkBottomUp(pExprNode, false, true, vVisitorExpr, (void *) sPipeline);

}

/*----------------------------------------------------------------------------*/
/* append aggregate Op to the pipeline                                        */
/*----------------------------------------------------------------------------*/
void Backend::vAppendAggregateOp(String* sPipeline, QueryNode* pNode)
{
  _static_

  bool bFirstExpr = false;

  // Get the list of projected expressions from ProjectOp
  QueryNode *pProjectedColumns = AggregateOp::pProjectedColumns(pNode);
  ASSERT(pProjectedColumns);

  // Iterate each projected expression
  if (pProjectedColumns->bHasChildren()) {

    sPipeline->vCatSprint(
        "    | aggreg 0 \""
        ""_Stz);

    // get a plan node iterator
    PlanNodeIter *pIter = pProjectedColumns->pNewChildrenIter();

    while (pIter->bHasNext()) {

      // append comma
      if (bFirstExpr)
        sPipeline->vCatSprint(" ,"_Stz);
      else
        bFirstExpr = true;

      // get the next projectedColumn from the iterator
      QueryNode *pProjectedColumn = static_cast<QueryNode *>(pIter->pNext());
      ASSERT(pProjectedColumn);
      ASSERT(ProjectedColumn::bIsOfType(pProjectedColumn));

      vAppendExpr(sPipeline, pProjectedColumn);
    }
    delete pIter;

    // if aggregOp , get a groupBy iterator
    if (AggregateOp::bIsOfType(pNode)) {

      // Get the groupBy from ProjectOp
      sPipeline->vCatSprint(" groupBy "_Stz);
      bFirstExpr = false;

      QueryNode *pGroupByList = AggregateOp::pGroupByList(pNode);
      PlanNodeIter *pGroupByIter = pGroupByList->pNewChildrenIter();
      while (pGroupByIter->bHasNext()) {

        // append comma
        if (bFirstExpr)
          sPipeline->vCatSprint(" ,"_Stz);
        else
          bFirstExpr = true;

        // get the next groupByExpr from the iterator
        QueryNode *pGroupByExpr = static_cast<QueryNode *>(pIter->pNext());
        ASSERT(pGroupByExpr);

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        // columnRef

        // TODO: Alka, please note that the pGroupByExpr may not necessarily
        //       be a columnRef. It could be any arbitrary expression.
        //       e.g. "GROUP BY (2 * colA / 3) % 18..."
        ASSERT(ColumnRef::bIsOfType(pGroupByExpr));
        QueryNode *pColumnRef = static_cast<QueryNode *>(pGroupByExpr);
        sPipeline->vCatSprint(
            " %s"
            ""_Stz,
            ColumnRef::szColumn(pColumnRef));
      }
      delete pGroupByIter;
    }
  }

  sPipeline->vCatSprint(" \" \n"_Stz);

}

/*----------------------------------------------------------------------------*/
/* append filter Op to the pipeline                                           */
/*----------------------------------------------------------------------------*/
void Backend::vAppendFilterOp(String* sPipeline, QueryNode* pNode)
{
  _static_

  // Get the list of projected expressions from ProjectOp
  QueryNode *pFilterExpr = FilterOp::pFilterExpr(pNode);
  ASSERT(pFilterExpr);


  sPipeline->vCatSprint(
      "    | filter 0 \""
      ""_Stz);

  // Walk the expr node plan graph
  GraphNav::bGlobalWalkBottomUp(pFilterExpr, false, true, vVisitorExpr, (void *) sPipeline);

  sPipeline->vCatSprint(" \" \n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* append project Op to the pipeline                                          */
/*----------------------------------------------------------------------------*/
void Backend::vAppendProjectOp(String* sPipeline, QueryNode* pNode)
{
  _static_

  bool bFirstExpr = false;

  // Get the list of projected expressions from ProjectOp
  QueryNode *pProjectedColumns = ProjectOp::pProjectedColumns(pNode);
  ASSERT(pProjectedColumns);

  // Iterate each projected expression
  if (pProjectedColumns->bHasChildren()) {

    sPipeline->vCatSprint(
        "    | project 0 \""
        ""_Stz);

    // get a plan node iterator
    PlanNodeIter *pIter = pProjectedColumns->pNewChildrenIter();

    while (pIter->bHasNext()) {

      // append comma
      if (bFirstExpr)
        sPipeline->vCatSprint(" ,"_Stz);
      else
        bFirstExpr = true;

      // get the next projectedColumn from the iterator
      QueryNode *pProjectedColumn = static_cast<QueryNode *>(pIter->pNext());
      ASSERT(pProjectedColumn);
      ASSERT(ProjectedColumn::bIsOfType(pProjectedColumn));

      vAppendExpr(sPipeline, pProjectedColumn);
    }
    delete pIter;

  }

  sPipeline->vCatSprint(" \" \n"_Stz);

}

/*----------------------------------------------------------------------------*/
/* append scan Op to the pipeline                                             */
/*----------------------------------------------------------------------------*/
void Backend::vAppendScanOp(String* sPipeline, QueryNode* pNode)
{
  _static_

  QueryNode *pTableRef = TableScanOp::pTableRef(pNode);
  Stz *szTableName = TableRef::szTable(pTableRef);

  sPipeline->vCatSprint(
      "    scan %s\n"
      ""_Stz,
      szTableName);
}

/*----------------------------------------------------------------------------*/
/* visitor function for the plan graph walk                                   */
/*----------------------------------------------------------------------------*/
GraphNav::eWalkCtrl Backend::vVisitorExpr(PlanNode* pExprNode, s256& sPath, i8 iDepth, void* pvArg)
{
  _static_
  UNUSED(sPath);
  UNUSED(iDepth);

  static bool bHasAgg = false;

  String* sPipeline = (String*)pvArg;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // aggregate
  if (AggregateExpr::bIsOfType(pExprNode)) {

    if (SumExpr::bIsOfType(pExprNode)) {

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // isDistinct
      bool bIsDistinct = AggregateExpr::bIsDistinct(pExprNode);

      sPipeline->vCatSprint(
          " sum "_Stz);

      // ALKA: to do in exprEval
      if (bIsDistinct) {
        sPipeline->vCatSprint(
            " distinct "_Stz);
      }

      bHasAgg = true;

    }
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // columnRef

  if (ColumnRef::bIsOfType(pExprNode)) {
    QueryNode *pColumnRef = static_cast<QueryNode *>(pExprNode);
    sPipeline->vCatSprint(
        " %s"
        ""_Stz,
        ColumnRef::szColumn(pColumnRef));
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // comparison operator

  if (ComparisonExpr::bIsOfType(pExprNode)) {

    if (EqExpr::bIsOfType(pExprNode)) {
      sPipeline->vCatSprint(" == "_Stz);
    }

    if (GeExpr::bIsOfType(pExprNode)) {
      sPipeline->vCatSprint(" >= "_Stz);
    }

    if (GtExpr::bIsOfType(pExprNode)) {
      sPipeline->vCatSprint(" > "_Stz);
    }

    if (LeExpr::bIsOfType(pExprNode)) {
      sPipeline->vCatSprint(" <= "_Stz);
    }

    if (LtExpr::bIsOfType(pExprNode)) {
      sPipeline->vCatSprint(" < "_Stz);
    }

    if (NeExpr::bIsOfType(pExprNode)) {
      sPipeline->vCatSprint(" != "_Stz);
    }

    sPipeline->vCatSprint(" <> "_Stz);

  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // literal

  if (LiteralExpr::bIsOfType(pExprNode)) {

    if (IntegerLiteral::bIsOfType(pExprNode)) {
      sPipeline->vCatSprint(" %lld "_Stz, IntegerLiteral::iValue(pExprNode));
    }
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // logical operator

  if (LogicExpr::bIsOfType(pExprNode)) {

    if (AndExpr::bIsOfType(pExprNode)) {
      sPipeline->vCatSprint(" and "_Stz);
    }

    if (IsNullExpr::bIsOfType(pExprNode)) {
      sPipeline->vCatSprint(" isNull "_Stz);
    }

    if (OrExpr::bIsOfType(pExprNode)) {
      sPipeline->vCatSprint(" or "_Stz);
    }

    if (XorExpr::bIsOfType(pExprNode)) {
      sPipeline->vCatSprint(" xor "_Stz);
    }

  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // matematical operator

  if (MathematicalExpr::bIsOfType(pExprNode)) {

    if (AdditionExpr::bIsOfType(pExprNode)) {
      sPipeline->vCatSprint(" + "_Stz);
    }

    if (DivisionExpr::bIsOfType(pExprNode)) {
      sPipeline->vCatSprint(" / "_Stz);
    }

    if (ModulusExpr::bIsOfType(pExprNode)) {
      sPipeline->vCatSprint(" % "_Stz);
    }

    if (MultiplicationExpr::bIsOfType(pExprNode)) {
      sPipeline->vCatSprint(" * "_Stz);
    }

    if (NegationExpr::bIsOfType(pExprNode)) {
      sPipeline->vCatSprint(" - "_Stz);
    }

    if (SubtractionExpr::bIsOfType(pExprNode)) {
      sPipeline->vCatSprint(" - "_Stz);
    }

  }

  // Keep walking
  return GraphNav::WALK_CONTINUE;
}

/*----------------------------------------------------------------------------*/
/* visitor function for the plan graph walk                                   */
/*----------------------------------------------------------------------------*/
GraphNav::eWalkCtrl Backend::vVisitorPlan(PlanNode* pRootNode, s256& sPath, i8 iDepth, void* pvArg)
{
  _static_

  QueryNode* pQueryNode = static_cast<QueryNode *>(pRootNode);

  // We are only interested in the /__root__/plan subtree. Skip the others.
  if (iDepth > 0 && !sPath.bBeginsWith(const_cast<Stz*>(C_PLAN_PATH))) {
    //return GraphNav::WALK_SKIP_CHILDREN;
    return GraphNav::WALK_CONTINUE;
  }

  // The root node can be useful for various graph navigation and manipulation.
  // e.g. finding other nodes.
  //RootNode* pPlanRoot = _pRootNode(pvArg);

  String* sPipeline = (String*)pvArg;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // SqlOperator

  if (SqlOperator::bIsOfType(pQueryNode)) {

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // AggregateOp

    if (AggregateOp::bIsOfType(pQueryNode)) {
      vAppendAggregateOp(sPipeline, pQueryNode);
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // FilterOp

    else if (FilterOp::bIsOfType(pQueryNode)) {
      vAppendFilterOp(sPipeline, pQueryNode);
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // ProjectOp

    else if (ProjectOp::bIsOfType(pQueryNode)) {
      if (ProjectOp::bHasAnAggregate(pQueryNode))
        vAppendAggregateOp(sPipeline, pQueryNode);
      else
        vAppendProjectOp(sPipeline, pQueryNode);
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // TableScanOp

    else if (TableScanOp::bIsOfType(pQueryNode)) {
      vAppendScanOp(sPipeline, pQueryNode);
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // other Op's
    else {
      RASSERT(0);
    }

  }

  // Keep walking
  return GraphNav::WALK_CONTINUE;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
