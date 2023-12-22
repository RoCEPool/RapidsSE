/*============================================================================*/
/* ModulusExpr.hpp - an modulus (%) expression operator                       */
/*============================================================================*/

#include "BASE.h"
#include "ModulusExpr.hpp"

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
static Stz* C_MODULUS_NAME_PREFIX = "ModulusExpr"_Stz;
static Stz* C_MODULUS_DEFN = "/__root__/definitions/object/expr/operator/mathematical/modulus"_Stz;

// What is the resulting type when we combine these inputs?
//  - The first parameter represents the type of the first operand
//  - The second parameter represents the type of the second operand
//  - The third parameter represents the type of the result from this operation.
//  - Any unspecified mappings are unsupported.
//  - Multiple mappings may be made in one go using the eTM values to represent a set of types.
static TypeMap2D* S_pTypeMap = (new TypeMap2D())

    ->pAddSelfMapping(TypeId::xIdOf(CT_INTEGER))
    ->pAddMapping(TypeId::xIdOf(CT_INTEGER),        TypeId::xIdOf(CT_UNKNOWN),            TypeId::xIdOf(CT_UNKNOWN))
    ->pAddMapping(TypeId::xIdOf(CT_UNKNOWN),        TypeId::xIdOf(CT_INTEGER),            TypeId::xIdOf(CT_UNKNOWN))
    ->pAddSelfMapping(TypeId::xIdOf(CT_UNKNOWN))
;

// References:
//  Exact numeric % exact numeric = exact numeric
//    - SQL standard 2008 (draft): Part 2 (Foundation), section 4.4.3, page 32


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

QueryNode* ModulusExpr::pCreate(QueryNode* pExpr1, QueryNode* pExpr2)
{
  _static_

  // Create a unique name for this object
  s256 sName;
  NodeNamer::vGenerateName(C_MODULUS_DEFN, sName);

  QueryNode* pExpr = new QueryNode(sName, C_MODULUS_DEFN);
  pExpr->vAppendChild(pExpr1);
  pExpr->vAppendChild(pExpr2);

  return pExpr;
}


/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void ModulusExpr::vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));

  QueryNode* pChild1 = ModulusExpr::pChild1(_pQueryNode(pNode));
  QueryNode* pChild2 = ModulusExpr::pChild2(_pQueryNode(pNode));

  if (pChild1->nChildren() == 0 || SubqueryRefExpr::bIsOfType(pChild1)) {
    Expr::vDescribe(pChild1, sBuf, pRootNode);

  } else {
    sBuf.vConcat("("_Stz);
    Expr::vDescribe(pChild1, sBuf, pRootNode);
    sBuf.vConcat(")"_Stz);
  }

  sBuf.vConcat(" % "_Stz);

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
void ModulusExpr::vDescribeAsTypes(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));

  QueryNode* pChild1 = ModulusExpr::pChild1(_pQueryNode(pNode));
  QueryNode* pChild2 = ModulusExpr::pChild2(_pQueryNode(pNode));

  sBuf.vConcat(Expr::xTypeId(pChild1, pRootNode).szTypeName());
  sBuf.vConcat(" % "_Stz);
  sBuf.vConcat(Expr::xTypeId(pChild2, pRootNode).szTypeName());
}

/*----------------------------------------------------------------------------*/
/* Return the first child expression of this operator                         */
/*----------------------------------------------------------------------------*/
QueryNode* ModulusExpr::pChild1(QueryNode *pNode)
{
  _static_
  ASSERT(bIsOfType(pNode));
  ASSERT(pNode->nChildren() == 2);

  return _pQueryNode(pNode->pFirstChild());
}

/*----------------------------------------------------------------------------*/
/* Return the second child expression of this operator                         */
/*----------------------------------------------------------------------------*/
QueryNode* ModulusExpr::pChild2(QueryNode *pNode)
{
  _static_
  ASSERT(bIsOfType(pNode));
  ASSERT(pNode->nChildren() == 2);

  return _pQueryNode(pNode->pSecondChild());
}


/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the expr definition         */
/*----------------------------------------------------------------------------*/
b1 ModulusExpr::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_MODULUS_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 ModulusExpr::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_MODULUS_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns the resulting type of the expression operation.                    */
/* A returned value of CT_NONE means that it is not supported or invalid.     */
/*----------------------------------------------------------------------------*/
TypeId ModulusExpr::xTypeId(QueryNode* pExprNode, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pExprNode));

  // Get the types of the two operands
  QueryNode* pChild1 = ModulusExpr::pChild1(pExprNode);
  QueryNode* pChild2 = ModulusExpr::pChild2(pExprNode);

  TypeId xType1 = Expr::xTypeId(pChild1, pRootNode);
  TypeId xType2 = Expr::xTypeId(pChild2, pRootNode);

  // Lookup the type of the result
  TypeId xResult = S_pTypeMap->xLookup(xType1, xType2);
  return xResult;
}


/*----------------------------------------------------------------------------*/
/* Returns the precision of the resulting expression or -1 if not set or n/a  */
/*----------------------------------------------------------------------------*/
i8 ModulusExpr::iPrecision(QueryNode* pExprNode, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pExprNode));
  ASSERT(pRootNode);

  TypeId xResultType = xTypeId(pExprNode, pRootNode);

  if (xResultType == TypeId::xIdOf(CT_INTEGER)) {
    return C_MAX_INT_PRECISION;
  }

  return -1;
}

/*----------------------------------------------------------------------------*/
/* Returns the scale of the resulting expression or -1 if not set or n/a      */
/*----------------------------------------------------------------------------*/
i8 ModulusExpr::iScale(QueryNode* pExprNode, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pExprNode));
  ASSERT(pRootNode);
  UNUSED(pExprNode);
  UNUSED(pRootNode);

  return -1;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
