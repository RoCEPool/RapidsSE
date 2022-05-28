/*============================================================================*/
/* SetOpValidator.hpp - Simplifies and validates plan segments that contain   */
/* set operations (e.g. UNION, INSERSECT, etc)                                */
/*============================================================================*/

#include "BASE.h"
#include "SetOpValidator.hpp"
#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "GraphNav.hpp"
#include "ShadowRoot.hpp"
#include "ShadowPlan.hpp"
#include "StatementMarker.hpp"
#include "SubqueryMarker.hpp"
#include "SqlException.hpp"
#include "ProjectedColumn.hpp"
#include "StringObject.hpp"
#include "ProjectOp.hpp"
#include "Expr.hpp"
#include "ColumnRef.hpp"

#include "SetOp.hpp"


using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Returns true if there are any set operations in this plan.                 */
/* If not then we can ignore these other functions.                           */
/*----------------------------------------------------------------------------*/
b1 SetOpValidator::bHasSetOps(PlanNode *pRootNode)
{
  PlanNode* pPlanNode = ShadowRoot::pPlanRoot(pRootNode);
  ASSERT(pPlanNode);
  ASSERT(ShadowPlan::bIsOfType(pPlanNode));

  // A condition to test for any set operators
  auto fCondition = [](PlanNode* pNode, void* pvArg) -> b1 {
    UNUSED(pvArg);
    return (SetOp::bIsOfType(pNode));
  };

  PlanNode* pSetOp = GraphNav::pGlobalSearch(pPlanNode,
                                             false,
                                             true,
                                             fCondition,
                                             nullptr);

  return pSetOp != nullptr;
}

/*----------------------------------------------------------------------------*/
/* Tries to simplify the set operators, if needed, by handling                */
/* the CORRESPONDING BY list.                                                 */
/*----------------------------------------------------------------------------*/
void SetOpValidator::vSimplifySetOps(PlanNode *pRootNode)
{
  // Find the /plan node
  QueryNode* pPlanNode = ShadowRoot::pPlanRoot(pRootNode);
  ASSERT(pPlanNode);

  // The plan node should have one or more children which are SubqueryMarker nodes
  ASSERT(pPlanNode->nChildren() > 0);
  ASSERT(SubqueryMarker::bIsOfType(pPlanNode->pFirstChild()));

  // Iterate each logical subgraph, bounded by StatementMarkers
  auto fVisitor = [pRootNode]
      (PlanNode *pNode, s2K &sPath, i8 iDepth, void *pvArg) -> GraphNav::eWalkCtrl {

    UNUSED(sPath);
    UNUSED(iDepth);
    UNUSED(pvArg);

    if (!StatementMarker::bIsOfType(pNode)) {
      return GraphNav::WALK_CONTINUE;
    }

    // Simplify the SetOp in this statement section
    QueryNode* pStatementMarker = _pQueryNode(pNode);
    vSimplifySetOp(pStatementMarker, pRootNode);

    return GraphNav::WALK_CONTINUE;
  };

  // Walk the entire plan graph to look for StatementMarkers.
  // We must do so in a bottom up manner because if there are multiple
  // set operators then the top level ones will depend on the ones below it.
  GraphNav::bGlobalWalkBottomUp(pPlanNode, false, true, fVisitor, pRootNode);
}

/*----------------------------------------------------------------------------*/
/* Walk the graph and check the semantics of any set operators.               */
/* e.g. Check that the operation supports the input data types.               */
/* A SqlException will be thrown if an invalid semantic is found.             */
/*----------------------------------------------------------------------------*/
void SetOpValidator::vValidateSetOps(PlanNode *pRootNode)
{
  // For every set operator found:
  //  - Check that the set operator has a null CORRESPONDING BY list.
  //  - Check that both children are projecting the same number of columns.
  //  - Check that the columns expression list of each child of a set operator
  //    has a matching type signature.

  // Find the /plan node
  QueryNode* pPlanNode = ShadowRoot::pPlanRoot(pRootNode);
  ASSERT(pPlanNode);

  // The plan node should have one or more children that are SubqueryMarker nodes
  ASSERT(pPlanNode->nChildren() > 0);
  ASSERT(SubqueryMarker::bIsOfType(pPlanNode->pFirstChild()));

  // Iterate each logical subgraph, bounded by StatementMarkers
  auto fVisitor = [pRootNode]
      (PlanNode *pNode, s2K &sPath, i8 iDepth, void *pvArg) -> GraphNav::eWalkCtrl {

    UNUSED(sPath);
    UNUSED(iDepth);
    UNUSED(pvArg);

    if (!StatementMarker::bIsOfType(pNode)) {
      return GraphNav::WALK_CONTINUE;
    }

    // Validate the SetOp in this statement section
    QueryNode* pStatementMarker = _pQueryNode(pNode);
    vValidateSetOp(pStatementMarker, pRootNode);

    return GraphNav::WALK_CONTINUE;
  };

  // Walk the entire plan graph to look for StatementMarkers.
  // We must do so in a bottom up manner because if there are multiple
  // set operators then the top level ones will depend on the ones below it.
  GraphNav::bGlobalWalkBottomUp(pPlanNode, false, true, fVisitor, pRootNode);
}

/*----------------------------------------------------------------------------*/
/* Validate the semantics of a set operator for a given statement section.    */
/*----------------------------------------------------------------------------*/
void SetOpValidator::vValidateSetOp(QueryNode* pStatementMarker, PlanNode *pRootNode)
{
  // For every set operator found:
  //  - Check that the set operator has a null CORRESPONDING BY list.
  //  - Check that both children are projecting the same number of columns.
  //  - Check that the columns expression list of each child of a set operator
  //    has a matching type signature.

  // Firstly, do we have a set operator in this statement section (bounded by StatementMarkers)?
  // Do a search for them within this statement section only.
  auto fCondition = [](PlanNode* pNode, void* pvArg) -> b1 {
    UNUSED(pvArg);
    return (SetOp::bIsOfType(pNode));
  };

  PlanNode* pNode = GraphNav::pBoundedSearch(pStatementMarker, false, true, fCondition, nullptr);

  if (!pNode) {
    // No set operators, so nothing to do.
    return;
  }

  QueryNode* pSetOp = _pQueryNode(pNode);
  ASSERT(pSetOp->nChildren() == 2);

  //  - Check that the set operator has a null CORRESPONDING BY list.
  // The corresponding by list should be null. This reflects that if a CORRESPONDING
  // BY clause was specified by the user that it the specified columns have been
  // pushed down as ProjectOps underneath this set operator.
  QueryNode* pCorrByList = SetOp::pCorrespondingByList(pSetOp);
  RASSERT(!pCorrByList);

  //  - Check that both children are projecting the same number of columns.
  QueryNode* pChild1 = _pQueryNode(pSetOp->pFirstChild());
  QueryNode* pChild2 = _pQueryNode(pSetOp->pSecondChild());

  QueryNodeList* pProjectedColumns1 = PlanObject::pColumnExprs(pChild1, pRootNode);
  QueryNodeList* pProjectedColumns2 = PlanObject::pColumnExprs(pChild2, pRootNode);

  i8 iCount1 = pProjectedColumns1->nItems();
  i8 iCount2 = pProjectedColumns2->nItems();

  if (iCount1 != iCount2) {
    pProjectedColumns1->vDeleteAll();
    pProjectedColumns2->vDeleteAll();
    delete pProjectedColumns1;
    delete pProjectedColumns2;

    SemanticException::vThrow(EC_COLUMN_COUNT_MISMATCH,
                              ": %lld vs %lld, %s operator"_Stz,
                              iCount1,
                              iCount2,
                              SetOp::szSqlKeyword(pSetOp) );
  }

  //  - Check that the columns expression list of each child of a set operator
  //    has a matching type signature.
  QueryNodeIter* pIter1 = pProjectedColumns1->pNewIter();
  QueryNodeIter* pIter2 = pProjectedColumns2->pNewIter();

  i8 iOrdinal = 0;
  while (pIter1->bHasNext() && pIter2->bHasNext()) {
    ++iOrdinal;

    QueryNode* pProjectedColumn1 = pIter1->pNext();
    QueryNode* pProjectedColumn2 = pIter2->pNext();

    ASSERT(ProjectedColumn::bIsOfType(pProjectedColumn1));
    ASSERT(ProjectedColumn::bIsOfType(pProjectedColumn2));

    QueryNode* pExpr1 = ProjectedColumn::pExprNode(pProjectedColumn1);
    QueryNode* pExpr2 = ProjectedColumn::pExprNode(pProjectedColumn2);

    TypeId xType1 = Expr::xTypeId(pExpr1, pRootNode);
    TypeId xType2 = Expr::xTypeId(pExpr2, pRootNode);

    if (xType1 != xType2) {
      delete pIter1;
      delete pIter2;

      pProjectedColumns1->vDeleteAll();
      pProjectedColumns2->vDeleteAll();
      delete pProjectedColumns1;
      delete pProjectedColumns2;

      SemanticException::vThrow(EC_COLUMN_COUNT_MISMATCH,
                                ": %s vs %s, column ordinal %lld, %s operator"_Stz,
                                aCT[xType1.eCTVal()],
                                aCT[xType2.eCTVal()],
                                iOrdinal,
                                SetOp::szSqlKeyword(pSetOp) );
    }
  }

  delete pIter1;
  delete pIter2;

  pProjectedColumns1->vDeleteAll();
  pProjectedColumns2->vDeleteAll();
  delete pProjectedColumns1;
  delete pProjectedColumns2;
}

/*----------------------------------------------------------------------------*/
/* Tries to simplify a single set operators if needed, by handling            */
/* the CORRESPONDING BY list.                                                 */
/*----------------------------------------------------------------------------*/
void SetOpValidator::vSimplifySetOp(QueryNode* pStatementMarker, PlanNode *pRootNode)
{
  // There are 3 cases we need to consider for each set operator's CORRESPONDING
  // BY list:
  //   1. The list is null.
  //   2. The list is non-null and not empty.
  //   3. The list is non-null but empty.
  //
  // Here's what they mean and what we have to do.
  //   1. The list is null.
  //      In this case  it is assumed that the corresponding column ordinals
  //      from each child query are of the same type. i.e. column 1 of child 1
  //      matches the same type as column 1 of child 2.
  //      There is no work to be done in this case as the validation of the column
  //      types will occur in vValidateSetOps().
  //
  //   2. The list is non-null and not empty.
  //      This means that the list contains the names of common columns that can
  //      be found in the projection list of each child query.
  //      e.g. if the correspondingByList contained "foo" then there should be a
  //      column named "foo" being projected by both child queries.
  //      This scenario should be simplified by first confirming that the named
  //      columns exist in both children, and then adding a ProjectOp between the
  //      set operator and the StatementMarker of the child queries. The ProjectOp
  //      should only project those columns in the correspondingByList. The
  //      correspondingByList in the set operator should now be deleted and nulled.
  //      Then we are equal to case #1 above.
  //
  //   3. The list is non-null but empty.
  //      In this version we have to get the names of each column of each query
  //      and determine which ones of these are common (in a case insensitive
  //      manner). Then we proceed as if we were in case #2 above and this then
  //      can be simplified to case #1.

  // Firstly, do we have a set operator in this statement section (bounded by StatementMarkers)?
  // Do a search for them within this statement section only.
  auto fCondition = [](PlanNode* pNode, void* pvArg) -> b1 {
    UNUSED(pvArg);
    return (SetOp::bIsOfType(pNode));
  };

  PlanNode* pNode = GraphNav::pBoundedSearch(pStatementMarker, false, true, fCondition, nullptr);

  if (!pNode) {
    // No set operators, so nothing to do.
    return;
  }

  QueryNode* pSetOp = _pQueryNode(pNode);
  ASSERT(pSetOp->nChildren() == 2);

  QueryNode* pCorrByList = SetOp::pCorrespondingByList(pSetOp);

  if (pCorrByList == nullptr) {
    // Case #1 - assume that we are joining all columns from both children
    // and that they have equal column types.
    return;
  }

  if (pCorrByList->nChildren() == 0) {
    // Case #3 - the CORRESPONDING BY list is non null but empty.
    // We need to determine the common columns by name (case insensitively)
    // and then add them to the CORRESPONDING BY list
    vDetermineCommonColumns(pSetOp, pRootNode);

    if (pCorrByList->nChildren() == 0) {
      SemanticException::vThrow(EC_CORRBY_NO_COMMON_COLUMNS);
    }

    ASSERT(pCorrByList->nChildren() > 0);
    // Fall out and now execute case #2
  }

  // Case #2 - The CORRESPONDING BY list is non-null and not empty.
  // We need to check the types of the columns specified in the
  // CORRESPONDING BY list and then convert these into ProjectOps
  // for each child.
  ASSERT(pCorrByList->nChildren() > 0);
  vConvertCorrByToProjectOp(pSetOp, pRootNode);

  ASSERT(SetOp::pCorrespondingByList(pSetOp) == nullptr);
  ASSERT(ProjectOp::bIsOfType(pSetOp->pFirstChild()));
  ASSERT(ProjectOp::bIsOfType(pSetOp->pSecondChild()));
}

/*----------------------------------------------------------------------------*/
/* Determine the common column names between the children of the set operator */
/* and populate its corresponding by list                                     */
/*----------------------------------------------------------------------------*/
void SetOpValidator::vDetermineCommonColumns(QueryNode* pSetOp, PlanNode* pRootNode)
{
  ASSERT(pRootNode);
  ASSERT(pSetOp);
  ASSERT(SetOp::bIsOfType(pSetOp));

  QueryNode* pChild1 = _pQueryNode(pSetOp->pFirstChild());
  QueryNode* pChild2 = _pQueryNode(pSetOp->pSecondChild());

  QueryNode* pCorrByList = SetOp::pCorrespondingByList(pSetOp);
  ASSERT(pCorrByList);
  ASSERT(pCorrByList->nChildren() == 0);

  QueryNodeList* pProjColumnList1 = PlanObject::pColumnExprs(pChild1, pRootNode);
  QueryNodeList* pProjColumnList2 = PlanObject::pColumnExprs(pChild2, pRootNode);

  QueryNodeIter* pIter1 = pProjColumnList1->pNewIter();
  QueryNodeIter* pIter2 = pProjColumnList2->pNewIter();

  while (pIter1->bHasNext()) {
    QueryNode* pProjColumn1 = pIter1->pNext();
    ASSERT(ProjectedColumn::bIsOfType(pProjColumn1));

    Stz* szName1 = ProjectedColumn::szAliasName(pProjColumn1);
    if (!szName1) {
      // Is there a ColumnRef that we can get a column name from?
      QueryNode* pExpr1 = ProjectedColumn::pExprNode(pProjColumn1);
      if (ColumnRef::bIsOfType(pExpr1)) {
        szName1 = ColumnRef::szColumn(pExpr1);
      }

      if (!szName1) {
        // No name, so it cannot be common
        continue;
      }
    }

    // Check szName1 against all columns of child2
    pIter2->vBeforeFirst();
    while (pIter2->bHasNext()) {
      QueryNode* pProjColumn2 = pIter2->pNext();
      ASSERT(ProjectedColumn::bIsOfType(pProjColumn2));

      Stz* szName2 = ProjectedColumn::szAliasName(pProjColumn2);
      if (!szName2) {
        // Is there a ColumnRef that we can get a column name from?
        QueryNode* pExpr2 = ProjectedColumn::pExprNode(pProjColumn2);
        if (ColumnRef::bIsOfType(pExpr2)) {
          szName2 = ColumnRef::szColumn(pExpr2);
        }

        if (!szName2) {
          // No name, so it cannot be common
          continue;
        }
      }

      // Compare column names case insensitively
      if (szName1->bSame(szName2)) {
        // Add to the corrByList of the set operator.
        // We are not going to check for compatible types here as we will do
        // that as part of case #2 (non-empty CORRESPONDING BY LIST).
        QueryNode* pColumnName = StringObject::pCreate(new Text(szName1));
        pCorrByList->vAppendChild(pColumnName);
        break;
      }
    }
  }

  // Cleanup
  delete pIter1;
  delete pIter2;

  pProjColumnList1->vDeleteAll();
  pProjColumnList2->vDeleteAll();
  delete pProjColumnList1;
  delete pProjColumnList2;
}

/*----------------------------------------------------------------------------*/
/* Convert the columns in the CORRESPONDING BY list to ProjectOps in each     */
/* child of the set operator.                                                 */
/*----------------------------------------------------------------------------*/
void SetOpValidator::vConvertCorrByToProjectOp(QueryNode* pSetOp, PlanNode* pRootNode)
{
  ASSERT(pRootNode);
  ASSERT(pSetOp);
  ASSERT(SetOp::bIsOfType(pSetOp));

  QueryNode* pCorrByList = SetOp::pCorrespondingByList(pSetOp);
  ASSERT(pCorrByList);
  ASSERT(pCorrByList->nChildren() > 0);

  // Get the projected columns of each child
  QueryNode* pChild1 = _pQueryNode(pSetOp->pFirstChild());
  QueryNode* pChild2 = _pQueryNode(pSetOp->pSecondChild());

  QueryNodeList* pProjColumnList1 = PlanObject::pColumnExprs(pChild1, pRootNode);
  QueryNodeList* pProjColumnList2 = PlanObject::pColumnExprs(pChild2, pRootNode);

  QueryNodeIter* pProjColumnIter1 = pProjColumnList1->pNewIter();
  QueryNodeIter* pProjColumnIter2 = pProjColumnList2->pNewIter();

  QueryNodeList* pNewProjectedColumnList1 = new QueryNodeList();
  QueryNodeList* pNewProjectedColumnList2 = new QueryNodeList();

  // Iterate the CORRESPONDING BY list
  PlanNodeIter* pCorrByIter = pCorrByList->pNewChildrenIter();
  while (pCorrByIter->bHasNext()) {
    QueryNode* pStringObj = _pQueryNode(pCorrByIter->pNext());
    Stz* szColumnName = pStringObj->szValue();
    ASSERT(szColumnName);

    // Find this column name in each child
    QueryNode* pProjectedColumn1 = pFindProjectedColumn(pProjColumnIter1, szColumnName);
    QueryNode* pProjectedColumn2 = pFindProjectedColumn(pProjColumnIter2, szColumnName);

    if (!pProjectedColumn1 || !pProjectedColumn2) {
      // We couldn't find the column in one or both of the child queries.
      // Cleanup then throw an error
      delete pCorrByIter;

      delete pProjColumnIter1;
      delete pProjColumnIter2;

      pProjColumnList1->vDeleteAll();
      pProjColumnList2->vDeleteAll();
      delete pProjColumnList1;
      delete pProjColumnList2;

      pNewProjectedColumnList1->vDeleteAll();
      pNewProjectedColumnList2->vDeleteAll();
      delete pNewProjectedColumnList1;
      delete pNewProjectedColumnList2;

      SemanticException::vThrow(EC_CORRBY_COLUMN_NOT_FOUND, ": %s"_Stz, szColumnName);
    }

    // Check the type of this column matches
    QueryNode* pExpr1 = ProjectedColumn::pExprNode(pProjectedColumn1);
    QueryNode* pExpr2 = ProjectedColumn::pExprNode(pProjectedColumn2);

    ASSERT(Expr::bIsOfType(pExpr1));
    ASSERT(Expr::bIsOfType(pExpr2));

    TypeId xType1 = Expr::xTypeId(pExpr1, pRootNode);
    TypeId xType2 = Expr::xTypeId(pExpr2, pRootNode);

    if (xType1 != xType2) {
      // Cleanup then throw an error
      delete pCorrByIter;

      delete pProjColumnIter1;
      delete pProjColumnIter2;

      pProjColumnList1->vDeleteAll();
      pProjColumnList2->vDeleteAll();
      delete pProjColumnList1;
      delete pProjColumnList2;

      delete pNewProjectedColumnList1;
      delete pNewProjectedColumnList2;

      SemanticException::vThrow(EC_CORRBY_COLUMN_TYPE_MISMATCH,
                                ": %s - %s, %s "_Stz,
                                szColumnName,
                                aCT[xType1.eCTVal()],
                                aCT[xType2.eCTVal()]);
    }

    // Found the column name in each list and the types match
    // Add it to the project list for each child.
    // We must add it using the symbolic expr that it is know by, so that
    // the ordinal conversion will still match it correctly.
    QueryNode* pNewProjectedColumn1 = pProjectedColumn1->pDeepCopy(true);
    ProjectedColumn::vSetAlias(pNewProjectedColumn1, new Text(szColumnName));
    pNewProjectedColumnList1->vAppend(pNewProjectedColumn1);

    QueryNode* pNewProjectedColumn2 = pProjectedColumn2->pDeepCopy(true);
    ProjectedColumn::vSetAlias(pNewProjectedColumn2, new Text(szColumnName));
    pNewProjectedColumnList2->vAppend(pNewProjectedColumn2);

  }

  // Now we need to turn these new projected column lists into ProjectOps
  // and insert them below the set operator

  ASSERT(pNewProjectedColumnList1->nItems() > 0);
  ASSERT(pNewProjectedColumnList2->nItems() > 0);

  QueryNode* pProjectOp1 = ProjectOp::pCreate(pNewProjectedColumnList1);
  QueryNode* pProjectOp2 = ProjectOp::pCreate(pNewProjectedColumnList2);

  pSetOp->vInsertAsParentOfChild1(pProjectOp1);
  pSetOp->vInsertAsParentOfChild2(pProjectOp2);

  // Now we can make the CORRESPONDING BY list in the set operator null
  SetOp::vRemoveCorrespondingByList(pSetOp);

  // cleanup
  delete pCorrByIter;

  delete pProjColumnIter1;
  delete pProjColumnIter2;

  pProjColumnList1->vDeleteAll();
  pProjColumnList2->vDeleteAll();
  delete pProjColumnList1;
  delete pProjColumnList2;

  delete pNewProjectedColumnList1;
  delete pNewProjectedColumnList2;
}

/*----------------------------------------------------------------------------*/
/* Given an iterator of a list of ProjectedColumns, find and return the       */
/* ProjectedColumn that matches a given column name. If the column is not     */
/* found then a nullptr is returned.                                          */
/*----------------------------------------------------------------------------*/
QueryNode* SetOpValidator::pFindProjectedColumn(QueryNodeIter* pProjColumnIter, Stz* szColumnName)
{
  pProjColumnIter->vBeforeFirst();

  while (pProjColumnIter->bHasNext()) {
    QueryNode* pProjColumn = pProjColumnIter->pNext();
    ASSERT(ProjectedColumn::bIsOfType(pProjColumn));

    // Can we match the alias?
    Stz* szAlias = ProjectedColumn::szAliasName(pProjColumn);
    if (szAlias && szColumnName->bSame(szAlias)) {
      return pProjColumn;
    }

    // Is the projected expression a column reference that we can match against?
    QueryNode* pExpr = ProjectedColumn::pExprNode(pProjColumn);
    ASSERT(Expr::bIsOfType(pExpr));

    if (ColumnRef::bIsOfType(pExpr)) {
      ASSERT(ColumnRef::bIsFullyQualified(pExpr));
      Stz* szRefColumnName = ColumnRef::szColumn(pExpr);
      ASSERT(szRefColumnName);

      if (szColumnName->bSame(szRefColumnName)) {
        return pProjColumn;
      }
    }
  }

  // not found
  return nullptr;
}

/*============================================================================*/

/*============================================================================*/



/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
