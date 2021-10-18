/*============================================================================*/
/* OrdinalOrdinalSubqueryRefExpr.hpp - a temp ordinal form of SubqueryRefExpr */
/*============================================================================*/

#include "BASE.h"
#include "OrdinalSubqueryRefExpr.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "PlanConstants.hpp"
#include "NodeNamer.hpp"

#include "ShadowRoot.hpp"
#include "MetadataNode.hpp"
#include "StringObject.hpp"
#include "ExprList.hpp"
#include "CteDef.hpp"
#include "ColumnDef.hpp"
#include "MainSubquery.hpp"
#include "ExprSubquery.hpp"
#include "ExprList.hpp"
#include "SqlOperator.hpp"


#include "GraphNav.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_DEFN        = "/__root__/definitions/object/expr/subqueryRefExpr/ordinalSubqueryRefExpr"_Stz;
static Stz* C_PARENT_DEFN = "/__root__/definitions/object/expr/subqueryRefExpr"_Stz;


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Helper function to create this node.                                       */
/*----------------------------------------------------------------------------*/
QueryNode* OrdinalSubqueryRefExpr::pCreate(QueryNode* pSymbolicSubqueryRefExpr, QueryNodeList* pOrdinalParams)
{
  _static_

  ASSERT(pSymbolicSubqueryRefExpr);
  ASSERT(SubqueryRefExpr::bIsExactType(pSymbolicSubqueryRefExpr));
  ASSERT(pOrdinalParams);

  // Create a unique name for this object
  s256 sName;
  NodeNamer::vGenerateName(C_DEFN, sName);

  QueryNode* pOrdinalSubqueryRefExpr = pSymbolicSubqueryRefExpr->pDeepCopy(false);
  pOrdinalSubqueryRefExpr->vSetName(sName);
  pOrdinalSubqueryRefExpr->vSetType(C_DEFN);

  // Add properties

  // Add the ordinal parameters, if any
  QueryNode* pParam;
  while ((pParam = pOrdinalParams->pPop()) != nullptr) {
    pOrdinalSubqueryRefExpr->vAppendChild(pParam);
  }

  return pOrdinalSubqueryRefExpr;
}


/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the expr definition         */
/*----------------------------------------------------------------------------*/
b1 OrdinalSubqueryRefExpr::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 OrdinalSubqueryRefExpr::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the expression does not contain any column references.     */
/* All expressions are relative column ordinals, operators or literals only.  */
/*----------------------------------------------------------------------------*/
b1 OrdinalSubqueryRefExpr::bIsFullyOrdinal(QueryNode* pExpr)
{
  ASSERT(pExpr);
  ASSERT(bIsOfType(pExpr));

  if (pExpr->nChildren() == 0) {
    return true;
  }

  // Check that each child expression is ordinal
  PlanNodeIter* pIter = pExpr->pNewChildrenIter();

  while (pIter->bHasNext()) {
    QueryNode* pChild = _pQueryNode(pIter->pNext());
    ASSERT(Expr::bIsOfType(pChild));

    if (!Expr::bIsFullyOrdinal(pChild)) {
      delete pIter;
      return false;
    }
  }

  // all good
  delete pIter;
  return true;
}

/*----------------------------------------------------------------------------*/
/* Takes an ordinal expression and returns a symbolic copy of it based on the */
/* column expressions projected from a child sql operator.                    */
/* The returned QueryNode must be freed.                                      */
/*----------------------------------------------------------------------------*/
QueryNode* OrdinalSubqueryRefExpr::pOrdinalToSymbolic(QueryNode*     pOrdinalExpr,
                                                      QueryNodeList* pColumnExprList,
                                                      PlanNode*      pRootNode)
{
  ASSERT(bIsOfType(pOrdinalExpr));

  QueryNode* pSymbolicExpr = pOrdinalExpr->pDeepCopy(true);

  QueryNode* pChild;
  while ((pChild = pSymbolicExpr->pPopChild()) != nullptr) {
    delete pChild;
  }

  ASSERT(pSymbolicExpr->nChildren() == 0);

  // Don't forget to update the definition
  pSymbolicExpr->vSetType(C_PARENT_DEFN);

  return pSymbolicExpr;
}



/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
