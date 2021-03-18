/*============================================================================*/
/* ProjectedExpr.hpp - a projected expression that may have an alias          */
/*============================================================================*/

#include "BASE.h"
#include "ProjectedExpr.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "PlanConstants.hpp"
#include "Expr.hpp"

#include "GraphNav.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_PROJECTEDEXPR_DEFN = "/__root__/definitions/object/compound/projectedExpr"_Stz;
static Stz* C_PROJECTEDEXPR_NAME = "projectedExpr"_Stz;
static Stz* C_EXPR_PROP  = "expr"_Stz;
static Stz* C_ALIAS_PROP = "alias"_Stz;


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Returns the aliasName of the projectedExpr                                 */
/*----------------------------------------------------------------------------*/
Stz* ProjectedExpr::szAliasName(QueryNode* pNode)
{
  QueryNode *pAliasNode =
      static_cast<QueryNode*>(GraphNav::pFindProperty(const_cast<Stz *>(C_ALIAS_PROP), pNode));

  return pAliasNode->szValue();;
}

/*----------------------------------------------------------------------------*/
/* Helper function to create ProjectedExpr nodes.                             */
/*----------------------------------------------------------------------------*/
QueryNode* ProjectedExpr::pCreate(QueryNode* pExprNode, Text* pAlias)
{
  _static_

  ASSERT(pExprNode);
  ASSERT(!pAlias || pAlias->nLength() > 0);

  Value xAliasValue(pAlias);

  s256 sName;
  ShadowNode::vObjName(C_PROJECTEDEXPR_NAME, sName);

  // Create the ProjectedExpr object
  QueryNode* pProjectedExpr = new QueryNode(sName.szStz(), const_cast<Stz*>(C_PROJECTEDEXPR_DEFN));

  // Add the expression as a property
  // Make sure it has the right property name first
  pExprNode->vSetName(C_EXPR_PROP);
  pProjectedExpr->vAppendProperty(pExprNode);

  // Create the alias and append it as a property
  QueryNode* pAliasNode  = new QueryNode(C_ALIAS_PROP, const_cast<Stz*>(C_STRING_OBJ_DEFN), static_cast<Value&&>(xAliasValue));
  pProjectedExpr->vAppendProperty(pAliasNode);

  return pProjectedExpr;
}

/*----------------------------------------------------------------------------*/
/* Appends a description of the ProjectedExpr into a buffer                   */
/*----------------------------------------------------------------------------*/
void ProjectedExpr::vDescribe(PlanNode* pNode, s2K& sBuf)
{
  _static_

  vDescribe(pNode, sBuf, nullptr, nullptr);
}

/*----------------------------------------------------------------------------*/
/* Appends a description of the ProjectedExpr into a buffer                   */
/*----------------------------------------------------------------------------*/
void ProjectedExpr::vDescribe(PlanNode* pNode, s2K& sBuf, PlanNodeList* pColumnExprList, PlanNode* pRootNode)
{
  _static_

  ASSERT(bIsOfType(pNode));

  QueryNode* pProjectedExpr = static_cast<QueryNode*>(pNode);

  QueryNode* pExprNode = static_cast<QueryNode*>(GraphNav::pFindProperty(C_EXPR_PROP, pProjectedExpr));
  QueryNode* pAliasNode  = static_cast<QueryNode*>(GraphNav::pFindProperty(C_ALIAS_PROP, pProjectedExpr));

  ASSERT(pExprNode);
  ASSERT(pAliasNode);

  Stz* szAliasName = pAliasNode->szValue();

  // Describe the expression
  s2K sExprBuf;
  Expr::vDescribe(pExprNode, sExprBuf);

  if (szAliasName) {
    sBuf.vSprintCat("Alias = %s ; "_Stz, (szAliasName == nullptr ? "<none>"_Stz : szAliasName));
  }

  sBuf.vSprintCat("Expr = %s"_Stz, sExprBuf.szStz());

  if (pColumnExprList && pRootNode) {
    sExprBuf.vReset();
    QueryNode* pOrdinalExpr = Expr::pSymbolicToOrdinal(pExprNode, pColumnExprList, pRootNode);
    Expr::vDescribe(pOrdinalExpr, sExprBuf);
    delete pOrdinalExpr;

    sBuf.vSprintCat(" ; Ordinal Expr = %s"_Stz, sExprBuf.szStz());
  }
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the ProjectedExpr definition*/
/*----------------------------------------------------------------------------*/
b1 ProjectedExpr::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_PROJECTEDEXPR_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the expression in this node is an aggregate expression     */
/*----------------------------------------------------------------------------*/
b1 ProjectedExpr::bIsAnAggregate(QueryNode* pNode)
{
  QueryNode *pExprNode = ProjectedExpr::pExprNode(pNode);

  return Expr::bIsAnAggregate(pExprNode);
}

/*----------------------------------------------------------------------------*/
/* Returns the exprNode of the ProjectedExpr                                  */
/*----------------------------------------------------------------------------*/
QueryNode* ProjectedExpr::pExprNode(QueryNode* pNode)
{
  QueryNode *pExprNode =
      static_cast<QueryNode*>(GraphNav::pFindProperty(const_cast<Stz *>(C_EXPR_PROP), pNode));

  return pExprNode;
}

/*----------------------------------------------------------------------------*/
/* Returns a new copy of this projected expression rewritten relative to      */
/* column ordinals. Must be freed after use.                                  */
/*----------------------------------------------------------------------------*/
QueryNode* ProjectedExpr::pSymbolicToOrdinal(QueryNode* pNode,
                                             PlanNodeList* pColExprList,
                                             PlanNode* pRootNode)
{
  // Get the column projection expression out of this ProjectedExpr and convert it
  // from symbolic to ordinal representation.
  QueryNode* pSymbolicExpr = pExprNode(pNode);
  ASSERT(pSymbolicExpr);

  // Use the column expression list and the above symbolic column projection expression to
  // generate an ordinal column projection expression (one that is relative to column
  // ordinals only, not column names).
  QueryNode* pOrdinalExpr = Expr::pSymbolicToOrdinal(pSymbolicExpr, pColExprList, pRootNode);
  ASSERT(pOrdinalExpr);
  ASSERT(Expr::bIsOrdinal(pOrdinalExpr));

  // Get the alias name of this column and (if applicable) duplicate it
  Text* pAlias = nullptr;
  if (ProjectedExpr::szAliasName(pNode)) {
    pAlias = new Text(ProjectedExpr::szAliasName(pNode));
  }

  // Create a new ProjectedExpr and return it
  QueryNode* pOrdinalProjectedExpr = ProjectedExpr::pCreate(pOrdinalExpr, pAlias);

  return pOrdinalProjectedExpr;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
