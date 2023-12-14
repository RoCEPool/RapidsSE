/*============================================================================*/
/* OrExpr.hpp - a logical OR expression operator                              */
/*============================================================================*/

#include "BASE.h"
#include "OrExpr.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "NodeNamer.hpp"
#include "SubqueryRefExpr.hpp"
#include "TypeSystem.hpp"
#include "TypeMap2D.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_OR_NAME_PREFIX = "OrExpr"_Stz;
static Stz* C_OR_DEFN = "/__root__/definitions/object/expr/operator/logic/or"_Stz;

// What is the resulting type when we combine these inputs?
//  - The first parameter represents the type of the first operand
//  - The second parameter represents the type of the second operand
//  - The third parameter represents the type of the result from this operation.
//  - Any unspecified mappings are unsupported.
//  - Multiple mappings may be made in one go using the eTM values to represent a set of types.
static TypeMap2D* S_pTypeMap = (new TypeMap2D())
    ->pAddMapping(TypeId::xIdOf(CT_BOOLEAN),  TM_SAME,                      TypeId::xIdOf(CT_BOOLEAN))
    ->pAddMapping(TypeId::xIdOf(CT_BOOLEAN),  TypeId::xIdOf(CT_UNKNOWN), TypeId::xIdOf(CT_BOOLEAN))
    ->pAddMapping(TypeId::xIdOf(CT_UNKNOWN),  TypeId::xIdOf(CT_BOOLEAN), TypeId::xIdOf(CT_BOOLEAN))
    ->pAddMapping(TypeId::xIdOf(CT_UNKNOWN),  TM_SAME,                      TypeId::xIdOf(CT_BOOLEAN))
;

// References:
// OR of a NULL literal will produce a NULL *value*, but the output type will still be a boolean.


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Create the necessary plan graph nodes to represent this object             */
/*----------------------------------------------------------------------------*/
QueryNode* OrExpr::pCreate(QueryNode* pExpr1, QueryNode* pExpr2)
{
  _static_

  // Create a unique name for this object
  s256 sName;
  NodeNamer::vGenerateName(C_OR_DEFN, sName);

  QueryNode* pExpr = new QueryNode(sName, C_OR_DEFN);
  pExpr->vAppendChild(pExpr1);
  pExpr->vAppendChild(pExpr2);

  return pExpr;
}


/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void OrExpr::vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));

  QueryNode* pChild1 = OrExpr::pChild1(_pQueryNode(pNode));
  QueryNode* pChild2 = OrExpr::pChild2(_pQueryNode(pNode));

  if (pChild1->nChildren() == 0 || SubqueryRefExpr::bIsOfType(pChild1)) {
    Expr::vDescribe(pChild1, sBuf, pRootNode);

  } else {
    sBuf.vConcat("("_Stz);
    Expr::vDescribe(pChild1, sBuf, pRootNode);
    sBuf.vConcat(")"_Stz);
  }

  sBuf.vConcat(" OR "_Stz);

  if (pChild2->nChildren() == 0 || SubqueryRefExpr::bIsOfType(pChild2)) {
    Expr::vDescribe(pChild2, sBuf, pRootNode);

  } else {
    sBuf.vConcat("("_Stz);
    Expr::vDescribe(pChild2, sBuf, pRootNode);
    sBuf.vConcat(")"_Stz);
  }
}

/*----------------------------------------------------------------------------*/
/* Represent the expression as types instead of names. Append to the buffer   */
/*----------------------------------------------------------------------------*/
void OrExpr::vDescribeAsTypes(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));

  QueryNode* pChild1 = OrExpr::pChild1(_pQueryNode(pNode));
  QueryNode* pChild2 = OrExpr::pChild2(_pQueryNode(pNode));

  sBuf.vConcat(Expr::xTypeId(pChild1, pRootNode).szTypeName());
  sBuf.vConcat(" OR "_Stz);
  sBuf.vConcat(Expr::xTypeId(pChild2, pRootNode).szTypeName());
}

/*----------------------------------------------------------------------------*/
/* Return the first child expression of this operator                         */
/*----------------------------------------------------------------------------*/
QueryNode* OrExpr::pChild1(QueryNode *pNode)
{
  _static_
  ASSERT(bIsOfType(pNode));
  ASSERT(pNode->nChildren() == 2);

  return _pQueryNode(pNode->pFirstChild());
}

/*----------------------------------------------------------------------------*/
/* Return the second child expression of this operator                         */
/*----------------------------------------------------------------------------*/
QueryNode* OrExpr::pChild2(QueryNode *pNode)
{
  _static_
  ASSERT(bIsOfType(pNode));
  ASSERT(pNode->nChildren() == 2);

  return _pQueryNode(pNode->pSecondChild());
}


/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the expr definition         */
/*----------------------------------------------------------------------------*/
b1 OrExpr::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_OR_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 OrExpr::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_OR_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns the resulting type of the expression operation.                    */
/* A returned value of CT_NONE means that it is not supported or invalid.     */
/*----------------------------------------------------------------------------*/
TypeId OrExpr::xTypeId(QueryNode* pExprNode, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pExprNode));

  // Get the types of the two operands
  QueryNode* pChild1 = OrExpr::pChild1(pExprNode);
  QueryNode* pChild2 = OrExpr::pChild2(pExprNode);

  TypeId xType1 = Expr::xTypeId(pChild1, pRootNode);
  TypeId xType2 = Expr::xTypeId(pChild2, pRootNode);

  // Lookup the type of the result
  TypeId xResult = S_pTypeMap->xLookup(xType1, xType2);
  return xResult;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
