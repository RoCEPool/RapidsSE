/*============================================================================*/
/* AggregateOp.hpp - operator that extends ProjectOp to add a groupBy list    */
/*============================================================================*/

#include "BASE.h"
#include "AggregateOp.hpp"

#include "PlanConstants.hpp"
#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "ProjectedColumn.hpp"
#include "Expr.hpp"
#include "AliasRef.hpp"
#include "ShadowRoot.hpp"
#include "MetadataNode.hpp"

#include "GraphNav.hpp"
#include "NodeNamer.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_AGGREGATEOP_DEFN = "/__root__/definitions/object/planObject/sqlOperator/projectOp/aggregateOp"_Stz;

static Stz* C_GROUPBY_PROP = "groupBy"_Stz;
static Stz* C_GROUPBY_DEFN = "/__root__/definitions/object/list/exprList"_Stz;



/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Helper function to create AggregateOp nodes.                                 */
/*----------------------------------------------------------------------------*/
QueryNode* AggregateOp::pCreate(QueryNodeList* pSelectExprs, QueryNodeList* pGroupByExprs)
{
  // Start by creating a project op. Then we will modify it to suit.
  QueryNode* pAggregateOp = ProjectOp::pCreate(pSelectExprs);

  // Create a unique name for this AggregateOp object
  s256 sName;
  NodeNamer::vGenerateName(C_AGGREGATEOP_DEFN, sName);
  pAggregateOp->vSetName(sName.szStz());

  pAggregateOp->vSetType(C_AGGREGATEOP_DEFN);


  ASSERT(pGroupByExprs);
  ASSERT(pGroupByExprs->nItems() > 0);

  // Create the groupBy property of the AggregateOp and append it
  QueryNode* pGroupByNode = new QueryNode(C_GROUPBY_PROP, C_GROUPBY_DEFN);

  QueryNode* pItem;
  while ((pItem = pGroupByExprs->pPop()) != nullptr) {
    pGroupByNode->vAppendChild(pItem);
  }

  pAggregateOp->vAppendProperty(pGroupByNode);

  return pAggregateOp;
}

/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void AggregateOp::vDescribe(PlanNode* pNode, s2K& sBuf)
{
  _static_
  ASSERT(bIsOfType(pNode));

  QueryNode* pAggregateOp = _pQueryNode(pNode);

  sBuf.vConcat("AggregateOp : "_Stz);

  QueryNode* pProjectedColumns = _pQueryNode(GraphNav::pFindProperty(C_PROJECTEDCOLUMNS_PROP, pAggregateOp));
  ASSERT(pProjectedColumns);

  if (pProjectedColumns->bHasChildren()) {
    sBuf.vConcat("project { "_Stz);
    b1 bFirst = true;
    PlanNodeIter* pIter = pProjectedColumns->pNewChildrenIter();

    while (pIter->bHasNext()) {
      QueryNode* pProjectedColumn = _pQueryNode(pIter->pNext());
      ASSERT(ProjectedColumn::bIsOfType(pProjectedColumn));

      if (bFirst) {
        bFirst = false;
      } else {
        sBuf.vConcat(", "_Stz);
      }

      ProjectedColumn::vDescribe(pProjectedColumn, sBuf);
    }

    delete pIter;
    sBuf.vConcat("}"_Stz);
  }

  QueryNode* pGroupByExprs = _pQueryNode(GraphNav::pFindProperty(C_GROUPBY_PROP, pAggregateOp));
  ASSERT(pGroupByExprs);

  if (pGroupByExprs->bHasChildren()) {
    sBuf.vConcat(", group by { "_Stz);
    b1 bFirst = true;
    PlanNodeIter* pIter = pGroupByExprs->pNewChildrenIter();

    while (pIter->bHasNext()) {
      QueryNode* pExpr = _pQueryNode(pIter->pNext());
      ASSERT(Expr::bIsOfType(pExpr));

      if (bFirst) {
        bFirst = false;
      } else {
        sBuf.vConcat(", "_Stz);
      }

      Expr::vDescribe(pExpr, sBuf);
    }

    delete pIter;
    sBuf.vConcat("}"_Stz);
  }
}


/*----------------------------------------------------------------------------*/
/* Recurses down the query plan and writes the explain of the stmt to the buf */
/*----------------------------------------------------------------------------*/
void AggregateOp::vExplain(PlanNode *pNode, PlanNode *pRootNode, QueryNode* pExplainNode, s48K &sBuf, i8 iIndent)
{
  _static_
  ASSERT(bIsOfType(pNode));

  // Add the object header and local properties
  vExplainCommon(pNode, pRootNode, pExplainNode, pNode->szType(), sBuf, iIndent);

  // Recurse any children of pNode
  if (pNode->bHasChildren()) {
    PlanNodeIter* pIter = pNode->pNewChildrenIter();

    while (pIter->bHasNext()) {
      PlanNode* pChild = pIter->pNext();
      ASSERT(PlanObject::bIsOfType(pChild));

      PlanObject::vExplain(pChild, pRootNode, pExplainNode, sBuf, iIndent + 1);
    }
    delete pIter;
  }
}

/*----------------------------------------------------------------------------*/
/* Recurses down the query plan and writes the explain of the stmt to the buf */
/*----------------------------------------------------------------------------*/
void AggregateOp::vExplainCommon(PlanNode *pNode, PlanNode* pRootNode, QueryNode* pExplainNode, Stz* szType, s48K &sBuf, i8 iIndent)
{
  _static_
  ASSERT(bIsOfType(pNode));

  if (bIsExactType(szType)) {
    // Explain any common attributes of this object
    // Get the parent type and keep recursing back
    s2K sParentType;
    GraphNav::vParentType(szType, sParentType);
    PlanObject::vExplainCommon(pNode, pRootNode, pExplainNode, sParentType, sBuf, iIndent);

    // Explain any local attributes of this object

    // Get the list of symbolic groupby expressions
    PlanNode* pGroupByExprs = GraphNav::pFindProperty(const_cast<Stz*>(C_GROUPBY_PROP), pNode);
    ASSERT(pGroupByExprs);

    // Iterate each groupby expression
    if (pGroupByExprs->bHasChildren()) {
      PlanNodeIter* pIter = pGroupByExprs->pNewChildrenIter();

      while (pIter->bHasNext()) {
        QueryNode* pGroupByExpr = _pQueryNode(pIter->pNext());
        ASSERT(pGroupByExpr);
        ASSERT(Expr::bIsOfType(pGroupByExpr));

        // Get the description of the group by expression
        s2K sExprBuf;
        Expr::vDescribe(pGroupByExpr, sExprBuf);
        sBuf.vConcat("\n"_Stz);
        vAppendIndent(sBuf, iIndent, IF_PLAIN);
        sBuf.vCatSprint("group by: (%s)"_Stz, sExprBuf.szStz());
      }

      delete pIter;
    }

    // Get the list of *ordinal* groupby expressions.
    // Again this is skipped if the plan is very early in the compilation pipeline
    // and not all names have been qualified.
    if (MetadataNode::bHasMetadata(ShadowRoot::pMetadataNode(pRootNode))) {
      PlanNode *pOrdinalGroupByExprs = pOrdinalGroupByList(_pQueryNode(pNode), pRootNode);
      ASSERT(pOrdinalGroupByExprs);

      // Iterate each ordinal groupby expression
      if (pOrdinalGroupByExprs->bHasChildren()) {
        PlanNodeIter *pIter = pOrdinalGroupByExprs->pNewChildrenIter();

        while (pIter->bHasNext()) {
          QueryNode *pOrdGroupByExpr = _pQueryNode(pIter->pNext());
          ASSERT(pOrdGroupByExpr);
          ASSERT(Expr::bIsOfType(pOrdGroupByExpr));

          // Get the description of the group by expression
          s2K sExprBuf;
          Expr::vDescribe(pOrdGroupByExpr, sExprBuf);
          sBuf.vCatSprint(" ; Ordinal Exprs = (%s)"_Stz, sExprBuf.szStz());
        }

        delete pIter;
      }

      delete pOrdinalGroupByExprs;
    }

    return;
  }

  // Manual polymorphism here

}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the statement definition    */
/*----------------------------------------------------------------------------*/
b1 AggregateOp::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_AGGREGATEOP_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given type str is derived from the objects definition  */
/*----------------------------------------------------------------------------*/
b1 AggregateOp::bIsOfType(Stz* szType)
{
  return ShadowNode::bIsOfType(szType, C_AGGREGATEOP_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely same as its definition         */
/*----------------------------------------------------------------------------*/
b1 AggregateOp::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_AGGREGATEOP_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 AggregateOp::bIsExactType(Stz* szType)
{
  return ShadowNode::bIsExactType(szType, C_AGGREGATEOP_DEFN);
}

/*----------------------------------------------------------------------------*/
/*  Returns the number of groupby expressions                                 */
/*----------------------------------------------------------------------------*/
i8 AggregateOp::nGroups(QueryNode* pNode)
{
  ASSERT(bIsOfType(pNode));

  QueryNode *pGroupByExprs =
      _pQueryNode(GraphNav::pFindProperty(const_cast<Stz *>(C_GROUPBY_PROP), pNode));

  return pGroupByExprs->nChildren();
}

/*----------------------------------------------------------------------------*/
/* Returns the internal node that represents a list of groupby expressions    */
/*----------------------------------------------------------------------------*/
QueryNode* AggregateOp::pGroupByList(QueryNode* pNode)
{
  ASSERT(bIsOfType(pNode));

  QueryNode *pGroupByExprs =
      _pQueryNode(GraphNav::pFindProperty(const_cast<Stz *>(C_GROUPBY_PROP), pNode));

  return pGroupByExprs;
}

/*----------------------------------------------------------------------------*/
/* Returns a copy of the node that represents a list of groupby expressions.  */
/* Each groupby expression can be iterated by calling pNewChildrenIter() on   */
/* the returned node. Must be freed after use.                                */
/*----------------------------------------------------------------------------*/
QueryNode* AggregateOp::pOrdinalGroupByList(QueryNode* pNode, PlanNode* pRootNode)
{
  ASSERT(bIsOfType(pNode));

  // Get the symbolic list of groupby expressions
  QueryNode *pSymbolicExprs = pGroupByList(pNode);


  // Get the list of column expressions being given to this plan from its child.
  // This is the list of expression symbols that represent each column of
  // each row that this sql operator will receive.
  ASSERT(pNode->nChildren() == 1);
  QueryNodeList* pColExprList = PlanObject::pColumnExprs(_pQueryNode(pNode->pFirstChild()), pRootNode);
  ASSERT(pColExprList);
  ASSERT(pColExprList->nItems() > 0);

  // Get the list of correlated expressions from the enclosing subquery.
  // We don't need to free this object.
  QueryNode* pCorrelatedExprs = PlanObject::pCorrelatedExprs(pNode, pRootNode);

  // Create a container node to hold the ordinal expressions, but without any children
  QueryNode* pOrdinalExprs = pSymbolicExprs->pDeepCopy(true, new PlanNodeList());

  // Iterate over each of the expression children of pSymbolicExprs
  PlanNodeIter* pIter = pSymbolicExprs->pNewChildrenIter();

  while (pIter->bHasNext()) {
    QueryNode* pSymExpr = _pQueryNode(pIter->pNext());
    ASSERT(Expr::bIsOfType(pSymExpr));

    // Convert pSymExpr from a symbolic representation to an ordinal representation
    QueryNode* pOrdExpr = Expr::pSymbolicToOrdinal(pSymExpr, pColExprList, pCorrelatedExprs, pRootNode);
    pOrdinalExprs->vAppendChild(pOrdExpr);
  }

  delete pIter;

  pColExprList->vDeleteAll();
  delete pColExprList;

  return pOrdinalExprs;
}

/*----------------------------------------------------------------------------*/
/* Returns a new copy of the projected expressions rewritten in ordinal       */
/* orientation (where possible) where the ordinals are relative to group by   */
/* list. Not all columns will be expressed relative to the group by list and  */
/* so they will retain use of ColumnRefs.                                     */
/* Must be freed after use.                                                   */
/*----------------------------------------------------------------------------*/
QueryNode* AggregateOp::pOrdinalProjectedColumnsRelativeToGroupBy(QueryNode* pNode, PlanNode* pRootNode)
{
  _static_
  ASSERT(pNode);
  ASSERT(bIsOfType(pNode));

  // Make a QueryNodeList that contains ProjectedColumns of the expressions
  // in the group by clause.
  QueryNodeList* pColExprList = new QueryNodeList();

  QueryNode* pGroupByNode = pGroupByList(pNode);
  PlanNodeIter* pGroupByIter = pGroupByNode->pNewChildrenIter();

  while (pGroupByIter->bHasNext()) {
    QueryNode* pExpr = _pQueryNode(pGroupByIter->pNext());
    ASSERT(Expr::bIsOfType(pExpr));

    QueryNode* pGroupByProjectedColumn = ProjectedColumn::pCreate(pExpr->pDeepCopy(false), nullptr);
    pColExprList->vAppend(pGroupByProjectedColumn);
  }
  delete pGroupByIter;

  ASSERT(pColExprList);
  ASSERT(pColExprList->nItems() > 0);

  // Get the list of correlated expressions from the enclosing subquery.
  // We don't need to free this object.
  QueryNode* pCorrelatedExprs = PlanObject::pCorrelatedExprs(pNode, pRootNode);

  QueryNode* pProjectedColumns = ProjectOp::pProjectedColumns(pNode);

  // Create a container node, but without any children
  QueryNode* pOrdinalProjectedColumns = ProjectOp::pProjectedColumns(pNode)->pDeepCopy(true, new PlanNodeList());

  // Iterate over each of the ProjectedColumn children of pNode
  PlanNodeIter* pIter = pProjectedColumns->pNewChildrenIter();

  while (pIter->bHasNext()) {
    QueryNode* pProjectedColumn = _pQueryNode(pIter->pNext());
    ASSERT(ProjectedColumn::bIsOfType(pProjectedColumn));

    // Convert the ProjectedColumn from a symbolic representation to an ordinal representation
    QueryNode* pOrdinalProjectedColumn = ProjectedColumn::pSymbolicToOrdinal(pProjectedColumn, pColExprList, pCorrelatedExprs, pRootNode);
    pOrdinalProjectedColumns->vAppendChild(pOrdinalProjectedColumn);
  }

  delete pIter;

  pColExprList->vDeleteAll();
  delete pColExprList;

  return pOrdinalProjectedColumns;
}

/*----------------------------------------------------------------------------*/
/* Merges a ProjectOp, pOtherOp, down into this AggregateOp                   */
/*----------------------------------------------------------------------------*/
void AggregateOp::vSquash(QueryNode* pNode, QueryNode* pOtherOp, PlanNode* pRootNode)
{
  _static_
  ASSERT(pNode);
  ASSERT(bIsOfType(pNode));
  ASSERT(pOtherOp);
  ASSERT(ProjectOp::bIsExactType(pOtherOp));
  ASSERT(pOtherOp->nChildren() == 0);
  ASSERT(pRootNode);

  // Squash the project list
  ProjectOp::vSquash(pNode, pOtherOp, pRootNode);

  // Now we need to consider the GROUP BY list.
  // Expressions in the group by list may contain aliasRefs whose alias is now no
  // longer defined in the project list (because they got squashed out). In that
  // case we need to expand these aliasRefs in the group by list so that it all
  // still makes sense.

  QueryNode* pGroupByList = AggregateOp::pGroupByList(pNode);
  ASSERT(pGroupByList);
  ASSERT(pGroupByList->nChildren() > 0);

  QueryNode* pProjectedColumns = ProjectOp::pProjectedColumns(pNode);
  ASSERT(pProjectedColumns);
  ASSERT(pProjectedColumns->nChildren() > 0);

  // Iterate the group by list and look for any AliasRefs
  PlanNodeIter* pGroupByIter = pGroupByList->pNewChildrenIter();
  PlanNodeIter* pProjectedColsIter = pProjectedColumns->pNewChildrenIter();

  while (pGroupByIter->bHasNext()) {
    QueryNode* pGroupByExpr = _pQueryNode(pGroupByIter->pNext());

    ASSERT(Expr::bIsOfType(pGroupByExpr));

    // Normally a select list expr is equal to or a superset of a group by expr.
    // i.e. a group by expr is equal to or a subset of a select list expr.
    // But here we are only interested if a group by expr was using an alias
    // from the select list that now no longer exists. If that was the case then
    // the group by column could not have been a subset of a select list expr, it
    // must have been equal. So we can ultimately check if the group by expr
    // is an AliasRef, as opposed to checking if the group by expr *contains*
    // an AliasRef.

    if (!AliasRef::bIsOfType(pGroupByExpr)) {
      continue;
    }

    Stz* szGroupByAlias = AliasRef::szAlias(pGroupByExpr);

    // Does this aliased expr still exist in the new project list?
    // (Remember we updated this at the top of the function)
    pProjectedColsIter->vBeforeFirst();
    b1 bFoundAlias = false;

    while (pProjectedColsIter->bHasNext()) {
      QueryNode* pProjectedCol = _pQueryNode(pProjectedColsIter->pNext());
      QueryNode* pProjectedExpr = ProjectedColumn::pExprNode(pProjectedCol);
      Stz* szProjectedAlias = ProjectedColumn::szAliasName(pProjectedCol);  // may be null

      // We can keep the group by alias if the group by expression is in the new
      // project list, the projected column still has an alias and the alias
      // name matches the alias of the group by expression.
      if (Expr::bIsSymbolicallyEqual(pProjectedExpr, pGroupByExpr) &&
          szProjectedAlias &&
          szProjectedAlias->bSame(szGroupByAlias)) {
        bFoundAlias = true;
        break;
      }
    }

    if (!bFoundAlias) {
      // We need to change the AliasRef to its expanded form
      AliasRef::vExpandAlias(pGroupByExpr);
    }

  }
  delete pGroupByIter;
  delete pProjectedColsIter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
