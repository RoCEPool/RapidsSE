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

Stz* ProjectOp::C_PROJECTEDCOLUMNS_PROP = "projectedColumns"_Stz;
Stz* ProjectOp::C_PROJECTEDCOLUMNS_DEFN = "/__root__/definitions/object/list/projectedColumns"_Stz;



/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Helper function to create ProjectOp nodes. Must be passed a list of        */
/* ProjectedColumn objects                                                    */
/*----------------------------------------------------------------------------*/
QueryNode* ProjectOp::pCreate(QueryNodeList* pSelectItems)
{
  ASSERT(pSelectItems);
  ASSERT(pSelectItems->nItems() > 0);

  // Create the projectedColumns property of the ProjectOp
  QueryNode* pProjectedColumns = new QueryNode(C_PROJECTEDCOLUMNS_PROP, const_cast<Stz*>(C_PROJECTEDCOLUMNS_DEFN));

  QueryNode* pItem;
  while ((pItem = pSelectItems->pPop()) != nullptr) {
    pProjectedColumns->vAppendChild(pItem);
  }

  // Create a unique name for this ProjectOp object
  s256 sName;
  ShadowNode::vObjName(GraphNav::szLeafType(C_PROJECTOP_DEFN), sName);

  // Create the object and add  its projectedColumns property
  QueryNode* pProjectOp = new QueryNode(sName, C_PROJECTOP_DEFN);
  pProjectOp->vAppendProperty(pProjectedColumns);

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

  QueryNode* pProjectedColumns = static_cast<QueryNode*>(GraphNav::pFindProperty(C_PROJECTEDCOLUMNS_PROP, pProjectOp));
  ASSERT(pProjectedColumns);

  if (pProjectedColumns->bHasChildren()) {
    PlanNodeIter* pIter = pProjectedColumns->pNewChildrenIter();

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
  PlanNode* pProjColumns = GraphNav::pFindProperty(const_cast<Stz*>(C_PROJECTEDCOLUMNS_PROP), pNode);
  ASSERT(pProjColumns);

  ASSERT(pNode->nChildren() == 1);
  ASSERT(PlanObject::bIsOfType(pNode->pFirstChild()));
  PlanNodeList* pColExprList = PlanObject::pColumnExprs(static_cast<QueryNode*>(pNode->pFirstChild()), pRootNode);

  // Iterate each projected expression
  if (pProjColumns->bHasChildren()) {
    PlanNodeIter* pIter = pProjColumns->pNewChildrenIter();

    while (pIter->bHasNext()) {
      QueryNode* pProjColumn = static_cast<QueryNode*>(pIter->pNext());
      ASSERT(pProjColumn);
      ASSERT(ProjectedColumn::bIsOfType(pProjColumn));

      // Get the description of the projected expression
      s2K sColBuf;
      ProjectedColumn::vDescribe(pProjColumn, sColBuf, pColExprList, pRootNode);
      sBuf.vConcat("\n"_Stz);
      vAppendIndent(sBuf, iIndent, IF_PLAIN);
      sBuf.vCatSprint("column: (%s)"_Stz, sColBuf.szStz());
    }

    delete pIter;
  }

  pColExprList->vDeleteAll();
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
  QueryNode* pProjColumns =
      static_cast<QueryNode*>(GraphNav::pFindProperty(const_cast<Stz *>(C_PROJECTEDCOLUMNS_PROP), pNode));

  b1 bResult = false;

  if (pProjColumns->nChildren() == 0) {
    return bResult;
  }

  // Iterate all of the projected columns
  PlanNodeIter* pIter = pProjColumns->pNewChildrenIter();

  while (pIter->bHasNext()) {
    QueryNode* pProjColumn = static_cast<QueryNode*>(pIter->pNext());

    ASSERT(ProjectedColumn::bIsOfType(pProjColumn));
    if (ProjectedColumn::bContainsAnAggregate(pProjColumn)) {
      bResult = true;
      break;
    }
  }

  delete pIter;
  return bResult;
}

/*----------------------------------------------------------------------------*/
/* Returns the projectedColumns node of the projectOp                           */
/*----------------------------------------------------------------------------*/
QueryNode* ProjectOp::pProjectedColumns(QueryNode* pNode)
{
  QueryNode* pProjColumns =
      static_cast<QueryNode*>(GraphNav::pFindProperty(const_cast<Stz *>(C_PROJECTEDCOLUMNS_PROP), pNode));

  return pProjColumns;
}

/*----------------------------------------------------------------------------*/
/* Returns a new copy of the projected expressions rewritten relative to      */
/* column ordinals. Must be freed after use.                                  */
/*----------------------------------------------------------------------------*/
QueryNode* ProjectOp::pOrdinalProjectedColumns(QueryNode* pNode, PlanNode* pRootNode)
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
  QueryNode* pProjectedColumns = pNode->pDeepCopy(new PlanNodeList());

  // Iterate over each of the ProjectedColumn children of pNode
  PlanNodeIter* pIter = pNode->pNewChildrenIter();

  while (pIter->bHasNext()) {
    QueryNode* pProjectedColumn = static_cast<QueryNode*>(pIter->pNext());
    ASSERT(ProjectedColumn::bIsOfType(pProjectedColumn));

    // Convert the ProjectedColumn from a symbolic representation to an ordinal representation
    QueryNode* pOrdinalProjectedColumn = ProjectedColumn::pSymbolicToOrdinal(pProjectedColumn, pColExprList, pRootNode);
    pProjectedColumns->vAppendChild(pOrdinalProjectedColumn);
  }

  delete pIter;

  pColExprList->vDeleteAll();
  delete pColExprList;

  return pProjectedColumns;
}

/*----------------------------------------------------------------------------*/
/* Return a list of ProjectedColumns that represent the column expressions      */
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
  QueryNode* pProjColumns = pProjectedColumns(pNode);

  PlanNodeIter* pIter = pProjColumns->pNewChildrenIter();
  while (pIter->bHasNext()) {

    QueryNode* pProjColumn = static_cast<QueryNode*>(pIter->pNext());
    ASSERT(ProjectedColumn::bIsOfType(pProjColumn));

    QueryNode* pDup = pProjColumn->pDeepCopy();
    pColumnRefList->vAppend(pDup);
  }

  delete pIter;
  return pColumnRefList;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
