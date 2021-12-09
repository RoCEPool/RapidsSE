/*============================================================================*/
/* WhenExpr.hpp - an expr test and result value within a CASE expression      */
/*============================================================================*/

#include "BASE.h"
#include "WhenExpr.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "NodeNamer.hpp"
#include "SubqueryRefExpr.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_NAME_PREFIX = "WhenExpr"_Stz;
static Stz* C_DEFN = "/__root__/definitions/object/expr/operator/conditional/when"_Stz;



/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Create the necessary plan graph nodes to represent this object             */
/*----------------------------------------------------------------------------*/
QueryNode* WhenExpr::pCreate(QueryNode* pTestExpr, QueryNode* pResultExpr)
{
  _static_
  ASSERT(pTestExpr);
  ASSERT(Expr::bIsOfType(pTestExpr));
  ASSERT(pResultExpr);
  ASSERT(Expr::bIsOfType(pResultExpr));

  // Create a unique name for this object
  s256 sName;
  NodeNamer::vGenerateName(C_DEFN, sName);

  QueryNode* pExpr = new QueryNode(sName, C_DEFN);
  pExpr->vAppendChild(pTestExpr);
  pExpr->vAppendChild(pResultExpr);

  return pExpr;
}


/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void WhenExpr::vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));

  QueryNode* pTestExpr = WhenExpr::pPredicateExpr(_pQueryNode(pNode));
  QueryNode* pResultExpr = WhenExpr::pResultExpr(_pQueryNode(pNode));

  sBuf.vConcat("WHEN "_Stz);

  if (pTestExpr->nChildren() == 0 || SubqueryRefExpr::bIsOfType(pTestExpr)) {
    Expr::vDescribe(pTestExpr, sBuf, pRootNode);

  } else {
    sBuf.vConcat("("_Stz);
    Expr::vDescribe(pTestExpr, sBuf, pRootNode);
    sBuf.vConcat(")"_Stz);
  }

  sBuf.vConcat(" THEN "_Stz);


  if (pResultExpr->nChildren() == 0 || SubqueryRefExpr::bIsOfType(pResultExpr)) {
    Expr::vDescribe(pResultExpr, sBuf, pRootNode);

  } else {
    sBuf.vConcat("("_Stz);
    Expr::vDescribe(pResultExpr, sBuf, pRootNode);
    sBuf.vConcat(")"_Stz);
  }
}

/*----------------------------------------------------------------------------*/
/* Retrieve the expression to be tested                                       */
/*----------------------------------------------------------------------------*/
QueryNode* WhenExpr::pPredicateExpr(QueryNode *pNode)
{
  _static_
  ASSERT(bIsOfType(pNode));
  ASSERT(pNode->nChildren() == 2);

  return _pQueryNode(pNode->pFirstChild());
}

/*----------------------------------------------------------------------------*/
/* Retrieve the result expr to be used if the test expr evaluates to true     */
/*----------------------------------------------------------------------------*/
QueryNode* WhenExpr::pResultExpr(QueryNode *pNode)
{
  _static_
  ASSERT(bIsOfType(pNode));
  ASSERT(pNode->nChildren() == 2);

  return _pQueryNode(pNode->pSecondChild());
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the expr definition         */
/*----------------------------------------------------------------------------*/
b1 WhenExpr::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 WhenExpr::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns the resulting type of the expression operation.                    */
/* A returned value of CT_NONE means that it is not supported or invalid.     */
/*----------------------------------------------------------------------------*/
eCT WhenExpr::eType(QueryNode* pExprNode, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pExprNode));

  // Get the types of the result expression
  QueryNode* pResult = WhenExpr::pResultExpr(pExprNode);

  eCT eResultType = Expr::eType(pResult, pRootNode);

  return eResultType;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
