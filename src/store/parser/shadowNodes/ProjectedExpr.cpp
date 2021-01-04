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
/* Helper function to create ProjectedExpr nodes.                             */
/*----------------------------------------------------------------------------*/
QueryNode* ProjectedExpr::pCreate(QueryNode* pExprNode, Text* pAlias)
{
  _static_

  ASSERT(pExprNode);
  ASSERT(!pAlias || pAlias->nLength() > 0);

  Value xAliasValue(pAlias);

  // Create the ProjectedExpr object
  QueryNode* pProjectedExpr = new QueryNode(C_PROJECTEDEXPR_NAME, const_cast<Stz*>(C_PROJECTEDEXPR_DEFN));

  // Add the expression as a property
  // Make sure it has the right property name first
  pExprNode->vRename(C_EXPR_PROP);
  pProjectedExpr->vAppendProperty(pExprNode);

  // Create the alias and append it as a property
  QueryNode* pAliasNode  = new QueryNode(C_ALIAS_PROP, const_cast<Stz*>(C_STRING_OBJ_DEFN), xAliasValue);
  pProjectedExpr->vAppendProperty(pAliasNode);

  return pProjectedExpr;
}

/*----------------------------------------------------------------------------*/
/* Appends a description of the ProjectedExpr into a buffer                   */
/*----------------------------------------------------------------------------*/
void ProjectedExpr::vDescribe(PlanNode* pNode, s2K& sBuf)
{
  _static_

  ASSERT(bValidate(pNode));

  QueryNode* pProjectedExpr = static_cast<QueryNode*>(pNode);

  QueryNode* pExprNode = static_cast<QueryNode*>(GraphNav::pFindProperty(C_EXPR_PROP, pProjectedExpr));
  QueryNode* pAliasNode  = static_cast<QueryNode*>(GraphNav::pFindProperty(C_ALIAS_PROP, pProjectedExpr));

  ASSERT(pExprNode);
  ASSERT(pAliasNode);

  Stz* szAliasName = pAliasNode->getStringValue();

  // Describe the expression
  s2K sExprBuf;
  Expr::vDescribe(pExprNode, sExprBuf);

  sBuf.vSprintCat("Alias: %s ; Expr: %s"_Stz, (szAliasName == nullptr ? "<none>"_Stz : szAliasName), sExprBuf.szStz());
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the ProjectedExpr definition*/
/*----------------------------------------------------------------------------*/
b1 ProjectedExpr::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_PROJECTEDEXPR_DEFN);
}



/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
