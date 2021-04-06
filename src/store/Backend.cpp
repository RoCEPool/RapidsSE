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
#include "LimitOp.hpp"
#include "PlanOperand.hpp"
#include "TableScanOp.hpp"
#include "QueryNode.hpp"

#include "TableRef.hpp"

#include "AggregateOp.hpp"
#include "SumExpr.hpp"

#include "ColumnOrdinal.hpp"
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

  // Find the /plan node
  m_pPlanRoot = pPlanRoot;

  m_pColExprList = nullptr;
  m_sPipeline = nullptr;

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
      cStageStart,
      cStageCurrent+1);

  ++ cStageCurrent;

  m_sPipeline->vCatSprint(
      "  stage stage%c on all partitions :\n"
      ""_Stz,
      cStageCurrent);


  // get the plan node of the plan
  PlanNode* pPlanNode = ShadowRoot::pPlanNode(m_pPlanRoot);
  ASSERT(pPlanNode);

  // The plan node should have one child which is a StatementMarker node
  ASSERT(pPlanNode->nChildren() == 1);
  ASSERT(StatementMarker::bIsOfType(pPlanNode->pFirstChild()));
  QueryNode* pStatementMarker = _pQueryNode(pPlanNode->pFirstChild());

  // We want to iterate over each logical subgraph, bounded by StatementMarkers,
  // and generate the pipeline for each statement
  GraphNav::bGlobalWalk(pStatementMarker, false, true, vVisitorPlan, (void *) this);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  m_sPipeline->vCatSprint(
      "    | stage%c\n"
      ""_Stz,
      cStageCurrent + 1);

  ++ cStageCurrent;

  // check if any aggregation in previous stage
//  char* sAggreg = m_sPipeline->azStr();

//  if (StrLib::bFind(&sAggreg, "aggreg")) {
//
//   // now append the aggreg as the super aggregate in the last stage (on origin)
//   s4K sAgg;
//   StrLib::vCopyTill(sAgg.azChar(), sAggreg, '|');
//
//    m_sPipeline->vCatSprint(
//        "  stage stage%c on origin :\n"
//        "    %s"
//        "    | output\n"
//        ""_Stz,
//        cStageCurrent, sAgg.azChar());
//  }
//  else {
    m_sPipeline->vCatSprint(
        "  stage stage%c on origin :\n"
        "    output\n"
        ""_Stz,
        cStageCurrent);
  //}
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
void Backend::vAppendExpr(Backend* pBackend, QueryNode* pNode) {
  _static_

  String* sPipeline = pBackend->m_sPipeline;

  // get the expr node
  QueryNode *pExprNode = static_cast<QueryNode *>(GraphNav::pFindProperty("expr"_Stz, pNode));
  ASSERT(pExprNode);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // type
  eCT eType = Expr::eType(pExprNode, pBackend->m_pPlanRoot);
  sPipeline->vCatSprint(
      " type %s"
      ""_Stz,
      aCT[eType]);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // alias
  Stz *szAliasName = ProjectedColumn::szAliasName(pNode);

  if (szAliasName) {
    sPipeline->vCatSprint(
        " as %s"
        ""_Stz,
        szAliasName);
  }
  else if (ColumnRef::bIsOfType(pExprNode)) {

    QueryNode *pColumnRef = static_cast<QueryNode *>(pExprNode);
    s256 sColumnName;
    sColumnName = ColumnRef::szColumn(pColumnRef);
    StrLib::vToLower(sColumnName.azChar());

    sPipeline->vCatSprint(
        " as %s"
        ""_Stz,
        sColumnName.szStz());
  }

  // Walk the expr node plan graph
  GraphNav::bGlobalWalkBottomUp(pExprNode, false, true, vVisitorExpr, (void *) pBackend);

}

/*----------------------------------------------------------------------------*/
/* append aggregate Op to the pipeline                                        */
/*----------------------------------------------------------------------------*/
void Backend::vAppendAggregateOp(Backend* pBackend, QueryNode* pNode)
{
  _static_

  bool bFirstExpr = false;
  String* sPipeline = pBackend->m_sPipeline;

  // Get the list of projected expressions from ProjectOp
  QueryNode *pProjectedColumns = AggregateOp::pProjectedColumns(pNode);
  ASSERT(pProjectedColumns);

  ASSERT(pNode->nChildren() == 1);
  ASSERT(PlanObject::bIsOfType(pNode->pFirstChild()));
  pBackend->m_pColExprList = PlanObject::pColumnExprs(_pQueryNode(pNode->pFirstChild()), pBackend->m_pPlanRoot);

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

      vAppendExpr(pBackend, pProjectedColumn);
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
//        ASSERT(ColumnRef::bIsOfType(pGroupByExpr));
//        QueryNode *pColumnRef = static_cast<QueryNode *>(pGroupByExpr);
//        sPipeline->vCatSprint(
//            " %s"
//            ""_Stz,
//            ColumnRef::szColumn(pColumnRef));
        vAppendExpr(pBackend, pGroupByExpr);
      }
      delete pGroupByIter;
    }
  }

  sPipeline->vCatSprint(" \" \n"_Stz);

}

/*----------------------------------------------------------------------------*/
/* append filter Op to the pipeline                                           */
/*----------------------------------------------------------------------------*/
void Backend::vAppendFilterOp(Backend* pBackend, QueryNode* pNode)
{
  _static_

  String* sPipeline = pBackend->m_sPipeline;

  // Get the list of projected expressions from ProjectOp
  //QueryNode *pFilterExpr = FilterOp::pFilterExpr(pNode);
  QueryNode *pOrdinalFilterExpr = FilterOp::pOrdinalFilterExpr(pNode, pBackend->m_pPlanRoot);

  ASSERT(pOrdinalFilterExpr);

  sPipeline->vCatSprint(
      "    | filter ordinal 0 \""
      ""_Stz);

  // Walk the expr node plan graph
  GraphNav::bGlobalWalkBottomUp(pOrdinalFilterExpr, false, true, vVisitorExpr, (void *) pBackend);
  delete pOrdinalFilterExpr;

  sPipeline->vCatSprint(" \" \n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* append limit Op to the pipeline                                            */
/*----------------------------------------------------------------------------*/
void Backend::vAppendLimitOp(Backend* pBackend, QueryNode* pNode)
{
  _static_

  String* sPipeline = pBackend->m_sPipeline;

  sPipeline->vCatSprint(
      "    | limit 0 %lld"
      ""_Stz,
      LimitOp::iLimit(pNode));

  i8 iOffset = LimitOp::iOffset(pNode);

  if (iOffset != C_UNKNOWN) {
    sPipeline->vCatSprint("offset %lld"_Stz, iOffset);
  }

  sPipeline->vCatSprint(" \n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* append project Op to the pipeline                                          */
/*----------------------------------------------------------------------------*/
void Backend::vAppendProjectOp(Backend* pBackend, QueryNode* pNode)
{
  _static_

  bool bFirstExpr = false;
  String* sPipeline = pBackend->m_sPipeline;

  // Get the list of projected expressions from ProjectOp
  QueryNode *pProjectedColumns = ProjectOp::pProjectedColumns(pNode);
  ASSERT(pProjectedColumns);

  ASSERT(pNode->nChildren() == 1);
  ASSERT(PlanObject::bIsOfType(pNode->pFirstChild()));
  pBackend->m_pColExprList = PlanObject::pColumnExprs(_pQueryNode(pNode->pFirstChild()), pBackend->m_pPlanRoot);

/*
  QueryNode *pOrdinalProjectedColumns = ProjectOp::pOrdinalProjectedColumns(pNode, pBackend->m_pPlanRoot);
  ASSERT(pOrdinalProjectedColumns);

  // Iterate each projected expression
  if (pOrdinalProjectedColumns->bHasChildren()) {

    sPipeline->vCatSprint(
        "    | project ordinal 0 \""
        ""_Stz);

    // get a plan node iterator
    PlanNodeIter *pIter = pOrdinalProjectedColumns->pNewChildrenIter();

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

      vAppendExpr(pBackend, pProjectedColumn);
    }
    delete pIter;

  }
  */

// Iterate each projected expression
  if (pProjectedColumns->bHasChildren()) {

    sPipeline->vCatSprint(
        "    | project ordinal 0 \""
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

      vAppendExpr(pBackend, pProjectedColumn);
    }
    delete pIter;

  }

  //delete pOrdinalProjectedColumns;

  sPipeline->vCatSprint(" \" \n"_Stz);

}

/*----------------------------------------------------------------------------*/
/* append scan Op to the pipeline                                             */
/*----------------------------------------------------------------------------*/
void Backend::vAppendScanOp(Backend* pBackend, QueryNode* pNode)
{
  _static_

  String* sPipeline = pBackend->m_sPipeline;

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

  QueryNode* pQueryNode = static_cast<QueryNode *>(pExprNode);

  Backend* pBackend = (Backend*)pvArg;
  String* sPipeline = pBackend->m_sPipeline;

  QueryNode* pOrdinalExpr;

  if (ColumnRef::bIsOfType(pQueryNode))
    pOrdinalExpr = Expr::pSymbolicToOrdinal(pQueryNode, pBackend->m_pColExprList, pBackend->m_pPlanRoot);
  else
    pOrdinalExpr = pQueryNode;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // aggregate
  if (AggregateExpr::bIsOfType(pOrdinalExpr)) {

    if (SumExpr::bIsOfType(pOrdinalExpr)) {

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // isDistinct
      bool bIsDistinct = AggregateExpr::bIsDistinct(pOrdinalExpr);

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

  if (ColumnOrdinal::bIsOfType(pOrdinalExpr)) {
    QueryNode* pColumnOrdinal = _pQueryNode(pOrdinalExpr);

      sPipeline->vCatSprint(
        " $%lld"
        ""_Stz,
        ColumnOrdinal::iOrdinal(pColumnOrdinal));
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // comparison operator

  if (ComparisonExpr::bIsOfType(pOrdinalExpr)) {

    if (EqExpr::bIsOfType(pOrdinalExpr)) {
      sPipeline->vCatSprint(" == "_Stz);
    }

    if (GeExpr::bIsOfType(pOrdinalExpr)) {
      sPipeline->vCatSprint(" >= "_Stz);
    }

    if (GtExpr::bIsOfType(pOrdinalExpr)) {
      sPipeline->vCatSprint(" > "_Stz);
    }

    if (LeExpr::bIsOfType(pOrdinalExpr)) {
      sPipeline->vCatSprint(" <= "_Stz);
    }

    if (LtExpr::bIsOfType(pOrdinalExpr)) {
      sPipeline->vCatSprint(" < "_Stz);
    }

    if (NeExpr::bIsOfType(pOrdinalExpr)) {
      sPipeline->vCatSprint(" != "_Stz);
    }

    sPipeline->vCatSprint(" <> "_Stz);

  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // literal

  if (LiteralExpr::bIsOfType(pOrdinalExpr)) {

    if (IntegerLiteral::bIsOfType(pOrdinalExpr)) {
      sPipeline->vCatSprint(" %lld "_Stz, IntegerLiteral::iValue(pOrdinalExpr));
    }
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // logical operator

  if (LogicExpr::bIsOfType(pOrdinalExpr)) {

    if (AndExpr::bIsOfType(pOrdinalExpr)) {
      sPipeline->vCatSprint(" and "_Stz);
    }

    if (IsNullExpr::bIsOfType(pOrdinalExpr)) {
      sPipeline->vCatSprint(" isNull "_Stz);
    }

    if (OrExpr::bIsOfType(pOrdinalExpr)) {
      sPipeline->vCatSprint(" or "_Stz);
    }

    if (XorExpr::bIsOfType(pOrdinalExpr)) {
      sPipeline->vCatSprint(" xor "_Stz);
    }

  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // matematical operator

  if (MathematicalExpr::bIsOfType(pOrdinalExpr)) {

    if (AdditionExpr::bIsOfType(pOrdinalExpr)) {
      sPipeline->vCatSprint(" + "_Stz);
    }

    if (DivisionExpr::bIsOfType(pOrdinalExpr)) {
      sPipeline->vCatSprint(" / "_Stz);
    }

    if (ModulusExpr::bIsOfType(pOrdinalExpr)) {
      sPipeline->vCatSprint(" % "_Stz);
    }

    if (MultiplicationExpr::bIsOfType(pOrdinalExpr)) {
      sPipeline->vCatSprint(" * "_Stz);
    }

    if (NegationExpr::bIsOfType(pOrdinalExpr)) {
      sPipeline->vCatSprint(" - "_Stz);
    }

    if (SubtractionExpr::bIsOfType(pOrdinalExpr)) {
      sPipeline->vCatSprint(" - "_Stz);
    }

  }

  // Keep walking
  return GraphNav::WALK_CONTINUE;
}

/*----------------------------------------------------------------------------*/
/* visitor function for the plan graph walk                                   */
/*----------------------------------------------------------------------------*/
GraphNav::eWalkCtrl Backend::vVisitorStatement(PlanNode* pRootNode, s256& sPath, i8 iDepth, void* pvArg)
{
  _static_

  QueryNode* pQueryNode = static_cast<QueryNode *>(pRootNode);

  // We are only interested in the /__root__/plan subtree. Skip the others.
//  if (iDepth > 0 && !sPath.bBeginsWith(const_cast<Stz*>(C_PLAN_PATH))) {
//    //return GraphNav::WALK_SKIP_CHILDREN;
//    return GraphNav::WALK_CONTINUE;
//  }

  // The root node can be useful for various graph navigation and manipulation.
  // e.g. finding other nodes.
  //RootNode* pPlanRoot = _pRootNode(pvArg);

  Backend* pBackend = (Backend*)pvArg;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // SqlOperator

  if (SqlOperator::bIsOfType(pQueryNode)) {

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // AggregateOp

    if (AggregateOp::bIsOfType(pQueryNode)) {
      vAppendAggregateOp(pBackend, pQueryNode);
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // FilterOp

    else if (FilterOp::bIsOfType(pQueryNode)) {
      vAppendFilterOp(pBackend, pQueryNode);
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // LimitOp

    else if (LimitOp::bIsOfType(pQueryNode)) {
      vAppendLimitOp(pBackend, pQueryNode);
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // ProjectOp

    else if (ProjectOp::bIsOfType(pQueryNode)) {
      if (ProjectOp::bHasAnAggregate(pQueryNode))
        vAppendAggregateOp(pBackend, pQueryNode);
      else
        vAppendProjectOp(pBackend, pQueryNode);
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // TableScanOp

    else if (TableScanOp::bIsOfType(pQueryNode)) {
      vAppendScanOp(pBackend, pQueryNode);
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

/*----------------------------------------------------------------------------*/
/* visitor function for the plan graph walk                                   */
/*----------------------------------------------------------------------------*/
GraphNav::eWalkCtrl Backend::vVisitorPlan(PlanNode* pRootNode, s256& sPath, i8 iDepth, void* pvArg)
{
  _static_

  if (!StatementMarker::bIsOfType(pRootNode)) {
    return GraphNav::WALK_CONTINUE;   // Keep walking
  }

  QueryNode* pStatementMarker = _pQueryNode(pRootNode);

  Backend* pBackend = (Backend*)pvArg;

  // Walk the current node and (potenitially) its properties and children,
  // calling the visitor function for each node encountered. This is a
  // bottom-up, depth-first traversal.
  // The bounded walk will stop when it hits a StatementMarker node as this
  // represents the start of another logical sub-graph.
  // Starting on a StatementMarker will walk until the subsequent StatementMarker
  // node is encountered.
  GraphNav::bBoundedWalkBottomUp(pStatementMarker, false, true, vVisitorStatement, (void *) pBackend);

  // Keep walking
  return GraphNav::WALK_CONTINUE;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
