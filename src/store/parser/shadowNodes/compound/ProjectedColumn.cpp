/*============================================================================*/
/* ProjectedColumn.hpp - an expression that may also have an alias            */
/*============================================================================*/

#include "BASE.h"
#include "ProjectedColumn.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "PlanConstants.hpp"
#include "Expr.hpp"

#include "GraphNav.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_PROJECTEDCOLUMN_DEFN = "/__root__/definitions/object/compound/projectedColumn"_Stz;
static Stz* C_PROJECTEDCOLUMN_NAME = "projectedColumn"_Stz;
static Stz* C_EXPR_PROP  = "expr"_Stz;
static Stz* C_ALIAS_PROP = "alias"_Stz;


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Returns the aliasName of the projectedColumn                               */
/*----------------------------------------------------------------------------*/
Stz* ProjectedColumn::szAliasName(QueryNode* pNode)
{
  QueryNode *pAliasNode =
      _pQueryNode(GraphNav::pFindProperty(const_cast<Stz *>(C_ALIAS_PROP), pNode));

  return pAliasNode->szValue();;
}

/*----------------------------------------------------------------------------*/
/* Helper function to create ProjectedColumn nodes.                           */
/*----------------------------------------------------------------------------*/
QueryNode* ProjectedColumn::pCreate(QueryNode* pExprNode, Text* pAlias)
{
  _static_

  ASSERT(pExprNode);
  ASSERT(!pAlias || pAlias->nLength() > 0);

  Value xAliasValue(pAlias);

  s256 sName;
  ShadowNode::vObjName(C_PROJECTEDCOLUMN_NAME, sName);

  // Create the ProjectedColumn object
  QueryNode* pProjectedColumn = new QueryNode(sName.szStz(), const_cast<Stz*>(C_PROJECTEDCOLUMN_DEFN));

  // Add the expression as a property
  // Make sure it has the right property name first
  pExprNode->vSetName(C_EXPR_PROP);
  pProjectedColumn->vAppendProperty(pExprNode);

  // Create the alias and append it as a property
  QueryNode* pAliasNode  = new QueryNode(C_ALIAS_PROP, const_cast<Stz*>(C_STRING_OBJ_DEFN), static_cast<Value&&>(xAliasValue));
  pProjectedColumn->vAppendProperty(pAliasNode);

  return pProjectedColumn;
}

/*----------------------------------------------------------------------------*/
/* Appends a description of the graph node into a buffer                      */
/*----------------------------------------------------------------------------*/
void ProjectedColumn::vDescribe(PlanNode* pNode, s2K& sBuf)
{
  _static_

  vDescribe(pNode, sBuf, nullptr, nullptr);
}

/*----------------------------------------------------------------------------*/
/* Appends a description of the graph node into a buffer                      */
/*----------------------------------------------------------------------------*/
void ProjectedColumn::vDescribe(PlanNode* pNode, s2K& sBuf, QueryNodeList* pColumnExprList, PlanNode* pRootNode)
{
  _static_

  ASSERT(bIsOfType(pNode));

  QueryNode* pProjectedColumn = _pQueryNode(pNode);

  QueryNode* pExprNode = _pQueryNode(GraphNav::pFindProperty(C_EXPR_PROP, pProjectedColumn));
  QueryNode* pAliasNode  = _pQueryNode(GraphNav::pFindProperty(C_ALIAS_PROP, pProjectedColumn));

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
/* Returns true if the given node is derived from the ProjectedColumn defn    */
/*----------------------------------------------------------------------------*/
b1 ProjectedColumn::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_PROJECTEDCOLUMN_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 ProjectedColumn::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_PROJECTEDCOLUMN_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the expression in this node contains an aggregate expression*/
/*----------------------------------------------------------------------------*/
b1 ProjectedColumn::bContainsAnAggregate(QueryNode* pNode)
{
  QueryNode *pExprNode = ProjectedColumn::pExprNode(pNode);

  return Expr::bContainsAnAggregate(pExprNode);
}

/*----------------------------------------------------------------------------*/
/* Returns the exprNode of the ProjectedColumn                                */
/*----------------------------------------------------------------------------*/
QueryNode* ProjectedColumn::pExprNode(QueryNode* pNode)
{
  QueryNode *pExprNode =
      _pQueryNode(GraphNav::pFindProperty(const_cast<Stz *>(C_EXPR_PROP), pNode));

  return pExprNode;
}

/*----------------------------------------------------------------------------*/
/* Returns a new copy of this projected expression rewritten relative to      */
/* column ordinals. Must be freed after use.                                  */
/*----------------------------------------------------------------------------*/
QueryNode* ProjectedColumn::pSymbolicToOrdinal(QueryNode*     pNode,
                                               QueryNodeList* pColExprList,
                                               PlanNode*      pRootNode)
{
  // Get the expression out of this ProjectedColumn and convert it
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
  if (ProjectedColumn::szAliasName(pNode)) {
    pAlias = new Text(ProjectedColumn::szAliasName(pNode));
  }

  // Create a new ProjectedColumn and return it
  QueryNode* pOrdinalProjectedColumn = ProjectedColumn::pCreate(pOrdinalExpr, pAlias);

  return pOrdinalProjectedColumn;
}

/*----------------------------------------------------------------------------*/
/* Sets or changes the alias for this ProjectedColumn                         */
/*----------------------------------------------------------------------------*/
void ProjectedColumn::vSetAlias(QueryNode* pNode, Text *pNewAlias)
{
  QueryNode *pAliasNode =
      _pQueryNode(GraphNav::pFindProperty(const_cast<Stz *>(C_ALIAS_PROP), pNode));

  Value xVal(pNewAlias);
  pAliasNode->vSetValue(static_cast<Value&&>(xVal));
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
