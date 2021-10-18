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
#include "ExprList.hpp"
#include "SqlOperator.hpp"
#include "OrdinalSubqueryRefExpr.hpp"


#include "GraphNav.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_DEFN = "/__root__/definitions/object/expr/subqueryRefExpr"_Stz;

static Stz* C_TABLEALIAS_PROP = "tableAlias"_Stz;
static Stz* C_TABLEALIAS_DEFN = "/__root__/definitions/object/valueObject/stringObject"_Stz;


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
void SubqueryRefExpr::vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));

  sBuf.vConcat(szTableAlias(_pQueryNode(pNode)));

  if (pNode->nChildren() > 0) {
    // Describe it in ordinal representation
    s2K sOrdinalBuf;
    b1 bFirst = true;

    sBuf.vConcat("("_Stz);
    PlanNodeIter* pChildIter = pNode->pNewChildrenIter();

    while (pChildIter->bHasNext()) {
      QueryNode* pChild = _pQueryNode(pChildIter->pNext());
      ASSERT(Expr::bIsOfType(pChild));

      sOrdinalBuf.vReset();
      Expr::vDescribe(pChild, sOrdinalBuf, pRootNode);

      if (bFirst) {
        bFirst = false;
      } else {
        sBuf.vConcat(", "_Stz);
      }

      sBuf.vConcat(sOrdinalBuf.szStz());
    }

    delete pChildIter;
    sBuf.vConcat(")"_Stz);

  } else {
    // Describe it in symbolic representation

    QueryNode* pSubqueryNode = pSubquery(_pQueryNode(pNode), pRootNode);
    ASSERT(pSubqueryNode);
    ASSERT(ExprSubquery::bIsOfType(pSubqueryNode));

    QueryNode* pCorrelatedExprs = SubqueryMarker::pCorrelatedColumnsNode(pSubqueryNode);
    ASSERT(pCorrelatedExprs);
    ASSERT(ExprList::bIsOfType(pCorrelatedExprs));

    s2K sExprBuf;
    ExprList::vDescribe(pCorrelatedExprs, sExprBuf, pRootNode);

    sBuf.vCatSprint("(%s)"_Stz, sExprBuf.szStz());
  }
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

  // In most cases an expression subquery should be projecting only 1 column.
  // (An EXISTS clause is different). So we won't assert that here, but rather
  // explicitly check it in ExprValidation.cpp. Instead, when asked the type of
  // an expression subquery we will simply return the type of the first column.
  QueryNode* pColumnDef = CteDef::pFirstColumnDef(pCteDef);
  ASSERT(pColumnDef);
  ASSERT(ColumnDef::bIsOfType(pColumnDef));

  eCT eType = ColumnDef::eColumnType(pColumnDef);
  ASSERT(eType > CT_NONE);
  ASSERT(eType < CT_ENUMS);

  return eType;
}

/*----------------------------------------------------------------------------*/
/* Returns true if the expression does not contain any column references.     */
/* All expressions are relative column ordinals, operators or literals only.  */
/*----------------------------------------------------------------------------*/
b1 SubqueryRefExpr::bIsFullyOrdinal(QueryNode* pExpr)
{
  ASSERT(pExpr);
  ASSERT(bIsOfType(pExpr));

  if (OrdinalSubqueryRefExpr::bIsOfType(pExpr)) {
    return OrdinalSubqueryRefExpr::bIsFullyOrdinal(pExpr);
  }

  // Otherwise this object is not in an ordinal representation
  return false;
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
/* Returns a list of symbolic parameters that the subquery requires.          */
/* List and contents must be freed.                                           */
/*----------------------------------------------------------------------------*/
QueryNodeList* SubqueryRefExpr::pSubqueryParams(QueryNode* pNode, PlanNode* pRootNode)
{
  ASSERT(pNode);
  ASSERT(bIsOfType(pNode));
  ASSERT(pRootNode);
  ASSERT(ShadowRoot::bIsOfType(pRootNode));

  QueryNode* pSubqueryNode = pSubquery(pNode, pRootNode);
  ASSERT(pSubqueryNode);
  ASSERT(ExprSubquery::bIsOfType(pSubqueryNode));

  QueryNode* pCalleeCorrelatedExprs = ExprSubquery::pCorrelatedColumnsNode(pSubqueryNode);
  ASSERT(pCalleeCorrelatedExprs);
  ASSERT(ExprList::bIsOfType(pCalleeCorrelatedExprs));

  // Make a new list to return
  QueryNodeList* pParamList = new QueryNodeList();

  PlanNodeIter* pIter = pCalleeCorrelatedExprs->pNewChildrenIter();
  while (pIter->bHasNext()) {
    QueryNode* pExpr = _pQueryNode(pIter->pNext());
    ASSERT(Expr::bIsOfType(pExpr));

    pParamList->vAppend(pExpr->pDeepCopy(true));
  }

  delete pIter;
  return pParamList;
}

/*----------------------------------------------------------------------------*/
/* Returns a list of ordinal parameters that the subquery requires.           */
/* List and contents must be freed.                                           */
/*----------------------------------------------------------------------------*/
QueryNodeList* SubqueryRefExpr::pOrdinalSubqueryParams(QueryNode* pNode, QueryNodeList* pProjectedColumns,
                                                       QueryNode* pCallerCorrelatedExprs, PlanNode* pRootNode)
{
  ASSERT(pNode);
  ASSERT(bIsOfType(pNode));
  ASSERT(pRootNode);
  ASSERT(ShadowRoot::bIsOfType(pRootNode));

  // Make a new list to return
  QueryNodeList* pParamList = new QueryNodeList();

  // Lookup the node that represents the expression subquery to be called
  QueryNode* pCalleeSubqueryNode = pSubquery(pNode, pRootNode);
  ASSERT(pCalleeSubqueryNode);
  ASSERT(ExprSubquery::bIsOfType(pCalleeSubqueryNode));

  // Get the list of correlated expressions for the expression subquery that pNode
  // is a proxy for. These are the parameters we need to pass when we call this subquery.
  // This is an internal node - don't delete it.
  QueryNode* pCalleeCorrelatedExprs = SubqueryMarker::pCorrelatedColumnsNode(pCalleeSubqueryNode);
  ASSERT(pCalleeCorrelatedExprs);
  ASSERT(ExprList::bIsOfType(pCalleeCorrelatedExprs));

  if (pCalleeCorrelatedExprs->nChildren() == 0) {
    return pParamList;
  }


  PlanNodeIter* pIter = pCalleeCorrelatedExprs->pNewChildrenIter();
  while (pIter->bHasNext()) {
    QueryNode* pSymbolicExpr = _pQueryNode(pIter->pNext());
    ASSERT(Expr::bIsOfType(pSymbolicExpr));

    // Generate the ordinal representation of this expression, relative to
    // the projected column expression list and any correlated expressions
    // passed to the current subquery.

    QueryNode* pOrdinalExpr = Expr::pSymbolicToOrdinal(pSymbolicExpr, pProjectedColumns, pCallerCorrelatedExprs, pRootNode);
    ASSERT(pOrdinalExpr);

    pParamList->vAppend(pOrdinalExpr);
  }

  delete pIter;

  return pParamList;
}

/*----------------------------------------------------------------------------*/
/* Takes a symbolic expression and returns a copy of it that is relative to   */
/* column ordinals for a given list of column expressions.                    */
/* The returned QueryNode must be freed. Column ordinals are 1-based.         */
/*----------------------------------------------------------------------------*/
QueryNode* SubqueryRefExpr::pSymbolicToOrdinal(QueryNode*     pSymbolicExpr,
                                               QueryNodeList* pColumnExprList,
                                               QueryNode*     pCorrelatedExprs,
                                               PlanNode*      pRootNode)
{
  ASSERT(bIsExactType(pSymbolicExpr));


  // Get the parameters to this subquery in ordinal format
  QueryNodeList* pOrdinalParamList = pOrdinalSubqueryParams(pSymbolicExpr, pColumnExprList, pCorrelatedExprs, pRootNode);

  // Make the ordinal representation of the symbolic expression
  QueryNode* pOrdinalNode = OrdinalSubqueryRefExpr::pCreate(pSymbolicExpr, pOrdinalParamList);
  ASSERT(pOrdinalParamList->nItems() == 0);
  delete pOrdinalParamList;

  return pOrdinalNode;
}

/*----------------------------------------------------------------------------*/
/* Takes an ordinal expression and returns a symbolic copy of it based on the */
/* column expressions projected from a child sql operator.                    */
/* The returned QueryNode must be freed.                                      */
/*----------------------------------------------------------------------------*/
QueryNode* SubqueryRefExpr::pOrdinalToSymbolic(QueryNode*     pOrdinalExpr,
                                               QueryNodeList* pColumnExprList,
                                               PlanNode*      pRootNode)
{
  if (OrdinalSubqueryRefExpr::bIsOfType(pOrdinalExpr)) {
    return OrdinalSubqueryRefExpr::pOrdinalToSymbolic(pOrdinalExpr, pColumnExprList, pRootNode);
  }

  NEVER_REACH
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
