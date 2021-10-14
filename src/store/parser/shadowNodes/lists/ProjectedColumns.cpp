/*============================================================================*/
/* ProjectedColumns.hpp - a list of ProjectedColumn instances                 */
/*============================================================================*/

#include "BASE.h"
#include "ProjectedColumns.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "ProjectedColumn.hpp"
#include "Expr.hpp"

#include "GraphNav.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_PROJECTEDCOLUMNS_DEFN = "/__root__/definitions/object/list/projectedColumns"_Stz;
static Stz* C_PROJECTEDCOLUMNS_NAME = "projectedColumns"_Stz;



/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Helper function to create a list of catalogDef nodes.                       */
/*----------------------------------------------------------------------------*/
QueryNode* ProjectedColumns::pCreate()
{
  _static_

  // Create an empty projectedColumns object
  QueryNode* pProjectedColumns = new QueryNode(C_PROJECTEDCOLUMNS_NAME, const_cast<Stz*>(C_PROJECTEDCOLUMNS_DEFN));

  return pProjectedColumns;
}

/*----------------------------------------------------------------------------*/
/* Appends a description of the catalog definitions into a buffer              */
/*----------------------------------------------------------------------------*/
void ProjectedColumns::vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode)
{
  _static_

  ASSERT(bIsOfType(pNode));

  QueryNode* pProjectedColumns = _pQueryNode(pNode);

  PlanNodeIter* pIter = pProjectedColumns->pNewChildrenIter();
  b1 bFirst = true;

  while (pIter->bHasNext()) {
    PlanNode* pChild = pIter->pNext();

    if (bFirst) {
      bFirst = false;
    } else {
      sBuf.vConcat(", "_Stz);
    }

    s2K sColBuf;
    ProjectedColumn::vDescribe(pChild, sColBuf, pRootNode);
    sBuf.vConcat(sColBuf.szStz());
  }

  delete pIter;
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the projectedColumns definition   */
/*----------------------------------------------------------------------------*/
b1 ProjectedColumns::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_PROJECTEDCOLUMNS_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 ProjectedColumns::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_PROJECTEDCOLUMNS_DEFN);
}


/*----------------------------------------------------------------------------*/
/* Returns true if the given expression is equal to or a superset of          */
/* one of the projected columns                                               */
/*----------------------------------------------------------------------------*/
b1 ProjectedColumns::bHasExprEqualOrSuperset(QueryNode* pNode, QueryNode* pExpr, b1 bAllowAggregateSupersets)
{
  ASSERT(pNode);
  ASSERT(bIsOfType(pNode));
  ASSERT(pExpr);
  ASSERT(Expr::bIsOfType(pExpr));

  if (pNode->nChildren() == 0) {
    return false;
  }

  b1 bResult = false;

  PlanNodeIter* pIter = pNode->pNewChildrenIter();
  while (pIter->bHasNext()) {
    PlanNode *pChild = pIter->pNext();
    ASSERT(ProjectedColumn::bIsOfType(pChild));

    QueryNode* pChildExpr = ProjectedColumn::pExprNode(_pQueryNode(pChild));
    ASSERT(Expr::bIsOfType(pChildExpr));
    if (Expr::bIsEqualOrSupersetOf(pExpr, pChildExpr)) {

      if (bAllowAggregateSupersets) {
        bResult = true;
        break;
      }

      // We are not allowing pExpr to be classed as a superset if it contains an
      // aggregate and pChildExpr does not. Either both must contain aggregates or
      // both must not
      b1 bExprAgg = Expr::bContainsAnAggregate(pExpr);
      b1 bChildExprAgg = Expr::bContainsAnAggregate(pChildExpr);
      if (bExprAgg == bChildExprAgg) {
        bResult = true;
        break;
      }

      // No good. pExpr is an aggregated superset of pChildExpr.
    }
  }

  delete pIter;
  return bResult;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
