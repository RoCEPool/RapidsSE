/*============================================================================*/
/* SubqueryExpr.hpp - an expression node that wraps a subquery                */
/*============================================================================*/

#include "BASE.h"
#include "SubqueryExpr.hpp"

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
#include "SubqueryContainer.hpp"


#include "GraphNav.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_DEFN = "/__root__/definitions/object/expr/subqueryExpr"_Stz;

static Stz* C_DEFNPATH_PROP = "defnPath"_Stz;
static Stz* C_CORRELATED_EXPRS_PROP = "correlatedExprs"_Stz;
static Stz* C_SUBQUERYCONTAINER_PROP = "subqueryContainer"_Stz;


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Helper function to create expression column reference nodes.               */
/*----------------------------------------------------------------------------*/
QueryNode* SubqueryExpr::pCreate(QueryNode* pSubqueryContainer)
{
  _static_

  ASSERT(pSubqueryContainer);
  ASSERT(SubqueryContainer::bIsOfType(pSubqueryContainer));
  ASSERT(pSubqueryContainer->nChildren() > 0);

  // Create a unique name for this object
  s256 sName;
  NodeNamer::vGenerateName(C_DEFN, sName);

  QueryNode* pSubqueryExpr = new QueryNode(sName, C_DEFN);

  // Add properties

  // A null definition path
  QueryNode* pDefnPathNode = StringObject::pCreate(nullptr);
  pDefnPathNode->vSetName(C_DEFNPATH_PROP);
  pSubqueryExpr->vAppendProperty(pDefnPathNode);

  // An empty correlated columns list
  QueryNode* pCorrelatedExprs = ExprList::pCreate(C_CORRELATED_EXPRS_PROP);
  pSubqueryExpr->vAppendProperty(pCorrelatedExprs);

  // Add the subquery container
  pSubqueryContainer->vSetName(C_SUBQUERYCONTAINER_PROP);
  pSubqueryExpr->vAppendProperty(pSubqueryContainer);

  return pSubqueryExpr;
}

/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void SubqueryExpr::vDescribe(PlanNode* pNode, s2K& sBuf)
{
  _static_
  ASSERT(bIsOfType(pNode));

  b1 bCorrelated = bIsCorrelated(_pQueryNode(pNode));

  sBuf.vCatSprint("%s expression subquery"_Stz, bCorrelated ? "Correlated"_Stz : "Uncorrelated"_Stz);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the expr definition         */
/*----------------------------------------------------------------------------*/
b1 SubqueryExpr::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 SubqueryExpr::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns the resulting type of the expression operation                     */
/*----------------------------------------------------------------------------*/
eCT SubqueryExpr::eType(QueryNode* pNode, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));
  ASSERT(pRootNode);
  ASSERT(ShadowRoot::bIsOfType(pRootNode));

  Stz* szDefnPath = SubqueryExpr::szDefnPath(pNode);
  ASSERT(szDefnPath);

  QueryNode* pCteDef = _pQueryNode(GraphNav::pFind(szDefnPath, pRootNode));
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
/* Returns the path where the definition of this subquery is defined          */
/*----------------------------------------------------------------------------*/
Stz* SubqueryExpr::szDefnPath(QueryNode* pNode)
{
  QueryNode *pDefnPathNode = static_cast<QueryNode *>(
      GraphNav::pFindProperty(C_DEFNPATH_PROP, pNode));
  ASSERT(pDefnPathNode);
  ASSERT(StringObject::bIsOfType(pDefnPathNode));

  return pDefnPathNode->szValue();
}

/*----------------------------------------------------------------------------*/
/* Sets the path of the node that represents the metadata of this CTE         */
/*----------------------------------------------------------------------------*/
void SubqueryExpr::vSetDefnPath(QueryNode *pNode, Stz *szPath)
{
  ASSERT(pNode);
  ASSERT(bIsOfType(pNode));
  ASSERT(szPath);

  QueryNode* pDefnPathNode = _pQueryNode(GraphNav::pFindProperty(const_cast<Stz*>(C_DEFNPATH_PROP), pNode));
  ASSERT(pDefnPathNode);
  ASSERT(StringObject::bIsOfType(pDefnPathNode));
  ASSERT(pDefnPathNode->xValue().m_bIsNull);

  Value xVal(szPath);
  pDefnPathNode->vSetValue(static_cast<Value&&>(szPath));
}

/*----------------------------------------------------------------------------*/
/* Returns true if this subquery is correlated or false if it is not.         */
/*----------------------------------------------------------------------------*/
b1 SubqueryExpr::bIsCorrelated(QueryNode* pNode)
{
  QueryNode *pCorrelatedExprs = static_cast<QueryNode *>(
      GraphNav::pFindProperty(C_CORRELATED_EXPRS_PROP, pNode));

  ASSERT(pCorrelatedExprs);
  ASSERT(ExprList::bIsOfType(pCorrelatedExprs));

  return (pCorrelatedExprs->nChildren() > 0);
}

/*----------------------------------------------------------------------------*/
/* Returns an iterator of the correlated expressions in this subquery.        */
/* Iterator must be freed after use.                                          */
/*----------------------------------------------------------------------------*/
PlanNodeIter* SubqueryExpr::pCorrelatedExprsIter(QueryNode* pNode)
{
  QueryNode *pCorrelatedExprs = static_cast<QueryNode *>(
      GraphNav::pFindProperty(C_CORRELATED_EXPRS_PROP, pNode));

  ASSERT(pCorrelatedExprs);
  ASSERT(ExprList::bIsOfType(pCorrelatedExprs));

  return pCorrelatedExprs->pNewChildrenIter();
}

/*----------------------------------------------------------------------------*/
/* Return the node that is the root of the subquery plan graph                */
/*----------------------------------------------------------------------------*/
QueryNode* SubqueryExpr::pSubqueryContainer(QueryNode *pNode)
{
  ASSERT(pNode);
  ASSERT(bIsOfType(pNode));

  QueryNode *pSubqueryContainer = static_cast<QueryNode *>(
      GraphNav::pFindProperty(C_SUBQUERYCONTAINER_PROP, pNode));

  ASSERT(pSubqueryContainer);
  ASSERT(SubqueryContainer::bIsOfType(pSubqueryContainer));

  return pSubqueryContainer;
}

/*----------------------------------------------------------------------------*/
/* Return the name of the property that is the root of the subquery plan graph*/
/*----------------------------------------------------------------------------*/
Stz* SubqueryExpr::szSubqueryContainerProp(void)
{
  return C_SUBQUERYCONTAINER_PROP;
}

/*----------------------------------------------------------------------------*/
/* Find and return the main subquery node that exists underneath this         */
/* expression subquery node.                                                  */
/*----------------------------------------------------------------------------*/
QueryNode* SubqueryExpr::pChildMainSubquery(QueryNode *pNode)
{
  ASSERT(pNode);
  ASSERT(bIsOfType(pNode));

  QueryNode* pSubqueryContainer = SubqueryExpr::pSubqueryContainer(pNode);
  ASSERT(pSubqueryContainer);
  ASSERT(SubqueryContainer::bIsOfType(pSubqueryContainer));

  QueryNode* pMainSubquery = SubqueryContainer::pMainSubquery(pSubqueryContainer);
  ASSERT(pMainSubquery);
  ASSERT(MainSubquery::bIsOfType(pMainSubquery));

  return pMainSubquery;
}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
