/*============================================================================*/
/* ProjectOp.hpp - operator that changes the defn of rows that it processes   */
/*============================================================================*/

#include "BASE.h"
#include "ProjectOp.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "store/parser/shadowNodes/compound/ProjectedColumn.hpp"
#include "ProjectedColumns.hpp"
#include "AliasRef.hpp"
#include "ShadowRoot.hpp"
#include "MetadataNode.hpp"

#include "GraphNav.hpp"
#include "NodeNamer.hpp"

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
  NodeNamer::vGenerateName(C_PROJECTOP_DEFN, sName);

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

  QueryNode* pProjectOp = _pQueryNode(pNode);

  sBuf.vConcat("ProjectOp :"_Stz);

  QueryNode* pProjectedColumns = _pQueryNode(GraphNav::pFindProperty(C_PROJECTEDCOLUMNS_PROP, pProjectOp));
  ASSERT(pProjectedColumns);

  if (pProjectedColumns->bHasChildren()) {
    PlanNodeIter* pIter = pProjectedColumns->pNewChildrenIter();

    while (pIter->bHasNext()) {
      QueryNode* pProjectedColumn = _pQueryNode(pIter->pNext());
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
void ProjectOp::vExplain(PlanNode *pNode, PlanNode *pRootNode, QueryNode* pExplainNode, s48K &sBuf, i8 iIndent)
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
void ProjectOp::vExplainCommon(PlanNode *pNode, PlanNode* pRootNode, QueryNode* pExplainNode, Stz* szType, s48K &sBuf, i8 iIndent)
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

    // Get the list of projected expressions
    PlanNode* pProjColumns = pProjectedColumns(_pQueryNode(pNode));
    ASSERT(pProjColumns);

    ASSERT(pNode->nChildren() == 1);
    ASSERT(PlanObject::bIsOfType(pNode->pFirstChild()));
    QueryNodeList* pColExprList = nullptr;

    // Explaining very early plans are difficult because we cannot resolve the column expressions.
    // We don't want to call PlanObject::pColumnExprs() below before we have been able
    // to resolve all table/column names in the system.
    // When we resolve names we also add metadata into plan. So if the metadata node has
    // no children then we probably haven't resolved any names yet (except for the case of
    // the plan only containing literals).

    if (MetadataNode::bHasMetadata(ShadowRoot::pMetadataNode(pRootNode))) {
      pColExprList = PlanObject::pColumnExprs(_pQueryNode(pNode->pFirstChild()), pRootNode);
    }

    // Get the list of correlated expressions from the enclosing subquery.
    // We don't need to free this object.
    QueryNode* pCorrelatedExprs = PlanObject::pCorrelatedExprs(_pQueryNode(pNode), pRootNode);

    // Iterate each projected expression
    if (pProjColumns->bHasChildren()) {
      PlanNodeIter* pIter = pProjColumns->pNewChildrenIter();

      while (pIter->bHasNext()) {
        QueryNode* pProjColumn = _pQueryNode(pIter->pNext());
        ASSERT(pProjColumn);
        ASSERT(ProjectedColumn::bIsOfType(pProjColumn));

        // Get the description of the projected expression
        s2K sColBuf;
        ProjectedColumn::vDescribe(pProjColumn, sColBuf, pColExprList, pCorrelatedExprs, pRootNode);
        sBuf.vConcat("\n"_Stz);
        vAppendIndent(sBuf, iIndent, IF_PLAIN);
        sBuf.vCatSprint("column: (%s)"_Stz, sColBuf.szStz());
      }

      delete pIter;
    }

    if (pColExprList) {
      pColExprList->vDeleteAll();
      delete pColExprList;
    }

    return;
  }

  // Manual polymorphism here

}


/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the statement definition    */
/*----------------------------------------------------------------------------*/
b1 ProjectOp::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_PROJECTOP_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given type str is derived from the objects definition  */
/*----------------------------------------------------------------------------*/
b1 ProjectOp::bIsOfType(Stz* szType)
{
  return ShadowNode::bIsOfType(szType, C_PROJECTOP_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely same as its definition         */
/*----------------------------------------------------------------------------*/
b1 ProjectOp::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_PROJECTOP_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 ProjectOp::bIsExactType(Stz* szType)
{
  return ShadowNode::bIsExactType(szType, C_PROJECTOP_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if any of the projected expressions are aggregate expressions */
/*----------------------------------------------------------------------------*/
b1 ProjectOp::bHasAnAggregate(QueryNode* pNode)
{
  QueryNode* pProjColumns =
      _pQueryNode(GraphNav::pFindProperty(const_cast<Stz *>(C_PROJECTEDCOLUMNS_PROP), pNode));

  b1 bResult = false;

  if (pProjColumns->nChildren() == 0) {
    return bResult;
  }

  // Iterate all of the projected columns
  PlanNodeIter* pIter = pProjColumns->pNewChildrenIter();

  while (pIter->bHasNext()) {
    QueryNode* pProjColumn = _pQueryNode(pIter->pNext());

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
/* Returns true if all of the projected expressions are aggregate expressions */
/*----------------------------------------------------------------------------*/
b1 ProjectOp::bHasOnlyAggregates(QueryNode* pNode, b1 bAllowLiterals)
{
  QueryNode* pProjColumns =
      _pQueryNode(GraphNav::pFindProperty(const_cast<Stz *>(C_PROJECTEDCOLUMNS_PROP), pNode));

  if (pProjColumns->nChildren() == 0) {
    return false;
  }

  // Iterate all of the projected columns
  PlanNodeIter* pIter = pProjColumns->pNewChildrenIter();

  b1 bResult = true;
  while (pIter->bHasNext()) {
    QueryNode* pProjColumn = _pQueryNode(pIter->pNext());

    ASSERT(ProjectedColumn::bIsOfType(pProjColumn));
    if (!ProjectedColumn::bContainsAnAggregate(pProjColumn)) {
      if (!bAllowLiterals || !ProjectedColumn::bIsFullyLiteral(pProjColumn)) {
        bResult = false;
        break;
      }
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
      _pQueryNode(GraphNav::pFindProperty(const_cast<Stz *>(C_PROJECTEDCOLUMNS_PROP), pNode));

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
  QueryNodeList* pColExprList = PlanObject::pColumnExprs(_pQueryNode(pNode->pFirstChild()), pRootNode);
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
/* Return a list of ProjectedColumns that represent the column expressions      */
/* that this PlanObject will return. All nodes and the list must be freed.    */
/*----------------------------------------------------------------------------*/
QueryNodeList* ProjectOp::pColumnExprs(QueryNode* pNode, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));
  ASSERT(pRootNode);

  UNUSED(pRootNode);

  // Create a list for the columnRefs to be returned
  QueryNodeList* pColumnRefList = new QueryNodeList();

  // The list we return and all of its elements need to be deletable.
  // So we need to duplicate each projected expression before adding it
  // to the list.
  QueryNode* pProjColumns = pProjectedColumns(pNode);

  PlanNodeIter* pIter = pProjColumns->pNewChildrenIter();
  while (pIter->bHasNext()) {

    QueryNode* pProjColumn = _pQueryNode(pIter->pNext());
    ASSERT(ProjectedColumn::bIsOfType(pProjColumn));

    QueryNode* pDup = pProjColumn->pDeepCopy(true);
    pColumnRefList->vAppend(pDup);
  }

  delete pIter;
  return pColumnRefList;
}

/*----------------------------------------------------------------------------*/
/* Merges the ProjectOp, pOtherOp, down into this ProjectOp                   */
/*----------------------------------------------------------------------------*/
void ProjectOp::vSquash(QueryNode* pNode, QueryNode* pOtherOp, PlanNode* pRootNode)
{
  _static_
  ASSERT(pNode);
  ASSERT(bIsOfType(pNode));
  ASSERT(pOtherOp);
  ASSERT(bIsOfType(pOtherOp));
  ASSERT(pOtherOp->nChildren() == 0);
  ASSERT(pRootNode);

  // We have two ProjectOps on top of each other. pOtherOp is on top of pNode.
  // We need to combine them.
  //
  // We are going to walk the expressions of each projected column of pOtherOp.
  // If we encounter an AliasRef then we are going to replace it with its expanded
  // expression. The reason for this is that an alias in a projected column of
  // pOtherOp can only be referring to an alias that is defined in pNode.
  // Since we are squashing the pNode ProjectOp then this lower alias definition
  // will disappear.

  // Replace all AliasRefs in pOtherOp with their expanded form.
  // We don't want to modify pOtherOp (in case it is pushed down into other
  // parts of the graph), so first make a copy of its projected columns.
  QueryNode* pNewProjCols = ProjectOp::pProjectedColumns(pOtherOp)->pDeepCopy(false);
  ASSERT(pNewProjCols);

  PlanNodeIter* pIter = pNewProjCols->pNewChildrenIter();
  while (pIter->bHasNext()) {
    QueryNode* pNewProjCol = _pQueryNode(pIter->pNext());
    ASSERT(ProjectedColumn::bIsOfType(pNewProjCol));

    QueryNode* pNewExpr = ProjectedColumn::pExprNode(pNewProjCol);

    auto fVisitor = []
        (PlanNode *pExpr, s2K &sPath, i8 iDepth, void *pvArg) -> GraphNav::eWalkCtrl {

      UNUSED(sPath);
      UNUSED(iDepth);
      UNUSED(pvArg);

      if (!AliasRef::bIsOfType(pExpr)) {
        return GraphNav::WALK_CONTINUE;
      }

      QueryNode* pAliasNode = _pQueryNode(pExpr);
      AliasRef::vExpandAlias(pAliasNode);

      // It should no longer be an alias
      ASSERT(!AliasRef::bIsOfType(pAliasNode));
      ASSERT(Expr::bIsOfType(pAliasNode));

      return GraphNav::WALK_CONTINUE;
    };

    GraphNav::bBoundedWalk(pNewExpr, fVisitor, pRootNode);
  }
  delete pIter;

  // Transplant the projected columns and aliases of pOtherOp into pNode.
  // We will do this by swapping the projected columns of pOtherOp and pNode.
  // Then we can delete pOtherOp and everything should be cleaned up.
  QueryNode* pOldProjCols = pNode->pRemoveProperty(C_PROJECTEDCOLUMNS_PROP);
  ASSERT(pOldProjCols);
  pNode->vAppendProperty(pNewProjCols);

  delete pOldProjCols;
}

/*----------------------------------------------------------------------------*/
/* Adds an expression to be projected, with an optional alias                 */
/*----------------------------------------------------------------------------*/
void ProjectOp::vPushdownExpr(QueryNode* pNode, QueryNode* pExpr, Text* pAlias)
{
  ASSERT(pNode);
  ASSERT(bIsOfType(pNode));
  ASSERT(pExpr);
  ASSERT(Expr::bIsOfType(pExpr));
  ASSERT(Expr::bIsNotOrdinal(pExpr));

  // First check if the expression is already being projected
  QueryNode* pExistingProjCol = pColumnWithExpr(pNode, pExpr);

  if (pExistingProjCol) {
    ASSERT(ProjectedColumn::bIsOfType(pExistingProjCol));

    if (!ProjectedColumn::szAliasName(pExistingProjCol) && pAlias) {
      ProjectedColumn::vSetAlias(pExistingProjCol, pAlias);
      pAlias = nullptr;
    }

    delete pExpr;
    delete pAlias;
    return;
  }

  // Create a projected column
  QueryNode* pProjectedColumn = ProjectedColumn::pCreate(pExpr, pAlias);

  // Add it to the projected columns list
  QueryNode* pProjectedColumns = ProjectOp::pProjectedColumns(pNode);
  ASSERT(ProjectedColumns::bIsOfType(pProjectedColumns));

  pProjectedColumns->vAppendChild(pProjectedColumn);
}

/*----------------------------------------------------------------------------*/
/* Returns a new ProjectOp node with projected columns modified for           */
/* partitioned aggregate expressions                                          */
/*----------------------------------------------------------------------------*/
QueryNode* ProjectOp::pCreatePartitioned(QueryNode* pSinglePartitionNode)
{
  QueryNode* pNewOp = pSinglePartitionNode->pDeepCopy(true, new PlanNodeList());

  QueryNode* pProjColumns =
      _pQueryNode(GraphNav::pFindProperty(const_cast<Stz *>(C_PROJECTEDCOLUMNS_PROP), pNewOp));

  // Iterate all of the projected columns
  PlanNodeIter* pIter = pProjColumns->pNewChildrenIter();

  while (pIter->bHasNext()) {
    QueryNode* pProjColumn = _pQueryNode(pIter->pNext());

    ASSERT(ProjectedColumn::bIsOfType(pProjColumn));

    // Check if the current projected column requires changes for partitioning.
    // If so, a non-null QueryNodeList is returned.
    QueryNodeList* pPartitionedColList = ProjectedColumn::pPartitionAggregates(pProjColumn);

    if (!pPartitionedColList) {
      // Column doesn't need partitioning
      continue;
    }

    // Column has been partitioned
    ASSERT(pPartitionedColList->nItems() > 0);

    // Need to remove the current column and insert these ones in its place,
    // unless their expressions are already being projected.
    QueryNode* pNewCol;
    while ((pNewCol = pPartitionedColList->pPop()) != nullptr) {

      // Check if pNewCol is already being projected
      QueryNode* pNewExpr = ProjectedColumn::pExprNode(pNewCol);
      QueryNode* pMatchingProjCol = pColumnWithExpr(pNewOp, pNewExpr);

      if (pMatchingProjCol && pMatchingProjCol != pProjColumn) {
        // This expression is already being projected and not the same
        // as the column we're about to remove. So no need to project it twice.
        delete pNewCol;
        continue;
      }

      pIter->vInsertBefore(pNewCol);
    }

    pIter->vRemove();
    delete pProjColumn;

    ASSERT(pPartitionedColList->nItems() == 0);
    delete pPartitionedColList;
  }

  delete pIter;
  return pNewOp;
}

/*----------------------------------------------------------------------------*/
/* Returns a new ProjectOp node with projected columns super-aggregating      */
/* aggregated expressions from each partition or node                         */
/*----------------------------------------------------------------------------*/
QueryNode* ProjectOp::pCreateSuperAggregate(QueryNode* pSinglePartitionNode)
{
  QueryNode* pNewOp = pSinglePartitionNode->pDeepCopy(true, nullptr);

  QueryNode* pProjColumns =
      _pQueryNode(GraphNav::pFindProperty(const_cast<Stz *>(C_PROJECTEDCOLUMNS_PROP), pNewOp));

  // Iterate all of the projected columns
  PlanNodeIter* pIter = pProjColumns->pNewChildrenIter();

  while (pIter->bHasNext()) {
    QueryNode* pProjColumn = _pQueryNode(pIter->pNext());

    ASSERT(ProjectedColumn::bIsOfType(pProjColumn));

    // Any aggregated expressions need to be super-aggregated
    if (ProjectedColumn::bContainsAnAggregate(pProjColumn)) {
      ProjectedColumn::vSuperAggregate(pProjColumn);
    }
  }

  delete pIter;
  return pNewOp;
}

/*----------------------------------------------------------------------------*/
/* Returns a new ProjectOp node with projected columns doubly super-aggregating*/
/* aggregated expressions from each partition or node                         */
/*----------------------------------------------------------------------------*/
QueryNode* ProjectOp::pCreateDoubleSuperAggregate(QueryNode* pSinglePartitionNode)
{
  QueryNode* pNewOp = pSinglePartitionNode->pDeepCopy(true, nullptr);

  QueryNode* pProjColumns =
      _pQueryNode(GraphNav::pFindProperty(const_cast<Stz *>(C_PROJECTEDCOLUMNS_PROP), pNewOp));

  // Iterate all of the projected columns
  PlanNodeIter* pIter = pProjColumns->pNewChildrenIter();

  while (pIter->bHasNext()) {
    QueryNode* pProjColumn = _pQueryNode(pIter->pNext());

    ASSERT(ProjectedColumn::bIsOfType(pProjColumn));

    // Any aggregated expressions need to be super-aggregated
    if (ProjectedColumn::bContainsAnAggregate(pProjColumn)) {
      ProjectedColumn::vDoubleSuperAggregate(pProjColumn);
    }
  }

  delete pIter;
  return pNewOp;
}

/*----------------------------------------------------------------------------*/
/* Returns the number of columns being projected                              */
/*----------------------------------------------------------------------------*/
i8 ProjectOp::nColumns(QueryNode* pNode)
{
  ASSERT(pNode);
  ASSERT(bIsOfType(pNode));

  QueryNode* pProjCols = pProjectedColumns(pNode);
  return pProjCols->nChildren();
}

/*----------------------------------------------------------------------------*/
/* Returns a ptr to the internal ProjectedColumn node (not Expr) that contains*/
/* a symbolically equal expression as pExpr, or nullptr                       */
/*----------------------------------------------------------------------------*/
QueryNode* ProjectOp::pColumnWithExpr(QueryNode* pNode, QueryNode* pExpr)
{
  ASSERT(pNode);
  ASSERT(bIsOfType(pNode));
  ASSERT(pExpr);
  ASSERT(Expr::bIsOfType(pExpr));
  ASSERT(Expr::bIsNotOrdinal(pExpr));

  QueryNode* pMatchingProjCol = nullptr;

  PlanNodeIter* pIter = pProjectedColumns(pNode)->pNewChildrenIter();
  while (pIter->bHasNext()) {

    QueryNode* pProjCol = _pQueryNode(pIter->pNext());
    ASSERT(ProjectedColumn::bIsOfType(pProjCol));

    QueryNode* pColExpr = ProjectedColumn::pExprNode(pProjCol);
    ASSERT(Expr::bIsOfType(pColExpr));

    if (Expr::bIsSymbolicallyEqual(pExpr, pColExpr)) {
      pMatchingProjCol = pProjCol;
      break;
    }
  }

  delete pIter;

  return pMatchingProjCol;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
