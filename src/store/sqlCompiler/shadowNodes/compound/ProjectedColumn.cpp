/*============================================================================*/
/* ProjectedColumn.hpp - an expression that may also have an alias            */
/*============================================================================*/

#include "BASE.h"
#include "ProjectedColumn.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "PlanConstants.hpp"
#include "NodeNamer.hpp"
#include "Expr.hpp"
#include "AvgExpr.hpp"
#include "SumExpr.hpp"
#include "CountExpr.hpp"

#include "GraphNav.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_PROJECTEDCOLUMN_DEFN = "/__root__/definitions/object/compound/projectedColumn"_Stz;
static Stz* C_PROJECTEDCOLUMN_NAME = "projectedColumn"_Stz;
static Stz* C_EXPR_PROP  = "expr"_Stz;
static Stz* C_ALIAS_PROP = "alias"_Stz;


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Returns the aliasName of the projectedColumn                               */
/*----------------------------------------------------------------------------*/
Stz* ProjectedColumn::szAliasName(QueryNode* pNode)
{
  QueryNode *pAliasNode =
      _pQueryNode(GraphNav::pFindProperty(const_cast<Stz *>(C_ALIAS_PROP), pNode));

  if (!pAliasNode) {
    return nullptr;
  }

  return pAliasNode->szValue();;
}

/*----------------------------------------------------------------------------*/
/* Helper function to create ProjectedColumn nodes.                           */
/*----------------------------------------------------------------------------*/
QueryNode* ProjectedColumn::pCreate(QueryNode* pExprNode, Text* pAlias)
{
  _static_

  ASSERT(pExprNode);
  ASSERT(!pAlias || pAlias->nLength() > 0);

  Value xAliasValue(pAlias);

  s256 sName;
  NodeNamer::vGenerateName(C_PROJECTEDCOLUMN_DEFN, sName);

  // Create the ProjectedColumn object
  QueryNode* pProjectedColumn = new QueryNode(sName.szStz(), const_cast<Stz*>(C_PROJECTEDCOLUMN_DEFN));

  // Add the expression as a property
  // Make sure it has the right property name first
  pExprNode->vSetName(C_EXPR_PROP);
  pProjectedColumn->vAppendProperty(pExprNode);

  // Create the alias and append it as a property
  QueryNode* pAliasNode  = new QueryNode(C_ALIAS_PROP, const_cast<Stz*>(C_STRING_OBJ_DEFN), static_cast<Value&&>(xAliasValue));
  pProjectedColumn->vAppendProperty(pAliasNode);

  return pProjectedColumn;
}

/*----------------------------------------------------------------------------*/
/* Appends a description of the graph node into a buffer                      */
/*----------------------------------------------------------------------------*/
void ProjectedColumn::vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode)
{
  _static_

  vDescribe(pNode, sBuf, nullptr, nullptr, pRootNode);
}

/*----------------------------------------------------------------------------*/
/* Appends a description of the graph node into a buffer                      */
/*----------------------------------------------------------------------------*/
void ProjectedColumn::vDescribe(PlanNode* pNode, s2K& sBuf, QueryNodeList* pColumnExprList,
                                QueryNode* pCorrelatedExprs, PlanNode* pRootNode)
{
  _static_

  ASSERT(bIsOfType(pNode));

  QueryNode* pProjectedColumn = _pQueryNode(pNode);

  QueryNode* pExprNode = _pQueryNode(GraphNav::pFindProperty(C_EXPR_PROP, pProjectedColumn));
  QueryNode* pAliasNode  = _pQueryNode(GraphNav::pFindProperty(C_ALIAS_PROP, pProjectedColumn));

  ASSERT(pExprNode);
  ASSERT(pAliasNode);

  Stz* szAliasName = pAliasNode->szValue();

  // Describe the expression
  s2K sExprBuf;
  Expr::vDescribe(pExprNode, sExprBuf, pRootNode);

  if (szAliasName) {
    sBuf.vSprintCat("Alias = %s ; "_Stz, (szAliasName == nullptr ? "<none>"_Stz : szAliasName));
  }

  sBuf.vSprintCat("Expr = %s"_Stz, sExprBuf.szStz());

  if (pColumnExprList && pRootNode) {
    sExprBuf.vReset();
    QueryNode* pOrdinalExpr = Expr::pSymbolicToOrdinal(pExprNode, pColumnExprList, pCorrelatedExprs, pRootNode);
    Expr::vDescribe(pOrdinalExpr, sExprBuf, pRootNode);
    delete pOrdinalExpr;

    sBuf.vSprintCat(" ; Ordinal Expr = %s"_Stz, sExprBuf.szStz());
  }
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the ProjectedColumn defn    */
/*----------------------------------------------------------------------------*/
b1 ProjectedColumn::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_PROJECTEDCOLUMN_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 ProjectedColumn::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_PROJECTEDCOLUMN_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the expression in this node contains an aggregate expression*/
/*----------------------------------------------------------------------------*/
b1 ProjectedColumn::bContainsAnAggregate(QueryNode* pNode)
{
  QueryNode *pExprNode = ProjectedColumn::pExprNode(pNode);

  return Expr::bContainsAnAggregate(pExprNode);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the expression in this node is literal.                    */
/*----------------------------------------------------------------------------*/
b1 ProjectedColumn::bIsFullyLiteral(QueryNode* pNode)
{
  QueryNode *pExprNode = ProjectedColumn::pExprNode(pNode);

  return Expr::bIsFullyLiteral(pExprNode);
}

/*----------------------------------------------------------------------------*/
/* Returns the exprNode of the ProjectedColumn                                */
/*----------------------------------------------------------------------------*/
QueryNode* ProjectedColumn::pExprNode(QueryNode* pNode)
{
  QueryNode *pExprNode =
      _pQueryNode(GraphNav::pFindProperty(const_cast<Stz *>(C_EXPR_PROP), pNode));

  return pExprNode;
}

/*----------------------------------------------------------------------------*/
/* Returns a new copy of this projected expression rewritten relative to      */
/* column ordinals. Must be freed after use.                                  */
/*----------------------------------------------------------------------------*/
QueryNode* ProjectedColumn::pSymbolicToOrdinal(QueryNode*     pNode,
                                               QueryNodeList* pColExprList,
                                               QueryNode*     pCorrelatedExprs,
                                               PlanNode*      pRootNode)
{
  // Get the expression out of this ProjectedColumn and convert it
  // from symbolic to ordinal representation.
  QueryNode* pSymbolicExpr = pExprNode(pNode);
  ASSERT(pSymbolicExpr);

  // Use the column expression list and the above symbolic column projection expression to
  // generate an ordinal column projection expression (one that is relative to column
  // ordinals only, not column names).
  QueryNode* pOrdinalExpr = Expr::pSymbolicToOrdinal(pSymbolicExpr, pColExprList, pCorrelatedExprs, pRootNode);
  ASSERT(pOrdinalExpr);

  // Get the alias name of this column and (if applicable) duplicate it
  Text* pAlias = nullptr;
  if (ProjectedColumn::szAliasName(pNode)) {
    pAlias = new Text(ProjectedColumn::szAliasName(pNode));
  }

  // Create a new ProjectedColumn and return it
  QueryNode* pOrdinalProjectedColumn = ProjectedColumn::pCreate(pOrdinalExpr, pAlias);

  return pOrdinalProjectedColumn;
}

/*----------------------------------------------------------------------------*/
/* Sets or changes the alias for this ProjectedColumn                         */
/*----------------------------------------------------------------------------*/
void ProjectedColumn::vSetAlias(QueryNode* pNode, Text *pNewAlias)
{
  QueryNode *pAliasNode =
      _pQueryNode(GraphNav::pFindProperty(const_cast<Stz *>(C_ALIAS_PROP), pNode));

  Value xVal(pNewAlias);
  pAliasNode->vSetValue(static_cast<Value&&>(xVal));
}

/*----------------------------------------------------------------------------*/
/* If the given projectedColumn requires partitioning then a non-null         */
/* QueryNodeList is returned which contains those partitioned                 */
/* projected columns.                                                         */
/*----------------------------------------------------------------------------*/
QueryNodeList* ProjectedColumn::pPartitionAggregates(QueryNode* pNode)
{
  // Some aggregate expressions need to be modified to enable partitioning.
  // The main one is AVG(x). It needs to be split into a SUM(x) and a COUNT(x)
  // and then in the super-aggregating ProjectOp it converts this back into an
  // average by calculating SUM(SUM(x))/SUM(COUNT(x)).
  //
  // However the expression may be more complex and partitioning it may actually
  // require multiple columns to be projected. e.g. consider the expression:
  //   AVG(x) + MIN(y)
  // This would require the partition level to emit columns for:
  //   SUM(x), COUNT(x), MIN(y)
  // Then when all partitions are super-aggregated we can reassemble the desired
  // result with:  SUM(SUM(x))/SUM(COUNT(x)) + MIN(MIN(y))


  QueryNodeList* pProjectedColList = nullptr;
  QueryNode* pExpr = pExprNode(pNode);

  // Get a list of aggregate terms appearing in this expression
  QueryNodeList* pAggList = Expr::pAggregateExprs(pExpr, false);

  if (pAggList->nItems() == 0) {
    delete pAggList;
    return pProjectedColList;
  }

  pProjectedColList = new QueryNodeList();
  QueryNodeIter* pIter = pAggList->pNewIter();

  while (pIter->bHasNext()) {
    QueryNode* pAgg = pIter->pNext();

    if (AvgExpr::bIsOfType(pAgg)) {
      ASSERT(pAgg->nChildren() == 1);
      QueryNode* pChild = _pQueryNode(pAgg->pFirstChild());

      // Create the SUM() projected column and add it to the list
      QueryNode* pSumExpr = SumExpr::pCreate(false, pChild->pDeepCopy(true));
      QueryNode* pSumCol  = ProjectedColumn::pCreate(pSumExpr, nullptr);
      pProjectedColList->vAppend(pSumCol);

      // Create the COUNT() projected column and add it to the list
      QueryNode* pCountExpr = CountExpr::pCreate(false, false, pChild->pDeepCopy(true));
      QueryNode* pCountCol  = ProjectedColumn::pCreate(pCountExpr, nullptr);
      pProjectedColList->vAppend(pCountCol);

    } else {
      // An aggregate that is not an AVG()
      ASSERT(AggregateExpr::bIsOfType(pAgg));
      QueryNode* pNewCol = ProjectedColumn::pCreate(pAgg->pDeepCopy(true), nullptr);
      pProjectedColList->vAppend(pNewCol);
    }
  }

  delete pIter;
  pAggList->vRemoveAll();
  delete pAggList;

  return pProjectedColList;
}

/*----------------------------------------------------------------------------*/
/* Changes the aggregate expression in pNode to super-aggregate results from  */
/* each partition or node                                                     */
/*----------------------------------------------------------------------------*/
void ProjectedColumn::vSuperAggregate(QueryNode* pNode, PlanNode* pRootNode)
{
  // e.g. an expression of SUM(col1), when all of the partitioned results are
  // being merged, will be super-aggregated with the expression SUM(SUM(col1)).
  // Super-aggregation occurs as follows:
  //
  // SUM(x)   -->   SUM(SUM(x))
  // COUNT(x) -->   SUM(COUNT(x))
  // MIN(x)   -->   MIN(MIN(x))
  // MAX(x)   -->   MAX(MAX(x))
  //
  // Note that an AVG(x) aggregate will be replaced at the partition level by
  // a SUM(x) and COUNT(x). A super-aggregating expression replaces AVG(x) with
  // SUM(SUM(x))/SUM(COUNT(x))

  QueryNode* pExpr = pExprNode(pNode);

  Expr::vSuperAggregate(pExpr, pRootNode);
  ASSERT(pExpr->szName()->bEqual(C_EXPR_PROP));
  ASSERT(Expr::bIsOfType(pExpr));
}

/*----------------------------------------------------------------------------*/
/* Changes the aggregate expression in pNode to super-aggregate results from  */
/* each partition or node. This version super-aggregates twice to factor in   */
/* node-level aggregation.                                                    */
/*----------------------------------------------------------------------------*/
void ProjectedColumn::vDoubleSuperAggregate(QueryNode* pNode, PlanNode* pRootNode)
{
  // e.g. an expression of SUM(col1), when all of the partitioned results are
  // being merged, will be super-aggregated with the expression SUM(SUM(col1)).
  // Super-aggregation occurs as follows:
  //
  // SUM(x)   -->   SUM(SUM(x))
  // COUNT(x) -->   SUM(COUNT(x))
  // MIN(x)   -->   MIN(MIN(x))
  // MAX(x)   -->   MAX(MAX(x))
  //
  // Note that an AVG(x) aggregate will be replaced at the partition level by
  // a SUM(x) and COUNT(x). A super-aggregating expression replaces AVG(x) with
  //   SUM(SUM(x))/SUM(COUNT(x))
  // A double-super-aggregation replaces AVG(x) with:
  //   SUM(SUM(SUM(x)))/SUM(SUM(COUNT(x)))

  QueryNode* pExpr = pExprNode(pNode);

  Expr::vSuperAggregate(pExpr, pRootNode);
  Expr::vSuperAggregate(pExpr, pRootNode);
  ASSERT(pExpr->szName()->bEqual(C_EXPR_PROP));
  ASSERT(Expr::bIsOfType(pExpr));
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
