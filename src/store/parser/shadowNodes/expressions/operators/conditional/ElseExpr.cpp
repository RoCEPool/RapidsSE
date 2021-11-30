/*============================================================================*/
/* ElseExpr.hpp - final expr value of a CASE expression                       */
/*============================================================================*/

#include "BASE.h"
#include "ElseExpr.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "NodeNamer.hpp"
#include "SubqueryRefExpr.hpp"
#include "ShadowRoot.hpp"
#include "GraphEditor.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_NAME_PREFIX = "ElseExpr"_Stz;
static Stz* C_DEFN = "/__root__/definitions/object/expr/operator/conditional/else"_Stz;



/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Create the necessary plan graph nodes to represent this object             */
/*----------------------------------------------------------------------------*/
QueryNode* ElseExpr::pCreate(QueryNode* pElseExpr)
{
  _static_
  ASSERT(pElseExpr);
  ASSERT(Expr::bIsOfType(pElseExpr));

  // Create a unique name for this object
  s256 sName;
  NodeNamer::vGenerateName(C_DEFN, sName);

  QueryNode* pExpr = new QueryNode(sName, C_DEFN);
  pExpr->vAppendChild(pElseExpr);

  return pExpr;
}


/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void ElseExpr::vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));

  QueryNode* pChild1 = ElseExpr::pElseExpr(_pQueryNode(pNode));

  sBuf.vConcat("ELSE "_Stz);

  if (pChild1->nChildren() == 0 || SubqueryRefExpr::bIsOfType(pChild1)) {
    Expr::vDescribe(pChild1, sBuf, pRootNode);

  } else {
    sBuf.vConcat("("_Stz);
    Expr::vDescribe(pChild1, sBuf, pRootNode);
    sBuf.vConcat(")"_Stz);
  }
}

/*----------------------------------------------------------------------------*/
/* Return the else value expression of this operator                          */
/*----------------------------------------------------------------------------*/
QueryNode* ElseExpr::pElseExpr(QueryNode *pNode)
{
  _static_
  ASSERT(bIsOfType(pNode));
  ASSERT(pNode->nChildren() == 1);

  return _pQueryNode(pNode->pFirstChild());
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the expr definition         */
/*----------------------------------------------------------------------------*/
b1 ElseExpr::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 ElseExpr::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns the resulting type of the expression operation.                    */
/* A returned value of CT_NONE means that it is not supported or invalid.     */
/*----------------------------------------------------------------------------*/
eCT ElseExpr::eType(QueryNode* pExprNode, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pExprNode));

  // Get the types of the two operands
  QueryNode* pChild1 = ElseExpr::pElseExpr(pExprNode);

  eCT eType1 = Expr::eType(pChild1, pRootNode);

  return eType1;
}

/*----------------------------------------------------------------------------*/
/* Tries to coerce the result expr of this ELSE statement to a given type     */
/*----------------------------------------------------------------------------*/
void ElseExpr::vCoerceResultExprTo(eCT eCoercibleTarget, QueryNode* pElseExpr, PlanNode* pRootNode)
{
  _static_
  ASSERT(pElseExpr);
  ASSERT(bIsOfType(pElseExpr));
  ASSERT(pRootNode);
  ASSERT(ShadowRoot::bIsOfType(pRootNode));
  ASSERT(eCoercibleTarget > CT_NONE);
  ASSERT(eCoercibleTarget < CT_ENUMS);

  QueryNode* pResult = ElseExpr::pElseExpr(pElseExpr);
  QueryNode* pCoercedResult = Expr::pCoerceTo(eCoercibleTarget, pResult, pRootNode);

  ASSERT(pCoercedResult);
  ASSERT(Expr::bIsOfType(pCoercedResult));

  GraphEditor::vReplaceChild(pElseExpr, pResult, pCoercedResult);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
