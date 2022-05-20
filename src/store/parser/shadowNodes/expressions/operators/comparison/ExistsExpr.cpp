/*============================================================================*/
/* ExistsExpr.hpp - an EXISTS expression operator                             */
/*============================================================================*/

#include "BASE.h"
#include "ExistsExpr.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "NodeNamer.hpp"
#include "SubqueryRefExpr.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_NAME_PREFIX = "existsExpr"_Stz;
static Stz* C_DEFN = "/__root__/definitions/object/expr/operator/comparison/exists"_Stz;


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

QueryNode* ExistsExpr::pCreate(QueryNode* pSubqueryRef)
{
  _static_
  ASSERT(pSubqueryRef);
  ASSERT(SubqueryRefExpr::bIsOfType(pSubqueryRef));

  // Create a unique name for this object
  s256 sName;
  NodeNamer::vGenerateName(C_DEFN, sName);

  QueryNode* pExistsExpr = new QueryNode(sName, C_DEFN);
  pExistsExpr->vAppendChild(pSubqueryRef);

  return pExistsExpr;
}


/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void ExistsExpr::vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));
  ASSERT(pNode->nChildren() == 1);

  QueryNode* pSubqueryRef = _pQueryNode(pNode->pFirstChild());
  ASSERT(SubqueryRefExpr::bIsOfType(pSubqueryRef));

  s2K sSubqueryBuf;
  Expr::vDescribe(pSubqueryRef, sSubqueryBuf, pRootNode);

  sBuf.vCatSprint("EXISTS %s"_Stz, sSubqueryBuf.szStz());
}


/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the expr definition         */
/*----------------------------------------------------------------------------*/
b1 ExistsExpr::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 ExistsExpr::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns the resulting type of the expression operation.                    */
/* A returned value of CT_NONE means that it is not supported or invalid.     */
/*----------------------------------------------------------------------------*/
TypeId ExistsExpr::xTypeId(QueryNode* pExprNode, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pExprNode));

  // EXISTS clauses always return a boolean
  return TypeId::xIdOf(CT_BOOLEAN);
}



/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
