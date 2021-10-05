/*============================================================================*/
/* SubqueryRefExpr.hpp - an expression node that references a subquery        */
/*============================================================================*/

#include "BASE.h"
#include "SubqueryRefExpr.hpp"

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


#include "GraphNav.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_DEFN = "/__root__/definitions/object/expr/subqueryRefExpr"_Stz;

static Stz* C_TABLEALIAS_PROP = "tableAlias"_Stz;


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Helper function to create this node.                                       */
/*----------------------------------------------------------------------------*/
QueryNode* SubqueryRefExpr::pCreate(Stz* szSubqueryAlias)
{
  _static_

  ASSERT(szSubqueryAlias);

  // Create a unique name for this object
  s256 sName;
  NodeNamer::vGenerateName(C_DEFN, sName);

  QueryNode* pSubqueryRefExpr = new QueryNode(sName, C_DEFN);

  // Add properties

  // The table alias
  QueryNode* pTableAliasNode = StringObject::pCreate(szSubqueryAlias);
  pTableAliasNode->vSetName(C_TABLEALIAS_PROP);
  pSubqueryRefExpr->vAppendProperty(pTableAliasNode);

  return pSubqueryRefExpr;
}

/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void SubqueryRefExpr::vDescribe(PlanNode* pNode, s2K& sBuf)
{
  _static_
  ASSERT(bIsOfType(pNode));

  sBuf.vConcat(szTableAlias(_pQueryNode(pNode)));
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the expr definition         */
/*----------------------------------------------------------------------------*/
b1 SubqueryRefExpr::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 SubqueryRefExpr::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns the resulting type of the expression operation                     */
/*----------------------------------------------------------------------------*/
eCT SubqueryRefExpr::eType(QueryNode* pNode, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));
  ASSERT(pRootNode);
  ASSERT(ShadowRoot::bIsOfType(pRootNode));

  QueryNode* pCteDef = pDefnNode(pNode, pRootNode);
  ASSERT(pCteDef);
  ASSERT(CteDef::bIsOfType(pCteDef));

  ASSERT(CteDef::nColumns(pCteDef) == 1);
  QueryNode* pColumnDef = CteDef::pFirstColumnDef(pCteDef);
  ASSERT(pColumnDef);
  ASSERT(ColumnDef::bIsOfType(pColumnDef));

  eCT eType = ColumnDef::eColumnType(pColumnDef);
  ASSERT(eType > CT_NONE);
  ASSERT(eType < CT_ENUMS);

  return eType;
}

/*----------------------------------------------------------------------------*/
/* Return the alias that this subquery is known as                            */
/*----------------------------------------------------------------------------*/
Stz* SubqueryRefExpr::szTableAlias(QueryNode *pNode)
{
  ASSERT(pNode);
  ASSERT(bIsOfType(pNode));

  PlanNode* pAliasNode = GraphNav::pFindProperty(const_cast<Stz*>(C_TABLEALIAS_PROP), pNode);
  if (!pAliasNode) {
    return nullptr;
  }

  ASSERT(StringObject::bIsOfType(pAliasNode));
  return pAliasNode->szValue();
}

/*----------------------------------------------------------------------------*/
/* Return the node that this SubqueryRefExpr is referencing                   */
/*----------------------------------------------------------------------------*/
QueryNode* SubqueryRefExpr::pSubquery(QueryNode* pNode, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));
  ASSERT(bIsOfType(pNode));
  ASSERT(pRootNode);
  ASSERT(ShadowRoot::bIsOfType(pRootNode));

  Stz* szAlias = szTableAlias(pNode);

  // Previously we tried to find the path of the current node (pNode) in the graph,
  // then find the enclosing MainSubquery and look for the ExprSubquery among its
  // siblings. Thie approach doesn't work when pNode is not actually in the graph
  // because it is a copy of a node that is in the graph (e.g. from calling soemthing
  // like PlanObject::pColumnExprs()).
  //
  // Instead, expression Subquery names are unique, so we can search the graph children
  // for a node that matches this. We don't need to search properties, so this should be
  // fairly quick.

  auto fSearch = [szAlias](PlanNode* pItem, void* pvArg) -> b1 {
    b1 bFound = (ExprSubquery::bIsOfType(pItem) && ExprSubquery::szTableAlias(_pQueryNode(pItem))->bSame(szAlias));
    return bFound;
  };

  QueryNode* pPlanRoot = ShadowRoot::pPlanRoot(pRootNode);
  QueryNode* pExprSubqueryNode = _pQueryNode(GraphNav::pGlobalSearch(pPlanRoot, false, true, fSearch, nullptr));

  ASSERT(pExprSubqueryNode);
  ASSERT(ExprSubquery::bIsOfType(pExprSubqueryNode));
  ASSERT(ExprSubquery::szTableAlias(pExprSubqueryNode)->bSame(szAlias));

  return pExprSubqueryNode;
}

/*----------------------------------------------------------------------------*/
/* Return the CteDef node that represents this subquery's definition.         */
/* If psPathBuf is not null then the path of the CteDef is written into it.   */
/*----------------------------------------------------------------------------*/
QueryNode* SubqueryRefExpr::pDefnNode(QueryNode* pNode, PlanNode* pRootNode, s2K* psPathBuf)
{
  _static_
  ASSERT(bIsOfType(pNode));
  ASSERT(pRootNode);
  ASSERT(ShadowRoot::bIsOfType(pRootNode));

  QueryNode* pExprSubquery = pSubquery(pNode, pRootNode);
  ASSERT(pExprSubquery);
  ASSERT(ExprSubquery::bIsOfType(pExprSubquery));

  Stz* szDefnPath = ExprSubquery::szDefnPath(pExprSubquery);
  ASSERT(szDefnPath);

  if (psPathBuf) {
    psPathBuf->vCopy(szDefnPath);
  }

  QueryNode* pCteDef = _pQueryNode(GraphNav::pFind(szDefnPath, pRootNode));
  ASSERT(pCteDef);
  ASSERT(CteDef::bIsOfType(pCteDef));

  return pCteDef;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
