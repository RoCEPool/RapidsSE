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
#include "store/sqlCompiler/shadowNodes/planObjects/planMarkers/StageMarker.hpp"
#include "store/sqlCompiler/shadowNodes/planObjects/planMarkers/statementMarkers/dml/ExplainMarker.hpp"
#include "store/sqlCompiler/shadowNodes/planObjects/planMarkers/subqueryMarkers/ExprSubquery.hpp"
#include "store/sqlCompiler/shadowNodes/planObjects/planMarkers/subqueryMarkers/SubqueryMarker.hpp"
#include "store/sqlCompiler/shadowNodes/planObjects/planMarkers/subqueryMarkers/MainSubquery.hpp"
#include "store/sqlCompiler/shadowNodes/planObjects/planMarkers/subqueryMarkers/TableSubquery.hpp"
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

#include "AbstractJoinOp.hpp"
#include "AggregateOp.hpp"

#include "ExprList.hpp"

#include "CaseExpr.hpp"
#include "Expr.hpp"
#include "ExtractExpr.hpp"
#include "ComparisonExpr.hpp"
#include "EqExpr.hpp"
#include "ElseExpr.hpp"
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
#include "WhenExpr.hpp"

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
#include "ColumnParameter.hpp"
#include "ColumnRef.hpp"

#include "FilterOp.hpp"
#include "Hint.hpp"

#include "BooleanLiteral.hpp"
#include "DateLiteral.hpp"
#include "DecimalLiteral.hpp"
#include "FloatLiteral.hpp"
#include "IntegerLiteral.hpp"
#include "IntervalLiteral.hpp"
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

#include "ConditionalQueryNodeIter.hpp"
#include "TypeSystem.hpp"

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

  m_sPipeline        = nullptr;
  m_bHasUnionOp      = false;
  m_bNewStage        = false;
  m_bStartStage      = false;
  m_bCombinedOp      = false;
  m_pPrevStageMarker = nullptr;
  m_bInSubquery      = false;
  m_bSubqueryRef     = false;
  m_sIndent          = ""_Stz;

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
bool Backend::bTransliterate(Stz* sPipelineName, String* sPipeline) {
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

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
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

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate the pipeline

  // append the main pipeline name
  vAppendPipeline(sPipelineName);

  // We want to iterate bottom up over each logical subquery and generate the pipeline for each subquery
  GraphNav::bGlobalWalkBottomUp(pStartingNode, false, true, vVisitorSubQuery, (void *) this);

  // pStartingNode's last child should be the main outer query
  QueryNode* pMainSubQuery = _pQueryNode(pStartingNode->pLastChild());

  // The MainSubquery node should have one child which is a StageMarker node
  ASSERT(pMainSubQuery->nChildren() == 1);
  ASSERT(StageMarker::bIsOfType(pMainSubQuery->pFirstChild()));
  QueryNode* pStageMarker = _pQueryNode(pMainSubQuery->pFirstChild());

  // append the start stage
  vAppendStartStage();

  // generate the pipeline for the main outer query
  GraphNav::bGlobalWalk(pStageMarker, false, true, vVisitorPlan, (void *) this);

  // append the last stage
  vAppendLastStage("on origin"_Stz);

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
  //s256 sAliasName;

	// check if the ordinalExpr is an aliasRef
	if (AliasRef::bIsOfType(pOrdinalExpr)) {
		QueryNode* pAliasExpr = AliasRef::pAliasedExpr(pOrdinalExpr);
		vAppendExpr(pBackend, pExprNode, pAliasExpr, nullptr);
		return;
	}

//  if (szAliasName) {
//    sAliasName = szAliasName;
//  }

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // type
  eCT eCTType    = Expr::xTypeId(pExprNode, pBackend->m_pPlanRoot).eCTVal();
	TypeId xTypeId = Expr::xTypeId(pExprNode, pBackend->m_pPlanRoot);
  i8 iPrecision  = Expr::iPrecision(pExprNode, pBackend->m_pPlanRoot);
  i8 iScale      = Expr::iScale(pExprNode, pBackend->m_pPlanRoot);
	//vTrace("TypeId %s %lld %s\n"_Stz, xTypeId.szTypeName(), xTypeId.iIdVal(), aIV_field2[xTypeId.eIVVal()]);

#ifdef TYPE_DETAILED
    
  switch (eCTType) {
      case CT_BOOLEAN:
      case CT_CHAR:
      case CT_DATE:
      case CT_DECIMAL:
      case CT_FLOAT:
      case CT_INTEGER:
      case CT_TIME:
      case CT_TIMESTAMP:
      case CT_VARCHAR:
      case CT_UNKNOWN:
          sPipeline->vCatSprint(
                                " type %s"
                                ""_Stz,
                                aCT[eCTType]);

          if (iPrecision >= 0) {
              if (iScale >= 0) {
                  sPipeline->vCatSprint(" ( %lld %lld )"_Stz, iPrecision, iScale);
              } else {
                  sPipeline->vCatSprint(" ( %lld )"_Stz, iPrecision);
              }
          }
          break;

      case CT_INTERVAL_DT:
      case CT_INTERVAL_YM:
          sPipeline->vCatSprint(
                                " type %s"
                                ""_Stz,
                                aIV_field2[xTypeId.eIVVal()]);
          break;

      default:
          NEVER_REACH
  }

#else

	if (eCTType == CT_INTERVAL_YM || eCTType == CT_INTERVAL_DT) {
		sPipeline->vCatSprint(
			" type %s"
			""_Stz,
			aIV_field2[xTypeId.eIVVal()]);
	}
	else {
		sPipeline->vCatSprint(
			" type %s"
			""_Stz,
			aCT[eCTType]);
	}
#endif


//  if ( ! szAliasName) {
//    if (ColumnRef::bIsOfType(pExprNode)) {
//      QueryNode *pColumnRef = static_cast<QueryNode *>(pExprNode);
//      szAliasName = ColumnRef::szColumn(pColumnRef);
//
//      // We cannot modify the szAliasName directly because we don't know the
//      // size of the underlying buffer. We need to modify a copy of it instead.
//      if (szAliasName) {
//        sAliasName = szAliasName;
//        StrLib::vToLower(sAliasName.azChar(), sAliasName.nAreaBytes());
//      }
//    }
//  }

  if (szAliasName && !szAliasName->bSame(""_Stz)) {
    sPipeline->vCatSprint(
        " as %s"
        ""_Stz,
        szAliasName->azChar());
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

  eCT eType      = Expr::xTypeId(pExprNode, pBackend->m_pPlanRoot).eCTVal();
	TypeId xTypeId = Expr::xTypeId(pExprNode, pBackend->m_pPlanRoot);
  i8 iPrecision  = Expr::iPrecision(pExprNode, pBackend->m_pPlanRoot);
  i8 iScale      = Expr::iScale(pExprNode, pBackend->m_pPlanRoot);

#ifdef TYPE_DETAILED

  switch (eType) {
    case CT_BOOLEAN:
    case CT_CHAR:
    case CT_DATE:
    case CT_DECIMAL:
    case CT_FLOAT:
    case CT_INTEGER:
    case CT_TIME:
    case CT_TIMESTAMP:
    case CT_VARCHAR:
    case CT_UNKNOWN:
      sPipeline->vCatSprint(
          " type %s"
          ""_Stz,
          aCT[eType]);

      if (iPrecision >= 0) {
        if (iScale >= 0) {
          sPipeline->vCatSprint(" ( %lld %lld )"_Stz, iPrecision, iScale);
        } else {
          sPipeline->vCatSprint(" ( %lld )"_Stz, iPrecision);
        }
      }
      break;

    case CT_INTERVAL_DT:
    case CT_INTERVAL_YM:
      sPipeline->vCatSprint(
          " type %s"
          ""_Stz,
          aIV_field2[xTypeId.eIVVal()]);
      break;

    default:
      NEVER_REACH
  }

#else
	if (eType == CT_INTERVAL_YM || eType == CT_INTERVAL_DT) {
		sPipeline->vCatSprint(
						" type %s"
						""_Stz,
						aIV_field2[xTypeId.eIVVal()]);
	}
	else {
		sPipeline->vCatSprint(
						" type %s"
						""_Stz,
						aCT[eType]);
	}
#endif

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

    pBackend->vAppendNextOp("gbyAgg \""_Stz);
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

			if (AliasRef::bIsOfType(pOrdinalExpr)) {
				QueryNode* pAliasExpr = AliasRef::pAliasedExpr(pOrdinalExpr);
				vAppendExpr(pBackend, pGroupByExpr, pAliasExpr, nullptr);
			}
			else
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
  pBackend->m_pColExprList = nullptr;
}

/*----------------------------------------------------------------------------*/
/* append call to subquery to the pipeline                                    */
/*----------------------------------------------------------------------------*/
void Backend::vAppendCallSubQry(Backend* pBackend, QueryNode* pNode)
{
  _static_

  s4K sCallSubqry;
  //String* sPipeline = pBackend->m_sPipeline;

  sCallSubqry.vSprint("call \"%s \""_Stz, SubqueryRefOp::szAliasName(pNode));
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

  pBackend->m_pColExprList = PlanObject::pColumnExprs(_pQueryNode(pNode->pFirstChild()), pBackend->m_pPlanRoot);

	pBackend->vAppendNextOp("filter \" type BOOLEAN "_Stz);

  pBackend->m_bCombinedOp = true;
//  sPipeline->vCatSprint(
//      "    filter 1 \" type BOOLEAN "
//      ""_Stz);

  // Walk the expr node plan graph
  //GraphNav::bGlobalWalkBottomUp(pOrdinalFilterExpr, false, true, vVisitorExpr, (void *) pBackend);
  GraphNav::bGlobalWalk(pOrdinalFilterExpr, false, true,vVisitorExpr, vVisitorExpr, (void *) pBackend);
  delete pOrdinalFilterExpr;

  // cleanup
  pBackend->m_pColExprList->vDeleteAll();
  delete pBackend->m_pColExprList;
  pBackend->m_pColExprList = nullptr;

  sPipeline->vCatSprint(" \""_Stz);

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
    sLimit.vCatSprint("limit %lld offset %lld"_Stz, LimitOp::iLimit(pNode), iOffset);
  }
  else {
    sLimit.vCatSprint("limit %lld"_Stz, LimitOp::iLimit(pNode));
  }
  pBackend->m_bCombinedOp = true;

  pBackend->vAppendNextOp(sLimit);
}

/*----------------------------------------------------------------------------*/
/* append abstractjoinOp to the pipeline                                      */
/*----------------------------------------------------------------------------*/
void Backend::vAppendAbstractJoinOp(Backend* pBackend, QueryNode* pNode)
{
  _static_

  String* sPipeline = pBackend->m_sPipeline;

  pBackend->m_pColExprList = JoinOp::pColumnExprs(pNode, pBackend->m_pPlanRoot);
  QueryNode* pOrdinalJoinPredicate = JoinOp::pOrdinalJoinPredicate(pNode, pBackend->m_pPlanRoot);

  pBackend->vAppendNextOp("abstractJoin \""_Stz);
  switch(JoinOp::eJoinType(pNode)) {
    case JT_INNER:                   /* sPipeline->vCatSprint("INNER"_Stz); */ break;
    case JT_CROSS:                   sPipeline->vCatSprint("CROSS"_Stz); break;
    case JT_SEMI:                    sPipeline->vCatSprint("SEMI"_Stz); break;
    case JT_ANTI:                    sPipeline->vCatSprint("ANTI"_Stz); break;
    case JT_LEFT_OUTER:              sPipeline->vCatSprint("LEFT_OUTER"_Stz); break;
    case JT_RIGHT_OUTER:             sPipeline->vCatSprint("RIGHT_OUTER"_Stz); break;
    case JT_FULL_OUTER:              sPipeline->vCatSprint("FULL_OUTER"_Stz); break;
    case JT_NATURAL_INNER:           sPipeline->vCatSprint("NATURAL_INNER"_Stz); break;
    case JT_NATURAL_LEFT_OUTER:      sPipeline->vCatSprint("NATURAL_LEFT_OUTER"_Stz); break;
    case JT_NATURAL_RIGHT_OUTER:     sPipeline->vCatSprint("NATURAL_RIGHT_OUTER"_Stz); break;
    case JT_NATURAL_FULL_OUTER:      sPipeline->vCatSprint("NATURAL_FULL_OUTER"_Stz); break;
    default:
      break;
  }

  GraphNav::bGlobalWalk(pOrdinalJoinPredicate, false, true,vVisitorExpr, vVisitorExpr, (void *) pBackend);
  sPipeline->vCatSprint(" \""_Stz);

  // cleanup
  pBackend->m_pColExprList->vDeleteAll();
  delete pBackend->m_pColExprList;
  pBackend->m_pColExprList = nullptr;

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

    pBackend->vAppendNextOp("sort \""_Stz);
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
  pBackend->m_pColExprList = nullptr;
}

/*----------------------------------------------------------------------------*/
/* append project Op to the pipeline                                          */
/*----------------------------------------------------------------------------*/
void Backend::vAppendProjectOp(Backend* pBackend, QueryNode* pNode)
{
  _static_

  bool bFirstExpr = false;
  String* sPipeline = pBackend->m_sPipeline;
	s256 sAliasName;

  // Get the list of projected expressions from ProjectOp
  ASSERT(pNode->nChildren() == 1);
  QueryNode *pProjectedColumns = ProjectOp::pProjectedColumns(pNode);
  ASSERT(pProjectedColumns);

  ASSERT(PlanObject::bIsOfType(pNode->pFirstChild()));
  pBackend->m_pColExprList = PlanObject::pColumnExprs(_pQueryNode(pNode->pFirstChild()), pBackend->m_pPlanRoot);

  // Get the list of correlated expressions from the enclosing subquery.
  // We don't need to free this object.
  QueryNode* pCorrelatedExprs = PlanObject::pCorrelatedExprs(pNode, pBackend->m_pPlanRoot);

  if (pProjectedColumns->bHasChildren()) {

    // get a plan node iterator
    PlanNodeIter *pIter = pProjectedColumns->pNewChildrenIter();

    // Iterate each projected expression to see if there are any aggregates
    bool bHasAggregate = false;

    while (pIter->bHasNext()) {

      // get the next projectedColumn from the iterator
      QueryNode *pProjectedColumn = static_cast<QueryNode *>(pIter->pNext());
      ASSERT(pProjectedColumn);
      ASSERT(ProjectedColumn::bIsOfType(pProjectedColumn));

      QueryNode* pExprNode  = ProjectedColumn::pExprNode(pProjectedColumn);

      QueryNode* pOrdinalExpr = Expr::pSymbolicToOrdinal(pExprNode, pBackend->m_pColExprList, pCorrelatedExprs, pBackend->m_pPlanRoot);
      RASSERT(pOrdinalExpr);

      if (AggregateExpr::bIsOfType(pOrdinalExpr)) {
        bHasAggregate = true;
      }

      delete pOrdinalExpr;
    }

    delete pIter;

    // does the projection have aggregates ?
    if (bHasAggregate)
      pBackend->vAppendNextOp("aggreg \""_Stz);
    else
      pBackend->vAppendNextOp("project \""_Stz);

    pBackend->m_bCombinedOp = true;

    // get a plan node iterator
    pIter = pProjectedColumns->pNewChildrenIter();

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

			if ( ! szAliasName) {
				if (ColumnRef::bIsOfType(pExprNode)) {
					QueryNode *pColumnRef = static_cast<QueryNode *>(pExprNode);
					szAliasName = ColumnRef::szColumn(pColumnRef);

					// We cannot modify the szAliasName directly because we don't know the
					// size of the underlying buffer. We need to modify a copy of it instead.
					if (szAliasName) {
						sAliasName = szAliasName;
						StrLib::vToLower(sAliasName.azChar(), sAliasName.nAreaBytes());
					}
				}
			}
			else {
				sAliasName = szAliasName;
			}

			RASSERT(pBackend->m_pColExprList);
      RASSERT(pCorrelatedExprs);

      QueryNode* pOrdinalExpr = Expr::pSymbolicToOrdinal(pExprNode, pBackend->m_pColExprList, pCorrelatedExprs, pBackend->m_pPlanRoot);
      RASSERT(pOrdinalExpr);

      //vAppendExpr(pBackend, pColExprList, pCorrelatedExprs, pExprNode, szAliasName);
      vAppendExpr(pBackend, pExprNode, pOrdinalExpr, sAliasName);
      delete pOrdinalExpr;
    }
    delete pIter;

  }

  sPipeline->vCatSprint(" \""_Stz);

  // cleanup
  pBackend->m_pColExprList->vDeleteAll();
  delete pBackend->m_pColExprList;
  pBackend->m_pColExprList = nullptr;
}

/*----------------------------------------------------------------------------*/
/* append scan Op to the pipeline                                             */
/*----------------------------------------------------------------------------*/
void Backend::vAppendScanOp(Backend* pBackend, QueryNode* pNode)
{
  _static_

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
    UNUSED(sPath);
    UNUSED(iDepth);
    UNUSED(pvArg);

    if (StageMarker::bIsExactType(pNode)) {
      pLeafStageMarker = pNode;
      return GraphNav::WALK_ABORT;
    }
    return GraphNav::WALK_CONTINUE;
  };

  // add child op's

  GraphNav::bGlobalWalkBottomUp(pChildStageMarker1, false, true, fLeafStageMarker, (void *) pBackend);
  RASSERT(pLeafStageMarker);

  // check for a unionOp
  auto fHasUnionOp = [](PlanNode* pNode, void* pvArg) -> b1 {
    UNUSED(pvArg);
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

    if (CaseExpr::bIsOfType(pOrdinalExpr)) {

      QueryNode* pSymbolicExpr = Expr::pOrdinalToSymbolic(pOrdinalExpr, pBackend->m_pColExprList,
                                                          pBackend->m_pPlanRoot);
      eCT eType = CaseExpr::xTypeId(pSymbolicExpr, pBackend->m_pPlanRoot).eCTVal();
			TypeId xTypeId = Expr::xTypeId(pSymbolicExpr, pBackend->m_pPlanRoot);

#ifdef TYPE_DETAILED

      i8 iPrecision  = Expr::iPrecision(pSymbolicExpr, pBackend->m_pPlanRoot);
      i8 iScale      = Expr::iScale(pSymbolicExpr, pBackend->m_pPlanRoot);

      switch (eType) {
        case CT_BOOLEAN:
        case CT_CHAR:
        case CT_DATE:
        case CT_DECIMAL:
        case CT_FLOAT:
        case CT_INTEGER:
        case CT_TIME:
        case CT_TIMESTAMP:
        case CT_VARCHAR:
        case CT_UNKNOWN:
          sPipeline->vCatSprint(
              " type %s"
              ""_Stz,
              aCT[eType]);

          if (iPrecision >= 0) {
            if (iScale >= 0) {
              sPipeline->vCatSprint(" ( %lld %lld )"_Stz, iPrecision, iScale);
            } else {
              sPipeline->vCatSprint(" ( %lld )"_Stz, iPrecision);
            }
          }
          break;

        case CT_INTERVAL_DT:
        case CT_INTERVAL_YM:
          sPipeline->vCatSprint(
              " type %s"
              ""_Stz,
              aIV_field2[xTypeId.eIVVal()]);
          break;

        default:
          NEVER_REACH
      }
#else
			if (eType == CT_INTERVAL_YM || eType == CT_INTERVAL_DT) {
				sPipeline->vCatSprint(
								" } type %s"_Stz, aIV_field2[xTypeId.eIVVal()]);
			}
			else {
				sPipeline->vCatSprint(" } type %s"_Stz, aCT[eType]);
			}
#endif

      delete pSymbolicExpr;

      // get the when expr
      ConditionalQueryNodeIter *pWhenIter = CaseExpr::pWhenExprIter(_pQueryNode(pOrdinalExpr));

      while (pWhenIter->bHasNext()) {
        QueryNode *pWhenExpr = pWhenIter->pNext();
        ASSERT(WhenExpr::bIsOfType(pWhenExpr));

        QueryNode *pTestExpr = WhenExpr::pPredicateExpr(_pQueryNode(pWhenExpr));
        QueryNode *pResultExpr = WhenExpr::pResultExpr(_pQueryNode(pWhenExpr));

        // Walk the then testexpr node plan graph
        GraphNav::bGlobalWalk(pTestExpr, false, true, vVisitorExpr, vVisitorExpr, (void *) pBackend);

        sPipeline->vConcat(" WHEN "_Stz);

        // Walk the then resultexpr node plan graph
        GraphNav::bGlobalWalk(pResultExpr, false, true, vVisitorExpr, vVisitorExpr, (void *) pBackend);

        sPipeline->vConcat(" THEN "_Stz);
      }
      delete pWhenIter;

      // get the else expr
      QueryNode *pElseExpr = CaseExpr::pElseExpr(_pQueryNode(pOrdinalExpr));

      // Walk the else expr node plan graph
      GraphNav::bGlobalWalk(pElseExpr, false, true, vVisitorExpr, vVisitorExpr, (void *) pBackend);

      sPipeline->vCatSprint(" ELSE "_Stz);

      pWhenIter = CaseExpr::pWhenExprIter(_pQueryNode(pOrdinalExpr));
      while (pWhenIter->bHasNext()) {
        QueryNode* pWhenExpr = pWhenIter->pNext();
        UNUSED(pWhenExpr);
        sPipeline->vCatSprint(" ENDIF "_Stz);
      }

      sPipeline->vCatSprint(" CASE "_Stz);
      delete pWhenIter;

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
      if (pBackend->m_bSubqueryRef) {
        sPipeline->vCatSprint(" in "_Stz);
        pBackend->m_bSubqueryRef = false;
      }
      else
        sPipeline->vCatSprint(" inList "_Stz);

      // TODO: CHECK THIS OUT
      return GraphNav::WALK_SKIP_CHILDREN;
    }

    //delete pOrdinalExpr;

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

  if (ColumnParameter::bIsOfType(pOrdinalExpr)) {
    QueryNode* pColumnParameter = _pQueryNode(pOrdinalExpr);


    i8 iColumnParameter = ColumnParameter::iParameter(pColumnParameter);

    sPipeline->vCatSprint(
        " %%%lld"
        ""_Stz,
        iColumnParameter);
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



        // ALKA: to do in exprEval
        if (bIsDistinct) {
          sPipeline->vCatSprint(
              " countDistinct "_Stz);
        }
        else {
          sPipeline->vCatSprint(
              " count "_Stz);
        }
      }

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
      // nothing to do
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
    StrLib::vToLower(sExtractType.azChar(), sExtractType.nAreaBytes());

    sPipeline->vCatSprint(" extract_%s "_Stz, sExtractType.azChar());

    return GraphNav::WALK_CONTINUE;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // literal

  if (LiteralExpr::bIsOfType(pOrdinalExpr)) {

    if (BooleanLiteral::bIsOfType(pOrdinalExpr)) {
      sPipeline->vCatSprint(" %s "_Stz, BooleanLiteral::bValue(pOrdinalExpr) ? "true" : "false");
    }

    if (DateLiteral::bIsOfType(pOrdinalExpr)) {
      SP_Dte spDte;
      sPipeline->vCatSprint(" DATE '%s' "_Stz, (char*)spDte.sz(DateLiteral::xValue(pOrdinalExpr)));
    }

    if (DecimalLiteral::bIsOfType(pOrdinalExpr)) {
      Bcd xBcd;
      SP_Bcd spBcd;
      xBcd = DecimalLiteral::xValue(pOrdinalExpr);
      sPipeline->vCatSprint(" %s "_Stz, (char*)spBcd.sz(xBcd.T.m_iMantissa, xBcd.T.m_uScale));
    }

    if (FloatLiteral::bIsOfType(pOrdinalExpr)) {
      // We print with 40 digits because C_FLT_MAX specifies 39 digits.
      // This is to avoid any errors with rounding the last decimal digit.
      sPipeline->vCatSprint(" %.40e "_Stz, FloatLiteral::dValue(pOrdinalExpr));
    }

    if (IntegerLiteral::bIsOfType(pOrdinalExpr)) {
      sPipeline->vCatSprint(" %lld "_Stz, IntegerLiteral::iValue(pOrdinalExpr));
    }

    if (IntervalLiteral::bIsOfType(pOrdinalExpr)) {
      Ivl xIvl = IntervalLiteral::xValue(pOrdinalExpr);
      if (xIvl.T.m_xIvl.m_eType < IV_SUBCATEGORY_YEARMONTH_END)
        sPipeline->vCatSprint(" INTERVAL_YM %lld "_Stz, xIvl.operator i8());
      else
        sPipeline->vCatSprint(" INTERVAL_DT %lld "_Stz, xIvl.operator i8());
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

      eCT eType = CastExpr::xTypeId(pSymbolicExpr, pBackend->m_pPlanRoot).eCTVal();
			TypeId xTypeId = Expr::xTypeId(pSymbolicExpr, pBackend->m_pPlanRoot);
      i8 iPrecision  = Expr::iPrecision(pSymbolicExpr, pBackend->m_pPlanRoot);
      i8 iScale      = Expr::iScale(pSymbolicExpr, pBackend->m_pPlanRoot);

#ifdef TYPE_DETAILED

//			i8 iLength = pBackend->iGetLength(eType);
//			i8 iScale = pBackend->iGetScale(eType);
//
//			if (eType == CT_INTERVAL_YM || eType == CT_INTERVAL_DT) {
//				sPipeline->vCatSprint(
//								" type %s"
//								""_Stz,
//								aIV_field2[xTypeId.eIVVal()]);
//			}
//			else {
//
//				if (iLength == C_UNKNOWN && iScale == C_UNKNOWN)
//					sPipeline->vCatSprint(
//									" casttype %s CAST"
//									""_Stz,
//									aCT[eType]
//					);
//				else
//				if (iScale == C_UNKNOWN)
//					sPipeline->vCatSprint(
//									" casttype %s ( %lld ) CAST"
//									""_Stz,
//									aCT[eType],
//									iLength
//					);
//				else
//					sPipeline->vCatSprint(
//									" casttype %s ( %lld %lld ) CAST"
//									""_Stz,
//									aCT[eType],
//									iLength,
//									iScale
//					);
//			}

      switch (eType) {
        case CT_BOOLEAN:
        case CT_CHAR:
        case CT_DATE:
        case CT_DECIMAL:
        case CT_FLOAT:
        case CT_INTEGER:
        case CT_TIME:
        case CT_TIMESTAMP:
        case CT_VARCHAR:
        case CT_UNKNOWN:
          sPipeline->vCatSprint(
              " type %s"
              ""_Stz,
              aCT[eType]);

          if (iPrecision >= 0) {
            if (iScale >= 0) {
              sPipeline->vCatSprint(" ( %lld %lld )"_Stz, iPrecision, iScale);
            } else {
              sPipeline->vCatSprint(" ( %lld )"_Stz, iPrecision);
            }
          }

          sPipeline->vConcat(" CAST"_Stz);
          break;

        case CT_INTERVAL_DT:
        case CT_INTERVAL_YM:
          sPipeline->vCatSprint(
              " type %s"
              ""_Stz,
              aIV_field2[xTypeId.eIVVal()]);
          break;

        default:
          NEVER_REACH
      }
#else
			if (eType == CT_INTERVAL_YM || eType == CT_INTERVAL_DT) {
				sPipeline->vCatSprint(
								" casttype %s CAST "
								""_Stz,
								aIV_field2[xTypeId.eIVVal()]);
			}
			else {
				sPipeline->vCatSprint(
								" casttype %s CAST "
								""_Stz,
								aCT[eType]);
			}
#endif

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
    pBackend->m_bSubqueryRef = true;
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

  UNUSED(sPath);
  UNUSED(iDepth);

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
      vAppendScanOp(pBackend, pQueryNode);
    }

    else if (AbstractJoinOp::bIsOfType(pQueryNode)) {
      vAppendAbstractJoinOp(pBackend, pQueryNode);
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

  UNUSED(sPath);
  UNUSED(iDepth);
  UNUSED(pvArg);

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
    UNUSED(pvArg);
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
  UNUSED(sPath);
  UNUSED(iDepth);

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
  GraphNav::bBoundedWalkBottomUp(pStageMarker, false, true, vVisitorStatement, (void *) pBackend);

  // Keep walking
  return GraphNav::WALK_CONTINUE;
}

/*----------------------------------------------------------------------------*/
/* visitor function for the stage                                             */
/*----------------------------------------------------------------------------*/
GraphNav::eWalkCtrl Backend::vVisitorSubQuery(PlanNode* pRootNode, s2K& sPath, i8 iDepth, void* pvArg)
{
  _static_
  UNUSED(sPath);
  UNUSED(iDepth);

  // Continue walking till a subQueryMarker is hit
  if (!TableSubquery::bIsOfType(pRootNode) &&
      !ExprSubquery::bIsOfType(pRootNode)) {
    return GraphNav::WALK_CONTINUE;
  }

  // get the backend
  Backend* pBackend = (Backend*)pvArg;

  QueryNode* pSubqueryMarker = _pQueryNode(pRootNode);
  ASSERT(SubqueryMarker::bIsOfType(pSubqueryMarker));

  pBackend->m_bInSubquery = true;
  pBackend->m_sIndent = "  "_Stz;

  // The Subquery node should have one child which is a StageMarker node
  ASSERT(pSubqueryMarker->nChildren() == 1);
  ASSERT(StageMarker::bIsOfType(pSubqueryMarker->pFirstChild()));

  pBackend->m_sPipeline->vCatSprint(
      "%spipeline %s\n"
      ""_Stz,
      pBackend->m_sIndent.azChar(),
      SubqueryMarker::szTableAlias(pSubqueryMarker));

  // Get the list of correlated columns
  QueryNode* pCorrColumnsNode = SubqueryMarker::pCorrelatedColumnsNode(pSubqueryMarker);
  ASSERT(pCorrColumnsNode);
  ASSERT(ExprList::bIsOfType(pCorrColumnsNode));

  QueryNode* pExprList = _pQueryNode(pCorrColumnsNode);

  if (pExprList->bHasChildren()) {
    //PlanNodeIter* pIter = pExprList->pNewChildrenIter();

    PlanNodeIter* pExprIter = pExprList->pNewChildrenIter();

    pBackend->m_sPipeline->vCatSprint(
        "  %sparams \""
        ""_Stz,
        pBackend->m_sIndent.azChar());

    bool bFirstExpr = true;
    while (pExprIter->bHasNext()) {

      if (bFirstExpr) {
        bFirstExpr = false;
      } else {
        pBackend->m_sPipeline->vCatSprint(" , "_Stz);
      }

      // get the next parameter from the iterator
      QueryNode *pParam = static_cast<QueryNode *>(pExprIter->pNext());
      ASSERT(pParam);
      ASSERT(ColumnRef::bIsOfType(pParam));

      pBackend->m_sPipeline->vCatSprint("%s"_Stz, ColumnRef::szColumn(pParam));

    }

    pBackend->m_sPipeline->vCatSprint(" \"\n"_Stz);
    delete pExprIter;
  }

  // append begin
  pBackend->m_sPipeline->vCatSprint(
      "%sbegin\n"
      ""_Stz,
      pBackend->m_sIndent.azChar());

  RASSERT(pSubqueryMarker->bHasChildren());

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // walk the main query of this subQuery

  QueryNode* pStageMarker = _pQueryNode(pSubqueryMarker->pFirstChild());
  ASSERT(StageMarker::bIsOfType(pStageMarker));

  PlanNodeIter* pIter = pStageMarker->pNewChildrenIter();
  while (pIter->bHasNext()) {
    QueryNode *pQueryNode = static_cast<QueryNode *>(pIter->pNext());

    // is it a main subQuery
    if (MainSubquery::bIsOfType(pQueryNode)) {

      QueryNode* pStageMarker_main = _pQueryNode(pQueryNode->pFirstChild());
      ASSERT(StageMarker::bIsOfType(pStageMarker_main));

      // append the start stage
      pBackend->vAppendStartStage();
      GraphNav::bGlobalWalk(pStageMarker_main, false, true, vVisitorPlan, (void *) pBackend);
    }

  }
  delete pIter;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // append the last stage
  pBackend->vAppendLastStage("on origin"_Stz);

  pBackend->m_bInSubquery = false;
  pBackend->m_sIndent = ""_Stz;

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
    pBackend->m_sPipeline->vCatSprint(
        "\n    %s| hash \""
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
  RASSERT(bFound);

  // First find the main subquery that this SubqueryRefOp is a child of.
  // If we walk the graph bottom-up along the path obtained above
  // then it will be the first main subquery that we come across.
  QueryNode* pTableSubquery = nullptr;
  s2K sTableSubqueryPath;

  // Lambda to call when we walk the graph upwards from the current node back to the root.
  auto fVisitor =
      [&pTableSubquery, &sTableSubqueryPath](PlanNode* pItem, s2K& sCurPath, i8 iDepth, GraphNav::eWalkEvent eEvent, void* pvArg) -> GraphNav::eWalkCtrl {

        UNUSED(iDepth);
        UNUSED(eEvent);
        UNUSED(pvArg);

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
