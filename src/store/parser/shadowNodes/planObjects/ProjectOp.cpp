/*============================================================================*/
/* ProjectOp.hpp - operator that changes the defn of rows that it processes   */
/*============================================================================*/

#include "BASE.h"
#include "ProjectOp.hpp"

#include "PlanConstants.hpp"
#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "store/parser/shadowNodes/compound/ProjectedColumn.hpp"
#include "Expr.hpp"

#include "GraphNav.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_PROJECTOP_DEFN = "/__root__/definitions/object/planObject/sqlOperator/projectOp"_Stz;

Stz* ProjectOp::C_PROJECTEDEXPRS_PROP = "projectedExprs"_Stz;
Stz* ProjectOp::C_PROJECTEDEXPRS_DEFN = "/__root__/definitions/object/list/projectedExprs"_Stz;



/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Helper function to create ProjectOp nodes.                                 */
/*----------------------------------------------------------------------------*/
QueryNode* ProjectOp::pCreate(QueryNodeList* pSelectItems)
{
  ASSERT(pSelectItems);
  ASSERT(pSelectItems->nItems() > 0);

  // Create the projectedExprs property of the ProjectOp
  QueryNode* pProjectedExprs = new QueryNode(C_PROJECTEDEXPRS_PROP, const_cast<Stz*>(C_PROJECTEDEXPRS_DEFN));

  QueryNode* pItem;
  while ((pItem = pSelectItems->pPop()) != nullptr) {
    pProjectedExprs->vAppendChild(pItem);
  }

  // Create a unique name for this ProjectOp object
  s256 sName;
  ShadowNode::vObjName(GraphNav::szLeafType(C_PROJECTOP_DEFN), sName);

  // Create the object and add  its projectedExprs property
  QueryNode* pProjectOp = new QueryNode(sName, C_PROJECTOP_DEFN);
  pProjectOp->vAppendProperty(pProjectedExprs);

  return pProjectOp;
}

/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void ProjectOp::vDescribe(PlanNode* pNode, s2K& sBuf)
{
  _static_
  ASSERT(bIsOfType(pNode));

  QueryNode* pProjectOp = static_cast<QueryNode*>(pNode);

  sBuf.vConcat("ProjectOp :"_Stz);

  QueryNode* pProjectedExprs = static_cast<QueryNode*>(GraphNav::pFindProperty(C_PROJECTEDEXPRS_PROP, pProjectOp));
  ASSERT(pProjectedExprs);

  if (pProjectedExprs->bHasChildren()) {
    PlanNodeIter* pIter = pProjectedExprs->pNewChildrenIter();

    while (pIter->bHasNext()) {
      QueryNode* pProjectedColumn = static_cast<QueryNode*>(pIter->pNext());
      ASSERT(ProjectedColumn::bIsOfType(pProjectedColumn));

      sBuf.vConcat("\n  "_Stz);
      ProjectedColumn::vDescribe(pProjectedColumn, sBuf);
    }

    delete pIter;
  }
}


/*----------------------------------------------------------------------------*/
/* Recurses down the query plan and writes the explain of the stmt to the buf */
/*----------------------------------------------------------------------------*/
void ProjectOp::vExplain(PlanNode *pNode, PlanNode *pRootNode, s48K &sBuf, i8 iIndent)
{
  _static_
  ASSERT(bIsOfType(pNode));

  sBuf.vConcat("\n\n"_Stz);
  vAppendIndent(sBuf, iIndent, eIndentFormat::IF_ARROW);
  sBuf.vConcat(GraphNav::szLeafType(pNode->szType()));

  // Get the list of projected expressions
  PlanNode* pProjExprs = GraphNav::pFindProperty(const_cast<Stz*>(C_PROJECTEDEXPRS_PROP), pNode);
  ASSERT(pProjExprs);

  ASSERT(pNode->nChildren() == 1);
  ASSERT(PlanObject::bIsOfType(pNode->pFirstChild()));
  PlanNodeList* pColExprList = PlanObject::pColumnExprs(static_cast<QueryNode*>(pNode->pFirstChild()), pRootNode);

  // Iterate each projected expression
  if (pProjExprs->bHasChildren()) {
    PlanNodeIter* pIter = pProjExprs->pNewChildrenIter();

    while (pIter->bHasNext()) {
      QueryNode* pProjExpr = static_cast<QueryNode*>(pIter->pNext());
      ASSERT(pProjExpr);
      ASSERT(ProjectedColumn::bIsOfType(pProjExpr));

      // Get the description of the projected expression
      s2K sColBuf;
      ProjectedColumn::vDescribe(pProjExpr, sColBuf, pColExprList, pRootNode);
      sBuf.vConcat("\n"_Stz);
      vAppendIndent(sBuf, iIndent, IF_PLAIN);
      sBuf.vCatSprint("column: (%s)"_Stz, sColBuf.szStz());
    }

    delete pIter;
  }

  delete pColExprList;

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
b1 ProjectOp::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_PROJECTOP_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely same as its definition         */
/*----------------------------------------------------------------------------*/
b1 ProjectOp::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_PROJECTOP_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if any of the projected expressions are aggregate expressions */
/*----------------------------------------------------------------------------*/
b1 ProjectOp::bHasAnAggregate(QueryNode* pNode)
{
  QueryNode* pProjExprs =
      static_cast<QueryNode*>(GraphNav::pFindProperty(const_cast<Stz *>(C_PROJECTEDEXPRS_PROP), pNode));

  b1 bResult = false;

  if (pProjExprs->nChildren() == 0) {
    return bResult;
  }

  // Iterate all of the projected columns
  PlanNodeIter* pIter = pProjExprs->pNewChildrenIter();

  while (pIter->bHasNext()) {
    QueryNode* pProjExpr = static_cast<QueryNode*>(pIter->pNext());

    ASSERT(ProjectedColumn::bIsOfType(pProjExpr));
    if (ProjectedColumn::bContainsAnAggregate(pProjExpr)) {
      bResult = true;
      break;
    }
  }

  delete pIter;
  return bResult;
}

/*----------------------------------------------------------------------------*/
/* Returns the projectedExprs node of the projectOp                           */
/*----------------------------------------------------------------------------*/
QueryNode* ProjectOp::pProjectedExprs(QueryNode* pNode)
{
  QueryNode* pProjExprs =
      static_cast<QueryNode*>(GraphNav::pFindProperty(const_cast<Stz *>(C_PROJECTEDEXPRS_PROP), pNode));

  return pProjExprs;
}

/*----------------------------------------------------------------------------*/
/* Returns a new copy of the projected expressions rewritten relative to      */
/* column ordinals. Must be freed after use.                                  */
/*----------------------------------------------------------------------------*/
QueryNode* ProjectOp::pOrdinalProjectedExprs(QueryNode* pNode, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));

  // Get the list of column expressions being given to this plan from its child.
  // This is the list of expression symbols that represent each column of
  // each row that this sql operator will receive.
  ASSERT(pNode->nChildren() == 1);
  PlanNodeList* pColExprList = PlanObject::pColumnExprs(static_cast<QueryNode*>(pNode->pFirstChild()), pRootNode);
  ASSERT(pColExprList);
  ASSERT(pColExprList->nItems() > 0);

  // Create a container node, but without any children
  QueryNode* pProjectedExprs = pNode->pDeepCopy(new PlanNodeList());

  // Iterate over each of the ProjectedColumn children of pNode
  PlanNodeIter* pIter = pNode->pNewChildrenIter();

  while (pIter->bHasNext()) {
    QueryNode* pProjectedColumn = static_cast<QueryNode*>(pIter->pNext());
    ASSERT(ProjectedColumn::bIsOfType(pProjectedColumn));

    // Convert the ProjectedColumn from a symbolic representation to an ordinal representation
    QueryNode* pOrdinalProjectedColumn = ProjectedColumn::pSymbolicToOrdinal(pProjectedColumn, pColExprList, pRootNode);
    pProjectedExprs->vAppendChild(pOrdinalProjectedColumn);
  }

  delete pIter;

  pColExprList->vDeleteAll();
  delete pColExprList;

  return pProjectedExprs;
}

/*----------------------------------------------------------------------------*/
/* Return a list of ProjectedExprs that represent the column expressions      */
/* that this PlanObject will return. All nodes and the list must be freed.    */
/*----------------------------------------------------------------------------*/
PlanNodeList* ProjectOp::pColumnExprs(QueryNode* pNode, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));
  ASSERT(pRootNode);

  // Create a list for the columnRefs to be returned
  PlanNodeList* pColumnRefList = new PlanNodeList();

  // The list we return and all of its elements need to be deletable.
  // So we need to duplicate each projected expression before adding it
  // to the list.
  QueryNode* pProjExprs = pProjectedExprs(pNode);

  PlanNodeIter* pIter = pProjExprs->pNewChildrenIter();
  while (pIter->bHasNext()) {

    QueryNode* pProjExpr = static_cast<QueryNode*>(pIter->pNext());
    ASSERT(ProjectedColumn::bIsOfType(pProjExpr));

    QueryNode* pDup = pProjExpr->pDeepCopy();
    pColumnRefList->vAppend(pDup);
  }

  delete pIter;
  return pColumnRefList;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
