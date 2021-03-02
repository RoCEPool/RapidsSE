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
void AggregateOp::vExplain(PlanNode* pNode, s48K& sBuf, i8 iIndent)
{
  _static_
  ASSERT(bIsOfType(pNode));

  sBuf.vConcat("\n\n"_Stz);
  vAppendIndent(sBuf, iIndent, eIndentFormat::IF_ARROW);
  sBuf.vConcat(GraphNav::szLeafType(pNode->szType()));

  // Get the list of Projected expressions
  PlanNode* pProjExprs = GraphNav::pFindProperty(const_cast<Stz*>(C_PROJECTEDEXPRS_PROP), pNode);
  ASSERT(pProjExprs);

  // Iterate each Projected expression
  if (pProjExprs->bHasChildren()) {
    PlanNodeIter* pIter = pProjExprs->pNewChildrenIter();

    while (pIter->bHasNext()) {
      QueryNode* pProjExpr = static_cast<QueryNode*>(pIter->pNext());
      ASSERT(pProjExpr);
      ASSERT(ProjectedExpr::bIsOfType(pProjExpr));

      // Get the description of the Projected expression
      s2K sColBuf;
      ProjectedExpr::vDescribe(pProjExpr, sColBuf);
      sBuf.vConcat("\n"_Stz);
      vAppendIndent(sBuf, iIndent, IF_PLAIN);
      sBuf.vCatSprint("column: (%s)"_Stz, sColBuf.szStz());
    }

    delete pIter;
  }

  // Get the list of Projected expressions
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


  // Recurse any children of pNode
  if (pNode->bHasChildren()) {
    PlanNodeIter* pIter = pNode->pNewChildrenIter();

    while (pIter->bHasNext()) {
      PlanNode* pChild = pIter->pNext();
      ASSERT(PlanObject::bIsOfType(pChild));

      PlanObject::vExplain(pChild, sBuf, iIndent+1);
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
  Parser::PlanNode *pGroupByExprs =
      static_cast<QueryNode*>(GraphNav::pFindProperty(const_cast<Stz *>(C_GROUPBY_PROP), pNode));

  return pGroupByExprs->nChildren();
}

/*----------------------------------------------------------------------------*/
/* Return an iterator to the groupby expression list. Must be freed afterwards*/
/*----------------------------------------------------------------------------*/
PlanNodeIter* AggregateOp::pGroupByIter(QueryNode* pNode)
{
  Parser::PlanNode *pGroupByExprs =
      static_cast<QueryNode*>(GraphNav::pFindProperty(const_cast<Stz *>(C_GROUPBY_PROP), pNode));

  if (pGroupByExprs->nChildren() == 0) {
    return nullptr;
  }

  return pGroupByExprs->pNewChildrenIter();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
