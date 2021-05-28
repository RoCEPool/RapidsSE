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
#include "StageMarker.hpp"
#include "ExplainMarker.hpp"
#include "ExplainOp.hpp"
#include "PlanConstants.hpp"
#include "GraphNav.hpp"
#include "LimitOp.hpp"
#include "PlanOperand.hpp"
#include "TableScanOp.hpp"
#include "QueryNode.hpp"

#include "TableRef.hpp"
#include "AliasRef.hpp"

#include "AggregateOp.hpp"
#include "AvgExpr.hpp"
#include "CountExpr.hpp"
#include "MinExpr.hpp"
#include "MaxExpr.hpp"
#include "SumExpr.hpp"

#include "ColumnOrdinal.hpp"
#include "ColumnRef.hpp"
#include "Expr.hpp"
#include "FilterOp.hpp"

#include "BooleanLiteral.hpp"
#include "DateLiteral.hpp"
#include "FloatLiteral.hpp"
#include "IntegerLiteral.hpp"
#include "NullLiteral.hpp"
#include "StringLiteral.hpp"
#include "TimeLiteral.hpp"
#include "TimestampLiteral.hpp"

#include "Literal.hpp"
#include "OrderOp.hpp"
#include "OrderItem.hpp"
#include "ProjectedColumn.hpp"
#include "ProjectOp.hpp"
#include "UnionOp.hpp"

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

#include "SP_Dte.hpp"
#include "SP_Tme.hpp"
#include "SP_Tsp.hpp"

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
  if (m_pColExprList) {
    m_pColExprList->vDeleteAll();
    delete m_pColExprList;
  }
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
  //vPrintPlanGraph(m_pPlanRoot);
//#endif

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // get the plan node of the plan
  PlanNode* pPlanNode = ShadowRoot::pPlanNode(m_pPlanRoot);
  ASSERT(pPlanNode);

  QueryNode* pStartingNode = _pQueryNode(pPlanNode->pFirstChild());

  // Skip over any explain markers and ExplainOps at the top
  while (ExplainMarker::bIsOfType(pStartingNode) || ExplainOp::bIsOfType(pStartingNode)) {
    ASSERT(pStartingNode);
    pStartingNode = _pQueryNode(pStartingNode->pFirstChild());
  }

  // The plan node should have one child which is a StageMarker node
  ASSERT(pPlanNode->nChildren() == 1);
  ASSERT(StageMarker::bIsOfType(pStartingNode));
  QueryNode* pStageMarker = _pQueryNode(pStartingNode);

  vAppendStartStage();

  // We want to iterate over each logical subgraph, bounded by StageMarkers,
  // and generate the pipeline for each statement
  vAppendPipe();
  vAppendNextOp(pStageMarker->szName());
  GraphNav::bGlobalWalk(pStageMarker, false, true, vVisitorPlan, (void *) this);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  vAppendLastStage();

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
void Backend::vAppendExpr(Backend* pBackend, QueryNode* pExprNode, Stz* szAliasName) {
  _static_

  String* sPipeline = pBackend->m_sPipeline;

  // get the expr node
  //QueryNode *pExprNode = static_cast<QueryNode *>(GraphNav::pFindProperty("expr"_Stz, pNode));
  //ASSERT(pExprNode);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // type
  eCT eType = Expr::eType(pExprNode, pBackend->m_pPlanRoot);
  sPipeline->vCatSprint(
      " type %s"
      ""_Stz,
      aCT[eType]);

  if (szAliasName) {
    sPipeline->vCatSprint(
        " as %s"
        ""_Stz,
        szAliasName);
  }

//  if (szAliasName) {
//    sPipeline->vCatSprint(
//        " as %s"
//        ""_Stz,
//        szAliasName);
//  }
//  else if (ColumnRef::bIsOfType(pExprNode)) {
//
//    QueryNode *pColumnRef = static_cast<QueryNode *>(pExprNode);
//    s256 sColumnName;
//    sColumnName = ColumnRef::szColumn(pColumnRef);
//    StrLib::vToLower(sColumnName.azChar());
//
//    sPipeline->vCatSprint(
//        " as %s"
//        ""_Stz,
//        sColumnName.szStz());
//  }

  // Walk the expr node plan graph
  //GraphNav::bGlobalWalkBottomUp(pExprNode, false, true, vVisitorExpr, (void *) pBackend);
  GraphNav::bGlobalWalk(pExprNode, false, true,vVisitorExpr, vVisitorExpr, (void *) pBackend);
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
  if (pBackend->m_pColExprList) {
    pBackend->m_pColExprList->vDeleteAll();
    delete pBackend->m_pColExprList;
  }
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

      // get the expr node
      QueryNode *pExprNode = static_cast<QueryNode *>(GraphNav::pFindProperty("expr"_Stz, pProjectedColumn));
      ASSERT(pExprNode);

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // alias
      Stz *szAliasName = ""_Stz;
      szAliasName = ProjectedColumn::szAliasName(pProjectedColumn);

      if ( ! szAliasName) {
        if (ColumnRef::bIsOfType(pExprNode)) {
          QueryNode *pColumnRef = static_cast<QueryNode *>(pExprNode);
          szAliasName = ColumnRef::szColumn(pColumnRef);
          StrLib::vToLower(szAliasName->azChar());
        }
      }

      vAppendExpr(pBackend, pExprNode, szAliasName);
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
        QueryNode *pGroupByExpr = static_cast<QueryNode *>(pGroupByIter->pNext());
        ASSERT(pGroupByExpr);

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        // alias
        Stz *szAliasName = ""_Stz;
        //szAliasName = ProjectedColumn::szAliasName(pGroupByExpr);

        if (ColumnRef::bIsOfType(pGroupByExpr)) {
          QueryNode *pColumnRef = static_cast<QueryNode *>(pGroupByExpr);
          // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
          // columnRef
          vAppendExpr(pBackend, pColumnRef, nullptr);
        }
        else if (AliasRef::bIsOfType(pGroupByExpr)) {
          QueryNode *pAliasRef = static_cast<QueryNode *>(pGroupByExpr);
          // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
          // columnRef
          vAppendExpr(pBackend, pAliasRef, nullptr);
        }
        else
          vAppendExpr(pBackend, pGroupByExpr, nullptr);
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
  QueryNode *pOrdinalFilterExpr = FilterOp::pOrdinalFilterExpr(pNode, pBackend->m_pPlanRoot);

  ASSERT(pOrdinalFilterExpr);

  sPipeline->vCatSprint(
      "    | filter 1 \""
      ""_Stz);

  // Walk the expr node plan graph
  //GraphNav::bGlobalWalkBottomUp(pOrdinalFilterExpr, false, true, vVisitorExpr, (void *) pBackend);
  GraphNav::bGlobalWalk(pOrdinalFilterExpr, false, true,vVisitorExpr, vVisitorExpr, (void *) pBackend);
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
      "    | limit 1 %lld"
      ""_Stz,
      LimitOp::iLimit(pNode));

  i8 iOffset = LimitOp::iOffset(pNode);

  if (iOffset != C_UNKNOWN) {
    sPipeline->vCatSprint("offset %lld"_Stz, iOffset);
  }

  sPipeline->vCatSprint(" \n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* append orderBy Op to the pipeline                                          */
/*----------------------------------------------------------------------------*/
void Backend::vAppendOrderByOp(Backend* pBackend, QueryNode* pNode)
{
  _static_

  bool bFirstExpr = false;
  String* sPipeline = pBackend->m_sPipeline;

  // Get the list of projected expressions from ProjectOp
  ASSERT(pNode->nChildren() == 1);
  QueryNode *pOrderItemList = OrderOp::pOrderItemList(pNode);
  ASSERT(pOrderItemList);

  ASSERT(PlanObject::bIsOfType(pNode->pFirstChild()));
  if (pBackend->m_pColExprList) {
    pBackend->m_pColExprList->vDeleteAll();
    delete pBackend->m_pColExprList;
  }
  pBackend->m_pColExprList = PlanObject::pColumnExprs(_pQueryNode(pNode->pFirstChild()), pBackend->m_pPlanRoot);

  // Iterate each projected expression
  if (pOrderItemList->bHasChildren()) {

    sPipeline->vCatSprint(
        "    | orderBy 0 \""
        ""_Stz);

    // get a plan node iterator
    PlanNodeIter *pIter = pOrderItemList->pNewChildrenIter();

    while (pIter->bHasNext()) {

      // append comma
      if (bFirstExpr)
        sPipeline->vCatSprint(" ,"_Stz);
      else
        bFirstExpr = true;

      // get the next projectedColumn from the iterator
      QueryNode *pOrderItem = static_cast<QueryNode *>(pIter->pNext());
      ASSERT(pOrderItem);
      ASSERT(OrderItem::bIsOfType(pOrderItem));

      // get the expr node
      QueryNode *pExprNode = static_cast<QueryNode *>(GraphNav::pFindProperty("expr"_Stz, pOrderItem));
      ASSERT(pExprNode);

      vAppendExpr(pBackend, pExprNode, ""_Stz);

      sPipeline->vCatSprint(" %s "_Stz, OrderItem::bIsAscending(pOrderItem) ? "ASC" : "DESC");
    }
    delete pIter;

  }

  sPipeline->vCatSprint(" \" \n"_Stz);

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
  ASSERT(pNode->nChildren() == 1);
  QueryNode *pProjectedColumns = ProjectOp::pProjectedColumns(pNode);
  ASSERT(pProjectedColumns);

  ASSERT(PlanObject::bIsOfType(pNode->pFirstChild()));
  if (pBackend->m_pColExprList) {
    pBackend->m_pColExprList->vDeleteAll();
    delete pBackend->m_pColExprList;
  }
  pBackend->m_pColExprList = PlanObject::pColumnExprs(_pQueryNode(pNode->pFirstChild()), pBackend->m_pPlanRoot);

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

      // get the expr node
      QueryNode *pExprNode = static_cast<QueryNode *>(GraphNav::pFindProperty("expr"_Stz, pProjectedColumn));
      ASSERT(pExprNode);

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // alias
      Stz *szAliasName = ProjectedColumn::szAliasName(pProjectedColumn);
      if ( ! szAliasName) {
        if (ColumnRef::bIsOfType(pExprNode)) {
          QueryNode *pColumnRef = static_cast<QueryNode *>(pExprNode);
          szAliasName = ColumnRef::szColumn(pColumnRef);
          StrLib::vToLower(szAliasName->azChar());
        }
      }

      vAppendExpr(pBackend, pExprNode, szAliasName);
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
  Stz *szCatalogName   = TableRef::szCatalog(pTableRef);
  Stz *szSchemaName    = TableRef::szSchema(pTableRef);
  Stz *szTableName     = TableRef::szTable(pTableRef);

  sPipeline->vCatSprint(
      "    scan %s %s %s\n"
      ""_Stz,
      szCatalogName, szSchemaName, szTableName);
}

/*----------------------------------------------------------------------------*/
/* append union Op to the pipeline                                            */
/*----------------------------------------------------------------------------*/
void Backend::vAppendUnionOp(Backend* pBackend, QueryNode* pUnionNode)
{
  _static_

  // this node should have two children
  ASSERT(pUnionNode->nChildren() == 2);

  // get the child markers
  QueryNode *pChildStageMarker1 = _pQueryNode(pUnionNode->pFirstChild());
  QueryNode *pChildStageMarker2 = _pQueryNode(pUnionNode->pSecondChild());

  pBackend->vAppendNextOp(pChildStageMarker1->szName());
  pBackend->vAppendPipe();
  pBackend->vAppendNextOp(pChildStageMarker2->szName());

  // bounded walk of each of first children
  GraphNav::bGlobalWalk(pChildStageMarker1, false, true, vVisitorPlan, (void *) pBackend);

  // bounded walk of each of first children
  GraphNav::bGlobalWalk(pChildStageMarker2, false, true, vVisitorPlan, (void *) pBackend);
}

/*----------------------------------------------------------------------------*/
/* visitor function for the plan graph walk                                   */
/*----------------------------------------------------------------------------*/
GraphNav::eWalkCtrl Backend::vVisitorExpr(PlanNode* pExprNode, s256& sPath, i8 iDepth, GraphNav::eWalkEvent eEventType, void* pvArg)
{
  _static_
  UNUSED(sPath);
  UNUSED(iDepth);

  static bool bHasAgg = false;

  QueryNode* pQueryNode = static_cast<QueryNode *>(pExprNode);

  Backend* pBackend = (Backend*)pvArg;
  String* sPipeline = pBackend->m_sPipeline;

//  vTrace("Got %s event for node path: %s\n"_Stz,
//           eEventType == Parser::GraphNav::EVENT_ENTER ? "ENTER" : "EXIT",
//           sPath.szStz());

  QueryNode* pOrdinalExpr = nullptr;
  b1 bDeleteOrdExpr = false;

  QueryNode* pAliasedNode;
  if (AliasRef::bIsOfType(pQueryNode)) {
    pAliasedNode = AliasRef::pAliasedExpr(pQueryNode);
    if (ColumnRef::bIsOfType(pAliasedNode)) {
      pOrdinalExpr = Expr::pSymbolicToOrdinal(pAliasedNode, pBackend->m_pColExprList, pBackend->m_pPlanRoot);
      bDeleteOrdExpr = true;
    } else
      pOrdinalExpr = pAliasedNode;
  }
  else {
//    if (ColumnRef::bIsOfType(pQueryNode)) {
//      pOrdinalExpr = Expr::pSymbolicToOrdinal(pQueryNode, pBackend->m_pColExprList, pBackend->m_pPlanRoot);
//      bDeleteOrdExpr = true;
//    } else
//      pOrdinalExpr = pQueryNode;

    if (pBackend->m_pColExprList) {
      pOrdinalExpr = Expr::pSymbolicToOrdinal(pQueryNode, pBackend->m_pColExprList, pBackend->m_pPlanRoot);
      bDeleteOrdExpr = true;
    }
    if ( ! pOrdinalExpr )
      pOrdinalExpr = pQueryNode;
  }

  if (eEventType == Parser::GraphNav::EVENT_ENTER) {

    if (AggregateExpr::bIsOfType(pOrdinalExpr)) {
      sPipeline->vCatSprint(" ) "_Stz);
    }

    if (bDeleteOrdExpr)
      delete pOrdinalExpr;

    return GraphNav::WALK_CONTINUE;
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
  // aggregate
  if (AggregateExpr::bIsOfType(pOrdinalExpr)) {

    if (AvgExpr::bIsOfType(pOrdinalExpr)) {

      RASSERT(0);
      bHasAgg = true;

    }

    else if (CountExpr::bIsOfType(pOrdinalExpr)) {

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // isCountAll
      bool bIsCountAll = CountExpr::bIsCountAll(pOrdinalExpr);

      if (bIsCountAll) {
        sPipeline->vCatSprint(
            " countAll "_Stz);
      }
      else {
        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        // isDistinct
        bool bIsDistinct = AggregateExpr::bIsDistinct(pOrdinalExpr);

        sPipeline->vCatSprint(
            " count "_Stz);

        // ALKA: to do in exprEval
        if (bIsDistinct) {
          sPipeline->vCatSprint(
              " distinct "_Stz);
        }
      }

      bHasAgg = true;

    }

    else if (MinExpr::bIsOfType(pOrdinalExpr)) {

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // isDistinct
      bool bIsDistinct = AggregateExpr::bIsDistinct(pOrdinalExpr);

      sPipeline->vCatSprint(
          " min "_Stz);

      // ALKA: to do in exprEval
      if (bIsDistinct) {
        sPipeline->vCatSprint(
            " distinct "_Stz);
      }

      bHasAgg = true;

    }

    else if (MaxExpr::bIsOfType(pOrdinalExpr)) {

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // isDistinct
      bool bIsDistinct = AggregateExpr::bIsDistinct(pOrdinalExpr);

      sPipeline->vCatSprint(
          " max "_Stz);

      // ALKA: to do in exprEval
      if (bIsDistinct) {
        sPipeline->vCatSprint(
            " distinct "_Stz);
      }

      bHasAgg = true;

    }

    else if (SumExpr::bIsOfType(pOrdinalExpr)) {

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

    else {
      NEVER_REACH;
    }

    if (bDeleteOrdExpr)
      delete pOrdinalExpr;
    return GraphNav::WALK_CONTINUE;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // comparison operator

  if (ComparisonExpr::bIsOfType(pOrdinalExpr)) {

    if (EqExpr::bIsOfType(pOrdinalExpr)) {
      sPipeline->vCatSprint(" == "_Stz);
    }

    else if (GeExpr::bIsOfType(pOrdinalExpr)) {
      sPipeline->vCatSprint(" >= "_Stz);
    }

    else if (GtExpr::bIsOfType(pOrdinalExpr)) {
      sPipeline->vCatSprint(" > "_Stz);
    }

    else if (LeExpr::bIsOfType(pOrdinalExpr)) {
      sPipeline->vCatSprint(" <= "_Stz);
    }

    else if (LtExpr::bIsOfType(pOrdinalExpr)) {
      sPipeline->vCatSprint(" < "_Stz);
    }

    else if (NeExpr::bIsOfType(pOrdinalExpr)) {
      sPipeline->vCatSprint(" != "_Stz);
    }
    else {
      NEVER_REACH;
    }

    if (bDeleteOrdExpr)
      delete pOrdinalExpr;
    return GraphNav::WALK_CONTINUE;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // literal

  if (LiteralExpr::bIsOfType(pOrdinalExpr)) {

    if (BooleanLiteral::bIsOfType(pOrdinalExpr)) {
      sPipeline->vCatSprint(" %lld "_Stz, BooleanLiteral::bValue(pOrdinalExpr));
    }

    if (DateLiteral::bIsOfType(pOrdinalExpr)) {
      SP_Dte spDte;
      sPipeline->vCatSprint(" DATE '%s' "_Stz, (char*)spDte.sz(DateLiteral::xValue(pOrdinalExpr)));
    }

    if (FloatLiteral::bIsOfType(pOrdinalExpr)) {
      sPipeline->vCatSprint(" %g "_Stz, FloatLiteral::dValue(pOrdinalExpr));
    }

    if (IntegerLiteral::bIsOfType(pOrdinalExpr)) {
      sPipeline->vCatSprint(" %lld "_Stz, IntegerLiteral::iValue(pOrdinalExpr));
    }

    if (NullLiteral::bIsOfType(pOrdinalExpr)) {
      sPipeline->vCatSprint(" null "_Stz);
    }

    if (StringLiteral::bIsOfType(pOrdinalExpr)) {
      sPipeline->vCatSprint(" '%s' "_Stz, StringLiteral::szValue(pOrdinalExpr));
    }

    if (TimeLiteral::bIsOfType(pOrdinalExpr)) {
      SP_Tme spTme;
      sPipeline->vCatSprint(" TIME '%s' "_Stz, (char*)spTme.sz(TimeLiteral::xValue(pOrdinalExpr)));
    }

    if (TimestampLiteral::bIsOfType(pOrdinalExpr)) {
      SP_Tsp spTsp;
      sPipeline->vCatSprint(" TIMESTAMP '%s' "_Stz, (char*)spTsp.sz(TimestampLiteral::xValue(pOrdinalExpr)));
    }

    if (bDeleteOrdExpr)
      delete pOrdinalExpr;
    return GraphNav::WALK_CONTINUE;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // logical operator

  if (LogicExpr::bIsOfType(pOrdinalExpr)) {

    if (AndExpr::bIsOfType(pOrdinalExpr)) {
      sPipeline->vCatSprint(" and "_Stz);
    }

    else if (IsNullExpr::bIsOfType(pOrdinalExpr)) {
      sPipeline->vCatSprint(" isNull "_Stz);
    }

    else if (NotExpr::bIsOfType(pOrdinalExpr)) {
      sPipeline->vCatSprint(" not "_Stz);
    }

    else if (OrExpr::bIsOfType(pOrdinalExpr)) {
      sPipeline->vCatSprint(" or "_Stz);
    }

    else if (XorExpr::bIsOfType(pOrdinalExpr)) {
      sPipeline->vCatSprint(" xor "_Stz);
    }
    else {
      NEVER_REACH;
    }

    if (bDeleteOrdExpr)
      delete pOrdinalExpr;
    return GraphNav::WALK_CONTINUE;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // matematical operator

  if (MathematicalExpr::bIsOfType(pOrdinalExpr)) {

    if (AdditionExpr::bIsOfType(pOrdinalExpr)) {
      sPipeline->vCatSprint(" + "_Stz);
    }

    else if (DivisionExpr::bIsOfType(pOrdinalExpr)) {
      sPipeline->vCatSprint(" / "_Stz);
    }

    else if (ModulusExpr::bIsOfType(pOrdinalExpr)) {
      sPipeline->vCatSprint(" %% "_Stz);
    }

    else if (MultiplicationExpr::bIsOfType(pOrdinalExpr)) {
      sPipeline->vCatSprint(" * "_Stz);
    }

    else if (NegationExpr::bIsOfType(pOrdinalExpr)) {
      sPipeline->vCatSprint(" negate "_Stz);
    }

    else if (SubtractionExpr::bIsOfType(pOrdinalExpr)) {
      sPipeline->vCatSprint(" - "_Stz);
    }
    else {
      NEVER_REACH;
    }

    if (bDeleteOrdExpr)
      delete pOrdinalExpr;
    return GraphNav::WALK_CONTINUE;
  }

  // Keep walking
  if (bDeleteOrdExpr)
    delete pOrdinalExpr;
  return GraphNav::WALK_CONTINUE;
}

/*----------------------------------------------------------------------------*/
/* visitor function for the plan graph walk                                   */
/*----------------------------------------------------------------------------*/
GraphNav::eWalkCtrl Backend::vVisitorStatement(PlanNode* pRootNode, s256& sPath, i8 iDepth, void* pvArg)
{
  _static_

  QueryNode* pQueryNode = static_cast<QueryNode *>(pRootNode);

  Backend* pBackend = (Backend*)pvArg;

  if (iDepth == 0) {
    pBackend->vAppendPipe();
    pBackend->vAppendNextOp("stageLast"_Stz);
  }

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
    // UnionOp

    else if (UnionOp::bIsOfType(pQueryNode)) {
      RASSERT(0);
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // other Op's
    else {
      vAppendOrderByOp(pBackend, pQueryNode);
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

  // Continue walking till a StageMarker is hit
  if (!StageMarker::bIsOfType(pRootNode)) {
    return GraphNav::WALK_CONTINUE;
  }

  // get the stageMarker
  QueryNode* pStageMarker = _pQueryNode(pRootNode);

  // get the backend
  Backend* pBackend = (Backend*)pvArg;

  // check for a unionOp
  auto fHasUnionOp = [](PlanNode* pNode, void* pvArg) -> b1 {
    return UnionOp::bIsExactType(pNode);
  };

  // check for a unionOp
  PlanNode *pHasUnionOp = GraphNav::pBoundedSearch(pStageMarker, false, true, fHasUnionOp, nullptr);

  // we have a unionOp
  if (pHasUnionOp) {
    pBackend->vAppendNextStage(pStageMarker->szName());

    QueryNode *pUnionOp = static_cast<QueryNode *>(pHasUnionOp);
    pBackend->vAppendNextOp(pUnionOp->szName());

    // previos stage to pipe to union stage
    pBackend->vAppendNextStage(pUnionOp->szName());
    vAppendUnionOp(pBackend, pUnionOp);
    return GraphNav::WALK_SKIP_CHILDREN;
  }

  pBackend->vAppendNextStage(pStageMarker->szName());

  // Walk the current node and its children,
  // calling the visitor function for each node encountered. This is a
  // bottom-up, depth-first traversal.
  // Starting on a StageMarker will walk until the subsequent StageMarker
  // node is encountered.
  GraphNav::bBoundedWalkBottomUp(pStageMarker, false, true, vVisitorStatement, (void *) pBackend);

  // Keep walking
  return GraphNav::WALK_CONTINUE;
}

/*----------------------------------------------------------------------------*/
/* append last stage to the pipeline                                          */
/*----------------------------------------------------------------------------*/
void Backend::vAppendLastStage(void) {

  m_sPipeline->vCatSprint(
      "  stage stageLast on origin :\n"
      "    output\n"
      ""_Stz);

  m_sPipeline->vCatSprint(
      "    | sink\n"
      "    | startStage\n"
      ""_Stz);

  m_sPipeline->vConcat(
      "end ;\n"
      ""_Stz);

  //vTrace("%s\n"_Stz, m_sPipeline->azStr());
}

/*----------------------------------------------------------------------------*/
/* append next stage to the pipeline                                          */
/*----------------------------------------------------------------------------*/
void Backend::vAppendNextStage(Stz* szStage) {

  m_sPipeline->vCatSprint(
      "  stage %s on all partitions :\n"
      ""_Stz,
      szStage);
}

/*----------------------------------------------------------------------------*/
/* append extra pipe pipeline                                                 */
/*----------------------------------------------------------------------------*/
void Backend::vAppendPipe(void) {

  m_sPipeline->vCatSprint(
      "    | "_Stz);
}

/*----------------------------------------------------------------------------*/
/* append extra pipe pipeline                                                 */
/*----------------------------------------------------------------------------*/
void Backend::vAppendNextOp(Stz* szNextStage) {

  m_sPipeline->vCatSprint(
      "    %s\n"
      ""_Stz,
      szNextStage);
}

/*----------------------------------------------------------------------------*/
/* append the start stage to the pipeline                                     */
/*----------------------------------------------------------------------------*/
void Backend::vAppendStartStage(void) {

  m_sPipeline->vCopy(
      "pipeline unnamed\n"
      "begin\n"
      ""_Stz);

  m_sPipeline->vCatSprint(
      "  stage startStage on origin :\n"
      "    start\n"
      "    | periodic once\n"
      ""_Stz
      );
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
