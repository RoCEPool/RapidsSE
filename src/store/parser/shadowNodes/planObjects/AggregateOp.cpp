/*============================================================================*/
/* AggregateOp.hpp - operator that extends ProjectOp to add a groupBy list    */
/*============================================================================*/

#include "BASE.h"
#include "AggregateOp.hpp"

#include "PlanConstants.hpp"
#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "ProjectedExpr.hpp"
#include "Expr.hpp"

#include "GraphNav.hpp"

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
  ShadowNode::vObjName(GraphNav::szLeafType(C_AGGREGATEOP_DEFN), sName);
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

  QueryNode* pAggregateOp = static_cast<QueryNode*>(pNode);

  sBuf.vConcat("AggregateOp : "_Stz);

  QueryNode* pProjectedExprs = static_cast<QueryNode*>(GraphNav::pFindProperty(C_PROJECTEDEXPRS_PROP, pAggregateOp));
  ASSERT(pProjectedExprs);

  if (pProjectedExprs->bHasChildren()) {
    sBuf.vConcat("project { "_Stz);
    b1 bFirst = true;
    PlanNodeIter* pIter = pProjectedExprs->pNewChildrenIter();

    while (pIter->bHasNext()) {
      QueryNode* pProjectedExpr = static_cast<QueryNode*>(pIter->pNext());
      ASSERT(ProjectedExpr::bIsOfType(pProjectedExpr));

      if (bFirst) {
        bFirst = false;
      } else {
        sBuf.vConcat(", "_Stz);
      }

      ProjectedExpr::vDescribe(pProjectedExpr, sBuf);
    }

    delete pIter;
    sBuf.vConcat("}"_Stz);
  }

  QueryNode* pGroupByExprs = static_cast<QueryNode*>(GraphNav::pFindProperty(C_GROUPBY_PROP, pAggregateOp));
  ASSERT(pGroupByExprs);

  if (pGroupByExprs->bHasChildren()) {
    sBuf.vConcat(", group by { "_Stz);
    b1 bFirst = true;
    PlanNodeIter* pIter = pGroupByExprs->pNewChildrenIter();

    while (pIter->bHasNext()) {
      QueryNode* pExpr = static_cast<QueryNode*>(pIter->pNext());
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
void AggregateOp::vExplain(PlanNode *pNode, PlanNode *pRootNode, s48K &sBuf, i8 iIndent)
{
  _static_
  ASSERT(bIsOfType(pNode));

  sBuf.vConcat("\n\n"_Stz);
  vAppendIndent(sBuf, iIndent, eIndentFormat::IF_ARROW);
  sBuf.vConcat(GraphNav::szLeafType(pNode->szType()));

  // Get the list of Projected expressions
  PlanNode* pProjExprs = GraphNav::pFindProperty(const_cast<Stz*>(C_PROJECTEDEXPRS_PROP), pNode);
  ASSERT(pProjExprs);

  ASSERT(pNode->nChildren() == 1);
  ASSERT(PlanObject::bIsOfType(pNode->pFirstChild()));
  PlanNodeList* pColExprList = PlanObject::pColumnExprs(static_cast<QueryNode*>(pNode->pFirstChild()), pRootNode);

  // Iterate each Projected expression
  if (pProjExprs->bHasChildren()) {
    PlanNodeIter* pIter = pProjExprs->pNewChildrenIter();

    while (pIter->bHasNext()) {
      QueryNode* pProjExpr = static_cast<QueryNode*>(pIter->pNext());
      ASSERT(pProjExpr);
      ASSERT(ProjectedExpr::bIsOfType(pProjExpr));

      // Get the description of the Projected expression
      s2K sColBuf;
      ProjectedExpr::vDescribe(pProjExpr, sColBuf, pColExprList, pRootNode);
      sBuf.vConcat("\n"_Stz);
      vAppendIndent(sBuf, iIndent, IF_PLAIN);
      sBuf.vCatSprint("column: (%s)"_Stz, sColBuf.szStz());
    }

    delete pIter;
  }

  delete pColExprList;


  // Get the list of symbolic groupby expressions
  PlanNode* pGroupByExprs = GraphNav::pFindProperty(const_cast<Stz*>(C_GROUPBY_PROP), pNode);
  ASSERT(pGroupByExprs);

  // Iterate each groupby expression
  if (pGroupByExprs->bHasChildren()) {
    PlanNodeIter* pIter = pGroupByExprs->pNewChildrenIter();

    while (pIter->bHasNext()) {
      QueryNode* pGroupByExpr = static_cast<QueryNode*>(pIter->pNext());
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

  // Get the list of *ordinal* groupby expressions
  PlanNode* pOrdinalGroupByExprs = pOrdinalGroupByList(static_cast<QueryNode*>(pNode), pRootNode);
  ASSERT(pOrdinalGroupByExprs);

  // Iterate each ordinal groupby expression
  if (pOrdinalGroupByExprs->bHasChildren()) {
    PlanNodeIter* pIter = pOrdinalGroupByExprs->pNewChildrenIter();

    while (pIter->bHasNext()) {
      QueryNode* pOrdGroupByExpr = static_cast<QueryNode*>(pIter->pNext());
      ASSERT(pOrdGroupByExpr);
      ASSERT(Expr::bIsOfType(pOrdGroupByExpr));

      // Get the description of the group by expression
      s2K sExprBuf;
      Expr::vDescribe(pOrdGroupByExpr, sExprBuf);
      sBuf.vCatSprint(" ; Ordinal Exprs = (%s)"_Stz, sExprBuf.szStz());
    }

    delete pIter;
  }


  // Recurse any children of pNode
  if (pNode->bHasChildren()) {
    PlanNodeIter* pIter = pNode->pNewChildrenIter();

    while (pIter->bHasNext()) {
      PlanNode* pChild = pIter->pNext();
      ASSERT(PlanObject::bIsOfType(pChild));

      PlanObject::vExplain(pChild, pRootNode, sBuf, iIndent + 1);
    }
    delete pIter;
  }
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the statement definition    */
/*----------------------------------------------------------------------------*/
b1 AggregateOp::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_AGGREGATEOP_DEFN);
}

/*----------------------------------------------------------------------------*/
/*  Returns the number of groupby expressions                                 */
/*----------------------------------------------------------------------------*/
i8 AggregateOp::nGroups(QueryNode* pNode)
{
  ASSERT(bIsOfType(pNode));

  QueryNode *pGroupByExprs =
      static_cast<QueryNode*>(GraphNav::pFindProperty(const_cast<Stz *>(C_GROUPBY_PROP), pNode));

  return pGroupByExprs->nChildren();
}

/*----------------------------------------------------------------------------*/
/* Returns the internal node that represents a list of groupby expressions    */
/*----------------------------------------------------------------------------*/
QueryNode* AggregateOp::pGroupByList(QueryNode* pNode)
{
  ASSERT(bIsOfType(pNode));

  QueryNode *pGroupByExprs =
      static_cast<QueryNode*>(GraphNav::pFindProperty(const_cast<Stz *>(C_GROUPBY_PROP), pNode));

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
  PlanNodeList* pColExprList = PlanObject::pColumnExprs(static_cast<QueryNode*>(pNode->pFirstChild()), pRootNode);
  ASSERT(pColExprList);
  ASSERT(pColExprList->nItems() > 0);

  // Create a container node to hold the ordinal expressions, but without any children
  QueryNode* pOrdinalExprs = pSymbolicExprs->pDeepCopy(new PlanNodeList());

  // Iterate over each of the expression children of pSymbolicExprs
  PlanNodeIter* pIter = pSymbolicExprs->pNewChildrenIter();

  while (pIter->bHasNext()) {
    QueryNode* pSymExpr = static_cast<QueryNode*>(pIter->pNext());
    ASSERT(Expr::bIsOfType(pSymExpr));

    // Convert pSymExpr from a symbolic representation to an ordinal representation
    QueryNode* pOrdExpr = Expr::pSymbolicToOrdinal(pSymExpr, pColExprList, pRootNode);
    pOrdinalExprs->vAppendChild(pOrdExpr);
  }

  delete pIter;

  pColExprList->vDeleteAll();
  delete pColExprList;

  return pOrdinalExprs;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
