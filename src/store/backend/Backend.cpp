/*============================================================================*/
/* Backend.cpp -- Backend (plan to pipeline transliterate)                    */
/*============================================================================*/

#include "BASE.h"

#include "eYX.h"

#include "Grid.hpp"
#include "OpSpec.hpp"
#include "VertexSpec.hpp"
#include "Pipeline.hpp"
#include "Backend.hpp"
#include "Job.hpp"
#include "StageSpec.hpp"

#include "TagConstants.h"

#include "SP_TknVecStr.hpp"

#include "SqlOperator.hpp"
#include "ShadowNode.hpp"
#include "ShadowRoot.hpp"
#include "store/parser/shadowNodes/planObjects/planMarkers/StageMarker.hpp"
#include "store/parser/shadowNodes/planObjects/planMarkers/statementMarkers/dml/ExplainMarker.hpp"
#include "store/parser/shadowNodes/planObjects/planMarkers/subqueryMarkers/ExprSubquery.hpp"
#include "store/parser/shadowNodes/planObjects/planMarkers/subqueryMarkers/SubqueryMarker.hpp"
#include "store/parser/shadowNodes/planObjects/planMarkers/subqueryMarkers/MainSubquery.hpp"
#include "store/parser/shadowNodes/planObjects/planMarkers/subqueryMarkers/TableSubquery.hpp"
#include "ExplainOp.hpp"
#include "PlanConstants.hpp"
#include "GraphNav.hpp"
#include "LimitOp.hpp"
#include "Location.hpp"
#include "PlanOperand.hpp"
#include "TableScanOp.hpp"
#include "QueryNode.hpp"
#include "Routing.hpp"
#include "SubqueryRefOp.hpp"
#include "Hashcast.hpp"

#include "TableRef.hpp"
#include "AliasRef.hpp"

#include "NestedLoopJoinOp.hpp"
#include "AggregateOp.hpp"

#include "ExprList.hpp"

#include "Expr.hpp"
#include "ExtractExpr.hpp"
#include "ComparisonExpr.hpp"
#include "EqExpr.hpp"
#include "ExistsExpr.hpp"
#include "GeExpr.hpp"
#include "GtExpr.hpp"
#include "LeExpr.hpp"
#include "LtExpr.hpp"
#include "NeExpr.hpp"
#include "BetweenExpr.hpp"
#include "LikeExpr.hpp"
#include "InExpr.hpp"
#include "SubstringExpr.hpp"

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

#include "AvgExpr.hpp"
#include "CountExpr.hpp"
#include "MinExpr.hpp"
#include "MaxExpr.hpp"
#include "MiscExpr.hpp"
#include "CastExpr.hpp"
#include "SubqueryRefExpr.hpp"
#include "SumExpr.hpp"



#include "ColumnOrdinal.hpp"
#include "ColumnRef.hpp"

#include "FilterOp.hpp"
#include "Hint.hpp"

#include "BooleanLiteral.hpp"
#include "DateLiteral.hpp"
#include "DecimalLiteral.hpp"
#include "FloatLiteral.hpp"
#include "IntegerLiteral.hpp"
#include "NullLiteral.hpp"
#include "StringLiteral.hpp"
#include "TimeLiteral.hpp"
#include "TimestampLiteral.hpp"

#include "IndexLookupJoinOp.hpp"
#include "Literal.hpp"
#include "OrderOp.hpp"
#include "OrderItem.hpp"
#include "ProjectedColumn.hpp"
#include "ProjectOp.hpp"
#include "UnionOp.hpp"

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
  m_pWalkNode = nullptr;

  m_sPipeline        = nullptr;
  m_bHasUnionOp      = false;
  m_bNewStage        = false;
  m_bStartStage      = false;
  m_bCombinedOp      = false;
  m_pPrevStageMarker = nullptr;
  m_bJoinPredicate   = false;
  m_bIndexJoin       = false;
  m_sIndexJoinTable  = ""_Stz;
  m_bInSubquery      = false;
  m_sIndent          = ""_Stz;

  m_iColumnsOfLeftChild = C_UNKNOWN;
  m_pColExprList        = nullptr;

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
  //vPrintPlanGraph(m_pPlanRoot);
//#endif

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // get the plan node of the plan
  QueryNode* pPlanNode = ShadowRoot::pPlanRoot(m_pPlanRoot);
  ASSERT(pPlanNode);

  QueryNode* pStartingNode = pPlanNode;

  // Handle an EXPLAIN of a plan. In this case pPlanNode will only have 1 child
  // which is a MainSubquery. The MainSubquery will have an ExplainMarker which
  // will have an ExplainOp. This may potentially be followed by another MainSubquery
  // and the pattern restarts again.
  while (pStartingNode->nChildren() == 1 &&
          MainSubquery::bIsOfType(pStartingNode->pFirstChild()) &&
         ExplainMarker::bIsOfType(pStartingNode->pFirstChild()->pFirstChild()) &&
             ExplainOp::bIsOfType(pStartingNode->pFirstChild()->pFirstChild()->pFirstChild()) ) {

    pStartingNode = _pQueryNode(pStartingNode->pFirstChild()->pFirstChild()->pFirstChild());
  }

  // At this point, pStartingNode is pointing to a node who will have children that
  // represent the subqueries in the statement. There must be one child which is
  // of type MainSubquery.
  PlanNodeIter* pIter = pStartingNode->pNewChildrenIter();

  // append the main pipeline
  vAppendPipeline("unamed"_Stz);

  while (pIter->bHasNext()) {
    QueryNode* pSubqueryMarker = _pQueryNode(pIter->pNext());
    ASSERT(SubqueryMarker::bIsOfType(pSubqueryMarker));

    if (TableSubquery::bIsOfType(pSubqueryMarker) ||
        ExprSubquery::bIsOfType(pSubqueryMarker) ) {

      m_bInSubquery = true;
      m_sIndent = "  "_Stz;

      // The MainSubquery node should have one child which is a StageMarker node
      ASSERT(pSubqueryMarker->nChildren() == 1);
      ASSERT(StageMarker::bIsOfType(pSubqueryMarker->pFirstChild()));
      QueryNode* pStageMarker = _pQueryNode(pSubqueryMarker->pFirstChild());

      m_sPipeline->vCatSprint(
          "%spipeline %s\n"
          ""_Stz,
          m_sIndent.azChar(),
          SubqueryMarker::szTableAlias(pSubqueryMarker));

      // append the nested pipeline for the table subquery
      //vAppendPipeline(SubqueryMarker::szTableAlias(pSubqueryMarker));

      // Get the list of correlated columns
      QueryNode* pCorrColumnsNode = SubqueryMarker::pCorrelatedColumnsNode(pSubqueryMarker);
      ASSERT(pCorrColumnsNode);
      ASSERT(ExprList::bIsOfType(pCorrColumnsNode));

      QueryNode* pExprList = _pQueryNode(pCorrColumnsNode);

      if (pExprList->bHasChildren()) {
        //PlanNodeIter* pIter = pExprList->pNewChildrenIter();

        PlanNodeIter* pExprIter = pExprList->pNewChildrenIter();

        m_sPipeline->vCatSprint(
            "  %sparams \""
            ""_Stz,
            m_sIndent.azChar());

        bool bFirstExpr = true;
        while (pExprIter->bHasNext()) {

          if (bFirstExpr) {
            bFirstExpr = false;
          } else {
            m_sPipeline->vCatSprint(" , "_Stz);
          }

          // get the next projectedColumn from the iterator
          QueryNode *pParam = static_cast<QueryNode *>(pExprIter->pNext());
          ASSERT(pParam);
          ASSERT(ColumnRef::bIsOfType(pParam));

          sPipeline->vCatSprint("%s"_Stz, ColumnRef::szColumn(pParam));

        }

        sPipeline->vCatSprint(" \"\n"_Stz);
        delete pExprIter;
      }

      m_sPipeline->vCatSprint(
          "%sbegin\n"
          ""_Stz,
          m_sIndent.azChar());

      // append the start stage
      vAppendStartStage();

      // We want to iterate over each logical subgraph, bounded by StageMarkers,
      // and generate the pipeline for each statement
      GraphNav::bGlobalWalk(pStageMarker, false, true, vVisitorPlan, (void *) this);

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      vAppendLastStage("on origin"_Stz);

      m_bInSubquery = false;
      m_sIndent = ""_Stz;

    } else if (MainSubquery::bIsOfType(pSubqueryMarker)) {
      QueryNode* pMainSubqueryMarker = _pQueryNode(pSubqueryMarker);

      // The MainSubquery node should have one child which is a StageMarker node
      ASSERT(pMainSubqueryMarker->nChildren() == 1);
      ASSERT(StageMarker::bIsOfType(pMainSubqueryMarker->pFirstChild()));
      QueryNode* pStageMarker = _pQueryNode(pMainSubqueryMarker->pFirstChild());

      // append the start stage
      vAppendStartStage();

      // We want to iterate over each logical subgraph, bounded by StageMarkers,
      // and generate the pipeline for each statement
      GraphNav::bGlobalWalk(pStageMarker, false, true, vVisitorPlan, (void *) this);

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      vAppendLastStage("on origin"_Stz);

    } else {
      NEVER_REACH
    }
  }

  delete pIter;
  return true;
}

/*----------------------------------------------------------------------------*/
/* print the plan graph                                                       */
/*----------------------------------------------------------------------------*/
void Backend::vPrintPlanGraph(PlanNode* pPlanRoot) {

  // Lambda visitor function to print the graph

  auto fVisitor = [](PlanNode *pNode, s2K &sPath, i8 iDepth, void *pvArg) -> GraphNav::eWalkCtrl {

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
void Backend::vAppendExpr(Backend* pBackend, QueryNode* pExprNode,
                          QueryNode* pOrdinalExpr, Stz* szAliasName) {
  _static_

  String* sPipeline = pBackend->m_sPipeline;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // type
  eCT eType = Expr::eType(pExprNode, pBackend->m_pPlanRoot);
  sPipeline->vCatSprint(
      " type %s"
      ""_Stz,
      aCT[eType]);

  if ( ! szAliasName) {
    if (ColumnRef::bIsOfType(pExprNode)) {
      QueryNode *pColumnRef = static_cast<QueryNode *>(pExprNode);
      szAliasName = ColumnRef::szColumn(pColumnRef);
      StrLib::vToLower(szAliasName->azChar());
    }
  }

  if (szAliasName) {
    sPipeline->vCatSprint(
        " as %s"
        ""_Stz,
        szAliasName);
  }

  // Walk the expr node plan graph
  GraphNav::bGlobalWalk(pOrdinalExpr, false, true,vVisitorExpr, vVisitorExpr, (void *) pBackend);

  //delete pOrdinalExpr;
}

/*----------------------------------------------------------------------------*/
/* append aggregate Op to the pipeline                                        */
/*----------------------------------------------------------------------------*/
void Backend::vAppendExprRelativeToGroupBy(Backend* pBackend, QueryNode* pExprNode, QueryNode* pOrdinalExprNode, Stz* szAliasName)
{
  _static_

  String* sPipeline = pBackend->m_sPipeline;

  eCT eType = Expr::eType(pExprNode, pBackend->m_pPlanRoot);

  sPipeline->vCatSprint(
    " type %s"
    ""_Stz,
    aCT[eType]);

  sPipeline->vCatSprint(
    " $K%lld"
    ""_Stz,
    ColumnOrdinal::iOrdinal(pOrdinalExprNode));

  if (szAliasName) {
    sPipeline->vCatSprint(
        " as %s"
        ""_Stz,
        szAliasName);
  }
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

  // Get the list of correlated expressions from the enclosing subquery.
  // We don't need to free this object.
  QueryNode* pCorrelatedExprs = PlanObject::pCorrelatedExprs(pNode, pBackend->m_pPlanRoot);

  // get the ordinal projected columns relative to group by
  QueryNode* pOrdinalProjectedColumns =
      AggregateOp::pOrdinalProjectedColumnsRelativeToGroupBy(pNode, pBackend->m_pPlanRoot);

  // Iterate each ordinal projected expression
  if (pProjectedColumns->bHasChildren()) {

    pBackend->vAppendNextOp("aggreg 2 \""_Stz);
    pBackend->m_bCombinedOp = true;

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

      RASSERT(pBackend->m_pColExprList);
      RASSERT(pCorrelatedExprs);

      QueryNode* pOrdinalExpr = Expr::pSymbolicToOrdinal(pGroupByExpr, pBackend->m_pColExprList, pCorrelatedExprs, pBackend->m_pPlanRoot);
      RASSERT(pOrdinalExpr);

      //vAppendExpr(pBackend, pColExprList, pCorrelatedExprs, pGroupByExpr, nullptr);
      vAppendExpr(pBackend, pGroupByExpr, pOrdinalExpr, nullptr);
      delete pOrdinalExpr;
    }

    // Get the aggregate expressions
    sPipeline->vCatSprint(" aggregates "_Stz);
    bFirstExpr = false;

    // get a plan node iterator
    PlanNodeIter *pIter = pProjectedColumns->pNewChildrenIter();
    i8 iChild = -1;
    while (pIter->bHasNext()) {

      iChild++;

      // append comma
      if (bFirstExpr)
        sPipeline->vCatSprint(" ,"_Stz);
      else
        bFirstExpr = true;

      // get the next projectedColumn from the iterator
      QueryNode *pProjectedColumn = static_cast<QueryNode *>(pIter->pNext());
      ASSERT(pProjectedColumn);

      ASSERT(ProjectedColumn::bIsOfType(pProjectedColumn));
      Stz* szAliasName = ProjectedColumn::szAliasName(pProjectedColumn);
      QueryNode* pExprNode  = ProjectedColumn::pExprNode(pProjectedColumn);

      // get the ordinal projected column relative to group by
      QueryNode *pRelativeOrdinal = static_cast<QueryNode *>(pOrdinalProjectedColumns->pNthChild(iChild));
      QueryNode *pOrdinalExprNode = static_cast<QueryNode *>(GraphNav::pFindProperty("expr"_Stz, pRelativeOrdinal));
      ASSERT(pOrdinalExprNode);

      // generate $K? relative to group by key if its a column ordinal
//      if ( ! ColumnRef::bIsOfType(pExprNode) &&
//           ColumnOrdinal::bIsOfType(pOrdinalExprNode)) {
//        vAppendExprRelativeToGroupBy(pBackend, pExprNode, pOrdinalExprNode, szAliasName);
//      } else {
      if ( ColumnOrdinal::bIsOfType(pOrdinalExprNode) ) {
        vAppendExprRelativeToGroupBy(pBackend, pExprNode, pOrdinalExprNode, szAliasName);
      } else {
        RASSERT(pBackend->m_pColExprList);
        RASSERT(pCorrelatedExprs);

        QueryNode *pOrdinalExpr = Expr::pSymbolicToOrdinal(pExprNode, pBackend->m_pColExprList, pCorrelatedExprs,
                                                           pBackend->m_pPlanRoot);
        RASSERT(pOrdinalExpr);

        //vAppendExpr(pBackend, pColExprList, pCorrelatedExprs, pExprNode, szAliasName);
        vAppendExpr(pBackend, pExprNode, pOrdinalExpr, szAliasName);
        delete pOrdinalExpr;
      }
    }

    // cleanup
    delete pOrdinalProjectedColumns;
    delete pIter;

    delete pGroupByIter;
  }

  sPipeline->vCatSprint(" \""_Stz);

  // cleanup
  pBackend->m_pColExprList->vDeleteAll();
  delete pBackend->m_pColExprList;
}

/*----------------------------------------------------------------------------*/
/* append call to subquery to the pipeline                                    */
/*----------------------------------------------------------------------------*/
void Backend::vAppendCallSubQry(Backend* pBackend, QueryNode* pNode)
{
  _static_

  s4K sCallSubqry;
  //String* sPipeline = pBackend->m_sPipeline;

  sCallSubqry.vSprint("call 1 \"%s \""_Stz, SubqueryRefOp::szAliasName(pNode));
  pBackend->vAppendNextOp(sCallSubqry);

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

  pBackend->vAppendNextOp("filter 2 \" filter type BOOLEAN "_Stz);
  pBackend->m_bCombinedOp = true;
//  sPipeline->vCatSprint(
//      "    filter 1 \" type BOOLEAN "
//      ""_Stz);

  // Walk the expr node plan graph
  //GraphNav::bGlobalWalkBottomUp(pOrdinalFilterExpr, false, true, vVisitorExpr, (void *) pBackend);
  GraphNav::bGlobalWalk(pOrdinalFilterExpr, false, true,vVisitorExpr, vVisitorExpr, (void *) pBackend);
  delete pOrdinalFilterExpr;

  sPipeline->vCatSprint(" \""_Stz);

}

/*----------------------------------------------------------------------------*/
/* append indexjoinOp to the pipeline                                         */
/*----------------------------------------------------------------------------*/
void Backend::vAppendIndexJoinOp(Backend* pBackend, QueryNode* pNode)
{
  _static_

  s4K sIndexJoin;
  String* sPipeline = pBackend->m_sPipeline;

  pBackend->m_pColExprList = JoinOp::pColumnExprs(pNode, pBackend->m_pPlanRoot);
  QueryNode* pOrdinalJoinPredicate = JoinOp::pOrdinalJoinPredicate(pNode, pBackend->m_pPlanRoot);

  // Get the joinType property
  //eJT eJoinType = JoinOp::eJoinType(pNode);

  // Get the joinPredicate property
  //QueryNode* pJoinPredicate = JoinOp::pJoinPredicate(pNode);
  //ASSERT(pJoinPredicate);

  // save the left child columns
  pBackend->m_bJoinPredicate = true;
  pBackend->m_iColumnsOfLeftChild = JoinOp::iColumnsOfChild1(pNode, pBackend->m_pPlanRoot);

  // NOTE: this will change , we should be able to get the joinIndex as a property
  // of the index
  // workaround: get the join index from the hints
  QueryNode* pHint = PlanObject::pGetHint(_pQueryNode(pNode), "JoinIndex"_Stz);
  if ( ! pHint) {
    vTrace("Warning: No JoinIndex hint found\n"_Stz);
    RASSERT(0);
  }
  ASSERT(Hint::bIsOfType(pHint));
  QueryNode* pHintValue = _pQueryNode(pHint->pFirstChild());
  Stz* sJoinIndex = pHintValue->szValue();

  sIndexJoin.vSprint("indexJoin 2 %s %s \""_Stz, pBackend->m_sIndexJoinTable.azChar(), sJoinIndex);
  pBackend->vAppendNextOp(sIndexJoin);
  pBackend->m_bCombinedOp = true;

  GraphNav::bGlobalWalk(pOrdinalJoinPredicate, false, true,vVisitorExpr, vVisitorExpr, (void *) pBackend);
  sPipeline->vCatSprint(" \""_Stz);

  // reset left child columns
  pBackend->m_bJoinPredicate = false;
  pBackend->m_iColumnsOfLeftChild = C_UNKNOWN;

  // cleanup

  pBackend->m_bIndexJoin = false;
  pBackend->m_sIndexJoinTable = ""_Stz;

  pBackend->m_pColExprList->vDeleteAll();
  delete pBackend->m_pColExprList;

  delete pOrdinalJoinPredicate;
}

/*----------------------------------------------------------------------------*/
/* append limit Op to the pipeline                                            */
/*----------------------------------------------------------------------------*/
void Backend::vAppendLimitOp(Backend* pBackend, QueryNode* pNode)
{
  _static_

  //String* sPipeline = pBackend->m_sPipeline;

  s256 sLimit;

//  sPipeline->vCatSprint(
//      "    limit 1 %lld"
//      ""_Stz,
//      LimitOp::iLimit(pNode));

  i8 iOffset = LimitOp::iOffset(pNode);

  if (iOffset != C_UNKNOWN) {
    //sPipeline->vCatSprint("offset %lld"_Stz, iOffset);
    sLimit.vCatSprint("limit 2 %lld offset %lld"_Stz, LimitOp::iLimit(pNode), iOffset);
  }
  else {
    sLimit.vCatSprint("limit 2 %lld"_Stz, LimitOp::iLimit(pNode));
  }
  pBackend->m_bCombinedOp = true;

  pBackend->vAppendNextOp(sLimit);
}

/*----------------------------------------------------------------------------*/
/* append loopjoinOp to the pipeline                                          */
/*----------------------------------------------------------------------------*/
void Backend::vAppendLoopJoinOp(Backend* pBackend, QueryNode* pNode)
{
  _static_

  String* sPipeline = pBackend->m_sPipeline;

  pBackend->m_pColExprList = JoinOp::pColumnExprs(pNode, pBackend->m_pPlanRoot);
  QueryNode* pOrdinalJoinPredicate = JoinOp::pOrdinalJoinPredicate(pNode, pBackend->m_pPlanRoot);

  // Get the joinType property
  //eJT eJoinType = JoinOp::eJoinType(pNode);

  // Get the joinPredicate property
  //QueryNode* pJoinPredicate = JoinOp::pJoinPredicate(pNode);
  //ASSERT(pJoinPredicate);

  // save the left child columns
  pBackend->m_bJoinPredicate = true;
  pBackend->m_iColumnsOfLeftChild = JoinOp::iColumnsOfChild1(pNode, pBackend->m_pPlanRoot);

  pBackend->vAppendNextOp("loopJoin 0 \""_Stz);
  GraphNav::bGlobalWalk(pOrdinalJoinPredicate, false, true,vVisitorExpr, vVisitorExpr, (void *) pBackend);
  sPipeline->vCatSprint(" \""_Stz);

  // reset left child columns
  pBackend->m_bJoinPredicate = false;
  pBackend->m_iColumnsOfLeftChild = C_UNKNOWN;

  // cleanup

  pBackend->m_pColExprList->vDeleteAll();
  delete pBackend->m_pColExprList;

  delete pOrdinalJoinPredicate;
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
  pBackend->m_pColExprList = PlanObject::pColumnExprs(_pQueryNode(pNode->pFirstChild()), pBackend->m_pPlanRoot);

  // Get the list of correlated expressions from the enclosing subquery.
  // We don't need to free this object.
  QueryNode* pCorrelatedExprs = PlanObject::pCorrelatedExprs(pNode, pBackend->m_pPlanRoot);

  // Iterate each projected expression
  if (pOrderItemList->bHasChildren()) {

    pBackend->vAppendNextOp("sort 2 \""_Stz);
//    sPipeline->vCatSprint(
//        "    orderBy 0 \""
//        ""_Stz);
    pBackend->m_bCombinedOp = true;

    // get a plan node iterator
    PlanNodeIter *pIter = pOrderItemList->pNewChildrenIter();

    while (pIter->bHasNext()) {

      // append comma
      if (bFirstExpr)
        sPipeline->vCatSprint(" ,"_Stz);
      else
        bFirstExpr = true;

      // get the next orderItem from the iterator
      QueryNode *pOrderItem = static_cast<QueryNode *>(pIter->pNext());
      ASSERT(pOrderItem);
      ASSERT(OrderItem::bIsOfType(pOrderItem));

      RASSERT(pBackend->m_pColExprList);
      RASSERT(pCorrelatedExprs);

      QueryNode* pExprNode = OrderItem::pExprNode(pOrderItem);

      QueryNode* pOrdinalExpr = Expr::pSymbolicToOrdinal(pExprNode, pBackend->m_pColExprList, pCorrelatedExprs, pBackend->m_pPlanRoot);
      RASSERT(pOrdinalExpr);

      //vAppendExpr(pBackend, pColExprList, pCorrelatedExprs, pOrderItem, ""_Stz);
      vAppendExpr(pBackend, pExprNode, pOrdinalExpr, nullptr);
      delete pOrdinalExpr;

      sPipeline->vCatSprint(" %s "_Stz, OrderItem::bIsAscending(pOrderItem) ? "ASC" : "DESC");
    }
    delete pIter;
  }

  sPipeline->vCatSprint(" \""_Stz);

  // cleanup
  pBackend->m_pColExprList->vDeleteAll();
  delete pBackend->m_pColExprList;
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
  pBackend->m_pColExprList = PlanObject::pColumnExprs(_pQueryNode(pNode->pFirstChild()), pBackend->m_pPlanRoot);

  // Get the list of correlated expressions from the enclosing subquery.
  // We don't need to free this object.
  QueryNode* pCorrelatedExprs = PlanObject::pCorrelatedExprs(pNode, pBackend->m_pPlanRoot);

  // Iterate each projected expression
  if (pProjectedColumns->bHasChildren()) {

    pBackend->vAppendNextOp("project 2 \""_Stz);
    pBackend->m_bCombinedOp = true;

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

      Stz* szAliasName = ProjectedColumn::szAliasName(pProjectedColumn);
      QueryNode* pExprNode  = ProjectedColumn::pExprNode(pProjectedColumn);

      RASSERT(pBackend->m_pColExprList);
      RASSERT(pCorrelatedExprs);

      QueryNode* pOrdinalExpr = Expr::pSymbolicToOrdinal(pExprNode, pBackend->m_pColExprList, pCorrelatedExprs, pBackend->m_pPlanRoot);
      RASSERT(pOrdinalExpr);

      //vAppendExpr(pBackend, pColExprList, pCorrelatedExprs, pExprNode, szAliasName);
      vAppendExpr(pBackend, pExprNode, pOrdinalExpr, szAliasName);
      delete pOrdinalExpr;
    }
    delete pIter;

  }

  sPipeline->vCatSprint(" \""_Stz);

  // cleanup
  pBackend->m_pColExprList->vDeleteAll();
  delete pBackend->m_pColExprList;
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

  s256 sScan;

  sScan.vCatSprint("scan %s %s %s"_Stz, szCatalogName, szSchemaName, szTableName);
  pBackend->vAppendNextOp(sScan);

}

/*----------------------------------------------------------------------------*/
/* append scan Op to the pipeline                                             */
/*----------------------------------------------------------------------------*/
void Backend::vAppendScanRightOp(Backend* pBackend, QueryNode* pNode)
{
  _static_

  String* sPipeline = pBackend->m_sPipeline;

  QueryNode *pTableRef = TableScanOp::pTableRef(pNode);
  Stz *szCatalogName   = TableRef::szCatalog(pTableRef);
  Stz *szSchemaName    = TableRef::szSchema(pTableRef);
  Stz *szTableName     = TableRef::szTable(pTableRef);

  s256 sScan;

  sScan.vCatSprint("scanRight %s %s %s"_Stz, szCatalogName, szSchemaName, szTableName);

  pBackend->vAppendNextOp(sScan);
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

  // check for a stagemarker
  PlanNode* pLeafStageMarker = nullptr;

  auto fLeafStageMarker = [&pLeafStageMarker](PlanNode* pNode, s2K& sPath, i8 iDepth, void* pvArg) -> GraphNav::eWalkCtrl  {
    if (StageMarker::bIsExactType(pNode)) {
      pLeafStageMarker = pNode;
      return GraphNav::WALK_ABORT;
    }
    return GraphNav::WALK_CONTINUE;
  };

  String* sPipeline = pBackend->m_sPipeline;

  // add child op's

  GraphNav::bGlobalWalkBottomUp(pChildStageMarker1, false, true, fLeafStageMarker, (void *) pBackend);
  RASSERT(pLeafStageMarker);

  // check for a unionOp
  auto fHasUnionOp = [](PlanNode* pNode, void* pvArg) -> b1 {
    return UnionOp::bIsExactType(pNode);
  };

  // check for a unionOp
  PlanNode *pHasUnionOp = GraphNav::pBoundedSearch(pChildStageMarker1, false, true, fHasUnionOp, nullptr);

  if ( ! pHasUnionOp ) {
    Stz* szRouting = szGetRouting(pBackend, pChildStageMarker1);
    pBackend->vAppendNextStage(pLeafStageMarker->szName(), szRouting);
  }

  // add child op's
  GraphNav::bGlobalWalkBottomUp(pChildStageMarker2, false, true, fLeafStageMarker, (void *) pBackend);
  RASSERT(pLeafStageMarker);

  Stz *szRouting = szGetRouting(pBackend, pChildStageMarker2);
  pBackend->vAppendNextStage(pLeafStageMarker->szName(), szRouting);

  pBackend->m_bNewStage = false;
  pBackend->m_bHasUnionOp = true;

  // bounded walk of first children
  GraphNav::bGlobalWalk(pChildStageMarker1, false, true, vVisitorPlan, (void *) pBackend);

  pBackend->m_bHasUnionOp = true;

  // bounded walk of first children
  GraphNav::bGlobalWalk(pChildStageMarker2, false, true, vVisitorPlan, (void *) pBackend);
}

/*----------------------------------------------------------------------------*/
/* visitor function for the plan graph walk                                   */
/*----------------------------------------------------------------------------*/
GraphNav::eWalkCtrl Backend::vVisitorExpr(PlanNode* pExprNode, s2K& sPath, i8 iDepth, GraphNav::eWalkEvent eEventType, void* pvArg)
{
  _static_
      UNUSED(sPath);
  UNUSED(iDepth);

  static bool bHasAgg = false;

  QueryNode* pOrdinalExpr = _pQueryNode(pExprNode);

  Backend* pBackend = (Backend*)pvArg;
  String* sPipeline = pBackend->m_sPipeline;

//  vTrace("Got %s event for node path: %s\n"_Stz,
//           eEventType == Parser::GraphNav::EVENT_ENTER ? "ENTER" : "EXIT",
//           sPath.szStz());

  if (eEventType == Parser::GraphNav::EVENT_ENTER) {

    if (AggregateExpr::bIsOfType(pOrdinalExpr)) {
      sPipeline->vCatSprint(" ) "_Stz);
    }

    if (BetweenExpr::bIsOfType(pOrdinalExpr)) {
      sPipeline->vCatSprint(" } "_Stz);

      QueryNode* pPredicand  = BetweenExpr::pPredicand(_pQueryNode(pOrdinalExpr));
      QueryNode* pLowerBound = BetweenExpr::pLowerBound(_pQueryNode(pOrdinalExpr));
      QueryNode* pUpperBound = BetweenExpr::pUpperBound(_pQueryNode(pOrdinalExpr));

      // Walk the expr node plan graph
      GraphNav::bGlobalWalk(pLowerBound, false, true,vVisitorExpr, vVisitorExpr, (void *) pBackend);

      // Walk the expr node plan graph
      GraphNav::bGlobalWalk(pUpperBound, false, true,vVisitorExpr, vVisitorExpr, (void *) pBackend);

      // Walk the expr node plan graph
      GraphNav::bGlobalWalk(pPredicand, false, true,vVisitorExpr, vVisitorExpr, (void *) pBackend);

      sPipeline->vCatSprint(" between "_Stz);

      return GraphNav::WALK_SKIP_CHILDREN;
    }

    if (LikeExpr::bIsOfType(pOrdinalExpr)) {
      sPipeline->vCatSprint(" } "_Stz);
    }

    if (InExpr::bIsOfType(pOrdinalExpr)) {
      sPipeline->vCatSprint(" } "_Stz);

      QueryNode* pLeftExpr = InExpr::pLeftExpr(pOrdinalExpr);

      PlanNodeIter* pRightIter = InExpr::pRightExprIter(pOrdinalExpr);
      i8 iInExprCount = 0;
      while (pRightIter->bHasNext()) {
        iInExprCount++;
        QueryNode* pChild = _pQueryNode(pRightIter->pNext());
        ASSERT(Expr::bIsOfType(pChild));

        // Walk the expr node plan graph
        GraphNav::bGlobalWalk(pChild, false, true,vVisitorExpr, vVisitorExpr, (void *) pBackend);
      }

      delete pRightIter;

      sPipeline->vCatSprint(" #%lld "_Stz, iInExprCount);
      GraphNav::bGlobalWalk(pLeftExpr, false, true,vVisitorExpr, vVisitorExpr, (void *) pBackend);
      sPipeline->vCatSprint(" in "_Stz);

      // TODO: CHECK THIS OUT
      return GraphNav::WALK_ABORT;
    }

    //delete pOrdinalExpr;

    return GraphNav::WALK_CONTINUE;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // columnRef

  if (ColumnOrdinal::bIsOfType(pOrdinalExpr)) {
    QueryNode* pColumnOrdinal = _pQueryNode(pOrdinalExpr);
    bool bJoinPredicate = pBackend->m_bJoinPredicate;
    bool bIndexJoin = pBackend->m_bIndexJoin;

    RASSERT(pBackend->m_iColumnsOfLeftChild);

    i8 iColumnOrdinal = ColumnOrdinal::iOrdinal(pColumnOrdinal);

    if (bJoinPredicate) {

      if (bIndexJoin) {
        if (iColumnOrdinal - pBackend->m_iColumnsOfLeftChild <= 0 )
          sPipeline->vCatSprint(
              " BOUND $L%lld"
              ""_Stz,
              iColumnOrdinal);
        else
          sPipeline->vCatSprint(
              " $I%lld"
              ""_Stz,
              iColumnOrdinal - pBackend->m_iColumnsOfLeftChild);
      }
      else {
        // loopJoin
        if (iColumnOrdinal - pBackend->m_iColumnsOfLeftChild <= 0 )
          sPipeline->vCatSprint(
              " $L%lld"
              ""_Stz,
              iColumnOrdinal);
        else
          sPipeline->vCatSprint(
              " $R%lld"
              ""_Stz,
              iColumnOrdinal - pBackend->m_iColumnsOfLeftChild);
      }
    }
    else {
      sPipeline->vCatSprint(
          " $%lld"
          ""_Stz,
          ColumnOrdinal::iOrdinal(pColumnOrdinal));
    }
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // aggregate
  if (AggregateExpr::bIsOfType(pOrdinalExpr)) {

    if (CountExpr::bIsOfType(pOrdinalExpr)) {

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

//    else {
//      NEVER_REACH;
//    }

    //if (bDeleteOrdExpr)
    //delete pOrdinalExpr;
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

    else if (ExistsExpr::bIsOfType(pOrdinalExpr)) {
      sPipeline->vCatSprint(" exists "_Stz);
    }

    else if (LikeExpr::bIsOfType(pOrdinalExpr)) {

      if (pOrdinalExpr->nChildren() == 2) {

        // get the pattern
        QueryNode* pPattern   = LikeExpr::pPattern(_pQueryNode(pOrdinalExpr));

        // check for a null pattern
        if ( pPattern->bIsNullValue() ) {
          sPipeline->vCatSprint(" like2 "_Stz);
          return GraphNav::WALK_CONTINUE;
        }

        // get the pattern
        char* szPattern = (char*)pPattern->szValue();

        // find first occurence of _ in pattern
        i8 iUnderScore = StrLib::iFind(szPattern, '_');
        if (iUnderScore != C_UNKNOWN) {
          sPipeline->vCatSprint(" like2 "_Stz);
          return GraphNav::WALK_CONTINUE;
        }

        // special case of LIKE

        // find length of the pattern string
        i8 iLength = StrLib::nLength(szPattern);

        // find no of occurences of %
        i8 iPercentages = StrLib::iOccurrence(szPattern, '%');

        switch (iPercentages) {
          case 1:
            // leading / trailing %
            if (szPattern[0] == '%')
              sPipeline->vCatSprint(" %lld matchTrailing "_Stz, iLength);
            else if (szPattern[iLength-1] == '%')
              sPipeline->vCatSprint(" %lld matchLeading "_Stz, iLength);
            else
              sPipeline->vCatSprint(" like2 "_Stz);
            break;

          case 2:
            // leading and trailing
            if (szPattern[0] == '%' && szPattern[iLength-1] == '%')
              sPipeline->vCatSprint(" %lld matchMiddle "_Stz, iLength-2);
            else
              sPipeline->vCatSprint(" like2 "_Stz);
            break;

          case 3:
            //leading and trailing and one in the middle
            if (szPattern[0] == '%' && szPattern[iLength-1] == '%') {
              i8 iLength1 = StrLib::iFind(szPattern+1, '%');
              i8 iLength2 = iLength - iLength1 - 3;
              sPipeline->vCatSprint(" %lld %lld matchMiddle1 "_Stz, iLength1, iLength2);
            }
            else
              sPipeline->vCatSprint(" like2 "_Stz);
            break;

//          case 4:
//            //leading and trailing and two in the middle
//            if (szPattern[0] == '%' && szPattern[iLength-1] == '%')
//              sPipeline->vCatSprint(" %lld matchMiddle2 "_Stz, iLength);
//            else
//              sPipeline->vCatSprint(" like2 "_Stz);
//            break;

          default:
            sPipeline->vCatSprint(" like2 "_Stz);
            break;
        }

      }
      else {
        sPipeline->vCatSprint(" like3 "_Stz);
      }

    }

    else if (InExpr::bIsOfType(pOrdinalExpr)) {
      sPipeline->vCatSprint(" in "_Stz);
    }
    else if (BetweenExpr::bIsOfType(pOrdinalExpr)) {
      // nothing to do
    }

    else {
      NEVER_REACH;
    }

    //if (bDeleteOrdExpr)
    //  delete pOrdinalExpr;
    return GraphNav::WALK_CONTINUE;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // extract

  if (ExtractExpr::bIsOfType(pOrdinalExpr)) {

    s32 sExtractType;

    ExtractField::eEF eExtractField = ExtractExpr::eExtractField(_pQueryNode(pOrdinalExpr));
    sExtractType =  ExtractField::szExtractFieldType(eExtractField);
    StrLib::vToLower(sExtractType.azChar());

    sPipeline->vCatSprint(" extract_%s "_Stz, sExtractType.azChar());

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

    if (DecimalLiteral::bIsOfType(pOrdinalExpr)) {
      Bcd xBcd;
      SP_Bcd spBcd;
      xBcd = DecimalLiteral::xValue(pOrdinalExpr);
      sPipeline->vCatSprint(" %s "_Stz, (char*)spBcd.sz(xBcd.T.m_iMantissa, xBcd.T.m_iScale));
    }

    if (FloatLiteral::bIsOfType(pOrdinalExpr)) {
      sPipeline->vCatSprint(" %e "_Stz, FloatLiteral::dValue(pOrdinalExpr));
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

    //if (bDeleteOrdExpr)
    //  delete pOrdinalExpr;
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

    //if (bDeleteOrdExpr)
    //  delete pOrdinalExpr;
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

    //if (bDeleteOrdExpr)
    //  delete pOrdinalExpr;
    return GraphNav::WALK_CONTINUE;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // misc operator

  if (MiscExpr::bIsOfType(pOrdinalExpr)) {

    if (CastExpr::bIsOfType(pOrdinalExpr)) {

      QueryNode* pSymbolicExpr = Expr::pOrdinalToSymbolic(pOrdinalExpr, pBackend->m_pColExprList,
                                                    pBackend->m_pPlanRoot);

      eCT eType = CastExpr::eType(pSymbolicExpr, pBackend->m_pPlanRoot);

      sPipeline->vCatSprint(
          " casttype %s CAST "
          ""_Stz,
          aCT[eType]);

      delete pSymbolicExpr;
    }
    else {
      NEVER_REACH;
    }

  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // substring

  if (SubstringExpr::bIsOfType(pOrdinalExpr)) {

    QueryNode* pLengthExprNode = SubstringExpr::pLengthExpr(_pQueryNode(pOrdinalExpr));

    if (pLengthExprNode)
      sPipeline->vCatSprint(" substring3 "_Stz);
    else
      sPipeline->vCatSprint(" substring2 "_Stz);

    return GraphNav::WALK_CONTINUE;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // subquery reference

  if (SubqueryRefExpr::bIsOfType(pOrdinalExpr)) {
    sPipeline->vCatSprint(" %s "_Stz, SubqueryRefExpr::szTableAlias(pOrdinalExpr));
    return GraphNav::WALK_CONTINUE;
  }

  // Keep walking
  //if (bDeleteOrdExpr)
  //  delete pOrdinalExpr;
  return GraphNav::WALK_CONTINUE;
}

/*----------------------------------------------------------------------------*/
/* visitor function for the plan graph walk                                   */
/*----------------------------------------------------------------------------*/
GraphNav::eWalkCtrl Backend::vVisitorStatement(PlanNode* pRootNode, s2K& sPath, i8 iDepth, void* pvArg)
{
  _static_

  QueryNode* pQueryNode = static_cast<QueryNode *>(pRootNode);

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
      vAppendProjectOp(pBackend, pQueryNode);
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // TableScanOp

    else if (TableScanOp::bIsOfType(pQueryNode)) {
      QueryNode *pParent = _pQueryNode(GraphNav::pGetParentOfPath(sPath, pBackend->m_pWalkNode));

      if (NestedLoopJoinOp::bIsOfType(pParent))
        vAppendScanRightOp(pBackend, pQueryNode);

      else if (IndexLookupJoinOp::bIsOfType(pParent)) {
        pBackend->m_bIndexJoin = true;
        QueryNode *pTableRef = TableScanOp::pTableRef(pQueryNode);
        pBackend->m_sIndexJoinTable = TableRef::szTable(pTableRef);
      }

      else {
        vAppendScanOp(pBackend, pQueryNode);
      }

    }

    else if (NestedLoopJoinOp::bIsOfType(pQueryNode)) {
      vAppendLoopJoinOp(pBackend, pQueryNode);
    }

    else if (IndexLookupJoinOp::bIsOfType(pQueryNode)) {
      vAppendIndexJoinOp(pBackend, pQueryNode);
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // UnionOp

    else if (UnionOp::bIsOfType(pQueryNode)) {
      RASSERT(0);
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // OrderByOp
    else if (OrderOp::bIsOfType(pQueryNode)) {
      vAppendOrderByOp(pBackend, pQueryNode);
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // SubqueryRefOp
    else if (SubqueryRefOp::bIsOfType(pQueryNode)) {
      vAppendCallSubQry(pBackend, pQueryNode);
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // other Op's
    else  {
      NOT_IMPLEMENTED_YET
    }
  }

  // Keep walking
  return GraphNav::WALK_CONTINUE;
}

/*----------------------------------------------------------------------------*/
/* visitor function for the plan graph walk                                   */
/*----------------------------------------------------------------------------*/
GraphNav::eWalkCtrl Backend::vVisitorPlan(PlanNode* pRootNode, s2K& sPath, i8 iDepth, void* pvArg)
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

    pBackend->m_bHasUnionOp = true;
    Stz *szRouting = szGetRouting(pBackend, pStageMarker);
    pBackend->vAppendNextStage(pStageMarker->szName(), szRouting);

    // get the location of this stageMarker
    QueryNode* pLocation = StageMarker::pLocation(pStageMarker);
    RASSERT(pLocation);
    Stz *szLocation = Parser::Location::szLocationType(pLocation);

    pBackend->vAppendNewStage(pStageMarker->szName(), szLocation);

    QueryNode *pUnionOp = static_cast<QueryNode *>(pHasUnionOp);
    pBackend->vAppendNextOp(pUnionOp->szName());

    // previous stage to pipe to union stage
    pBackend->vAppendNewStage(pUnionOp->szName(), szLocation);
    vAppendUnionOp(pBackend, pUnionOp);
    return GraphNav::WALK_SKIP_CHILDREN;
  }

  // get the location of this stageMarker
  QueryNode* pLocation = StageMarker::pLocation(pStageMarker);
  RASSERT(pLocation);
  Stz *szLocation = Parser::Location::szLocationType(pLocation);

  // Walk the current node and its children,
  // calling the visitor function for each node encountered. This is a
  // bottom-up, depth-first traversal.
  // Starting on a StageMarker will walk until the subsequent StageMarker
  // node is encountered.
  if ( ! pHasUnionOp ) {
    pBackend->m_pPrevStageMarker = pStageMarker;
    GraphNav::bGlobalWalkBottomUp(pStageMarker, false, true, vVisitorStage, (void *) pBackend);

    //if ( !pBackend->bInTableSubquery(pStageMarker) )
    pBackend->vAppendNextStage("stageLast"_Stz);

    return GraphNav::WALK_ABORT;
  }

  // Keep walking
  return GraphNav::WALK_CONTINUE;
}

/*----------------------------------------------------------------------------*/
/* visitor function for the stage                                             */
/*----------------------------------------------------------------------------*/
GraphNav::eWalkCtrl Backend::vVisitorStage(PlanNode* pRootNode, s2K& sPath, i8 iDepth, void* pvArg)
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

  // get the location of this stageMarker
  QueryNode* pLocation = StageMarker::pLocation(pStageMarker);
  RASSERT(pLocation);
  Stz *szLocation = Parser::Location::szLocationType(pLocation);

  if (pBackend->m_bHasUnionOp) {
    pBackend->m_bHasUnionOp = false;
  }
  else {
    Stz *szRouting = szGetRouting(pBackend, pBackend->m_pPrevStageMarker);
    pBackend->vAppendNextStage(pStageMarker->szName(), szRouting);
  }

  pBackend->vAppendNewStage(pStageMarker->szName(), szLocation);

  // Walk the current node and its children,
  // calling the visitor function for each node encountered. This is a
  // bottom-up, depth-first traversal.
  // Starting on a StageMarker will walk until the subsequent StageMarker
  // node is encountered.

  pBackend->m_pPrevStageMarker = pStageMarker;
  pBackend->m_pWalkNode = pStageMarker;
  GraphNav::bBoundedWalkBottomUp(pStageMarker, false, true, vVisitorStatement, (void *) pBackend);

  // Keep walking
  return GraphNav::WALK_CONTINUE;
}

/*----------------------------------------------------------------------------*/
/* append last stage to the pipeline                                          */
/*----------------------------------------------------------------------------*/
void Backend::vAppendLastStage(Stz* szLocation) {

  m_sPipeline->vCatSprint(
      "\n  %sstage stageLast %s :\n"
      "    %soutput\n"
      ""_Stz, m_sIndent.azChar(), szLocation, m_sIndent.azChar());

  m_sPipeline->vCatSprint(
      "    %s| sink\n"
      "    %s| stageStart\n"
      ""_Stz, m_sIndent.azChar(), m_sIndent.azChar());

  if (m_bInSubquery)
    m_sPipeline->vCatSprint(
        "%send\n\n"
        ""_Stz, m_sIndent.azChar());
  else
    m_sPipeline->vCatSprint(
        "%send ; \n"
        ""_Stz, m_sIndent.azChar());

 // vTrace("%s\n"_Stz, m_sPipeline->azStr());
}

/*----------------------------------------------------------------------------*/
/* append new stage to the pipeline                                           */
/*----------------------------------------------------------------------------*/
void Backend::vAppendNewStage(Stz* szStage, Stz* szLocation) {

  m_bNewStage = true;

  m_sPipeline->vCatSprint(
      "\n  %sstage %s on %s :\n"
      ""_Stz,
      m_sIndent.azChar(),
      szStage,
      szLocation);
}

/*----------------------------------------------------------------------------*/
/* append extra pipe pipeline                                                 */
/*----------------------------------------------------------------------------*/
void Backend::vAppendNextOp(Stz* szNextStage) {

  if (m_bNewStage) {
    m_bNewStage = false;
    m_sPipeline->vCatSprint(
        "    %s%s"
        ""_Stz,
        m_sIndent.azChar(),
        szNextStage);
  }
  else {
    m_sPipeline->vCatSprint(
        "\n    %s| %s"
        ""_Stz,
        m_sIndent.azChar(),
        szNextStage);
  }
}

/*----------------------------------------------------------------------------*/
/* append extra pipe pipeline                                                 */
/*----------------------------------------------------------------------------*/
void Backend::vAppendNextStage(Stz* szNextStage, Stz* szRouting) {

  if (szRouting) {
    if (m_bCombinedOp) {
      m_sPipeline->vCatSprint(
          "\n    %s| combined 0"
          ""_Stz,
          m_sIndent.azChar());
      m_bCombinedOp = false;
    }
    m_sPipeline->vCatSprint(
        "\n    %s| %s %s"
        ""_Stz,
        m_sIndent.azChar(),
        szNextStage,
        szRouting);
  }
  else {
    if ( m_bStartStage )
      m_bStartStage = false;
    else {
      if (m_bCombinedOp) {
        m_sPipeline->vCatSprint(
            "\n    %s| combined 0"
            ""_Stz,
            m_sIndent.azChar());
        m_bCombinedOp = false;
      }
    }

  if (m_bNewStage) {
    m_bNewStage = false;
    m_sPipeline->vCatSprint(
        "\n    %s%s"
        ""_Stz,
        m_sIndent.azChar(),
        szNextStage);
  }
  else
    m_sPipeline->vCatSprint(
        "\n    %s| %s"
        ""_Stz,
        m_sIndent.azChar(),
        szNextStage);
  }
}

/*----------------------------------------------------------------------------*/
/* append a pipeline                                                          */
/*----------------------------------------------------------------------------*/
void Backend::vAppendPipeline(Stz* sPipeline) {

  m_sPipeline->vCatSprint(
      "%spipeline %s\n"
      "%sbegin\n"
      ""_Stz,
      m_sIndent.azChar(),
      sPipeline,
      m_sIndent.azChar());
}

/*----------------------------------------------------------------------------*/
/* append the start stage to the pipeline                                     */
/*----------------------------------------------------------------------------*/
void Backend::vAppendStartStage() {

  m_bStartStage = true;

  m_sPipeline->vCatSprint(
      "  %sstage stageStart on origin :\n"
      "    %sstart\n"
      "    %s| pulse once"
      ""_Stz,
      m_sIndent.azChar(), m_sIndent.azChar(), m_sIndent.azChar()
      );
}

/*----------------------------------------------------------------------------*/
/* get routing of this stage                                                  */
/*----------------------------------------------------------------------------*/
Stz* Backend::szGetRouting(Backend* pBackend, QueryNode* pStageMarker) {

  // get the routing of the stage
  QueryNode *pRouting = StageMarker::pRouting(pStageMarker);
  RASSERT(pRouting);
  Routing::eRT eRouting = Routing::eRoutingType(pRouting);

  if (eRouting == Routing::RT_HASHCAST) {

    ASSERT(Hashcast::bIsOfType(pRouting));

    QueryNode* pHashExprList = Hashcast::pHashExprList(pRouting);
    QueryNode* pOrdHashExprList = Hashcast::pOrdinalHashExprList(pRouting, pBackend->m_pPlanRoot);

    RASSERT(pHashExprList);
    RASSERT(pOrdHashExprList);

    pBackend->m_bCombinedOp = true;
    //pBackend->vAppendNextOp("hash 2"_Stz);
    pBackend->m_sPipeline->vCatSprint(
        "\n    %s| hash 2 \""
        ""_Stz,
        pBackend->m_sIndent.azChar()
    );


    if (pHashExprList->nChildren()) {

      i8 i = 0;

      while (i < pHashExprList->nChildren()) {
        QueryNode *pExprNode = _pQueryNode(pHashExprList->pNthChild(i));
        QueryNode *pOrdExprNode = _pQueryNode(pOrdHashExprList->pNthChild(i));

        vAppendExpr(pBackend, pExprNode, pOrdExprNode, nullptr);

        ++i;
      }
    }

    pBackend->m_sPipeline->vCatSprint("  \""_Stz);

    delete pOrdHashExprList;
  }

  Stz *szRouting = nullptr;
  if (eRouting == Routing::RT_BROADCAST || eRouting == Routing::RT_HASHCAST)
    szRouting = Routing::szRoutingType(pRouting);

  return szRouting;
}

/*----------------------------------------------------------------------------*/
/* is given node a node in a table subquery ?                                 */
/*----------------------------------------------------------------------------*/
bool Backend::bInTableSubquery(QueryNode* pNode) {

  // Get the path of the current node in the graph
  s2K sNodePath;
  i8 iNodeDepth;

  b1 bFound = GraphNav::bGetPathAndDepth(pNode, m_pPlanRoot, sNodePath, iNodeDepth, true);
  ASSERT(bFound);

  // First find the main subquery that this SubqueryRefOp is a child of.
  // If we walk the graph bottom-up along the path obtained above
  // then it will be the first main subquery that we come across.
  QueryNode* pTableSubquery = nullptr;
  s2K sTableSubqueryPath;

  // Lambda to call when we walk the graph upwards from the current node back to the root.
  auto fVisitor =
      [&pTableSubquery, &sTableSubqueryPath](PlanNode* pItem, s2K& sCurPath, i8 iDepth, GraphNav::eWalkEvent eEvent, void* pvArg) -> GraphNav::eWalkCtrl {

        if (!TableSubquery::bIsOfType(pItem)) {
          return GraphNav::WALK_CONTINUE;
        }

        pTableSubquery = _pQueryNode(pItem);
        sTableSubqueryPath.vCopy(sCurPath.szStz());
        return GraphNav::WALK_ABORT;
      };

  // Invoke the walk and check the results.
  b1 bWalkResult = GraphNav::bWalkPath(m_pPlanRoot, sNodePath, nullptr, fVisitor, nullptr);

  // we should have aborted the walk when we found the node
  if (!bWalkResult) {
    ASSERT(pTableSubquery);
    ASSERT(sTableSubqueryPath.nLength() > 0);
    return true;
  }

  return false;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
