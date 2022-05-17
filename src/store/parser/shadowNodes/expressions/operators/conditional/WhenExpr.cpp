/*============================================================================*/
/* WhenExpr.hpp - an expr test and result value within a CASE expression      */
/*============================================================================*/

#include "BASE.h"
#include "WhenExpr.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "NodeNamer.hpp"
#include "SubqueryRefExpr.hpp"
#include "GraphEditor.hpp"
#include "ShadowRoot.hpp"
#include "TypeSystem.hpp"

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
i8 WhenExpr::iType(QueryNode* pExprNode, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pExprNode));

  // First check that the predicate doesn't need coercing
  QueryNode* pPredicate = WhenExpr::pPredicateExpr(pExprNode);
  eCT ePredicateType = TypeSystem::eCTVal(Expr::iType(pPredicate, pRootNode));

  if (ePredicateType == CT_NONE) {
    // This will try a coercion
    return TypeSystem::iIdOf(ePredicateType);
  }

  // Get the types of the result expression
  QueryNode* pResult = WhenExpr::pResultExpr(pExprNode);
  eCT eResultType = TypeSystem::eCTVal(Expr::iType(pResult, pRootNode));

  return TypeSystem::iIdOf(eResultType);
}

/*----------------------------------------------------------------------------*/
/* Modifies subexpressions by promoting/casting data types so that they       */
/* are type compatible with this expression. Modifies the expression          */
/* tree in place.                                                             */
/*----------------------------------------------------------------------------*/
void WhenExpr::vCoerceSubExprs(QueryNode* pExpr, PlanNode* pRootNode)
{
  _static_
  ASSERT(pExpr);
  ASSERT(bIsOfType(pExpr));
  ASSERT(pRootNode);
  ASSERT(ShadowRoot::bIsOfType(pRootNode));

  QueryNode* pPredicate = WhenExpr::pPredicateExpr(pExpr);
  QueryNode* pResult    = WhenExpr::pResultExpr(pExpr);

  eCT ePredicateType = TypeSystem::eCTVal(Expr::iType(pPredicate, pRootNode));
  eCT eResultType    = TypeSystem::eCTVal(Expr::iType(pResult, pRootNode));

  if (ePredicateType == CT_NONE) {
    Expr::vCoerceSubExprs(pPredicate, pRootNode);
  }

  if (eResultType == CT_NONE) {
    Expr::vCoerceSubExprs(pResult, pRootNode);
  }
}

/*----------------------------------------------------------------------------*/
/* Tries to coerce the result expr of this WHEN statement to a given type     */
/*----------------------------------------------------------------------------*/
void WhenExpr::vCoerceResultExprTo(eCT eCoercibleTarget, QueryNode* pWhenExpr, PlanNode* pRootNode)
{
  _static_
  ASSERT(pWhenExpr);
  ASSERT(bIsOfType(pWhenExpr));
  ASSERT(pRootNode);
  ASSERT(ShadowRoot::bIsOfType(pRootNode));
  ASSERT(eCoercibleTarget > CT_NONE);
  ASSERT(eCoercibleTarget < CT_ENUMS);

  QueryNode* pResult = WhenExpr::pResultExpr(pWhenExpr);
  QueryNode* pCoercedResult = Expr::pCoerceTo(eCoercibleTarget, pResult, pRootNode);

  ASSERT(pCoercedResult);
  ASSERT(Expr::bIsOfType(pCoercedResult));

  GraphEditor::vReplaceChild(pWhenExpr, pResult, pCoercedResult);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
