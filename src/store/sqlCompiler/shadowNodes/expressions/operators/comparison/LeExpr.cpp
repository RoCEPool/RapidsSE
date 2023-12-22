/*============================================================================*/
/* LeExpr.hpp - a greater-than (>) expression operator                        */
/*============================================================================*/

#include "BASE.h"
#include "LeExpr.hpp"

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
static Stz* C_LE_NAME_PREFIX = "LeExpr"_Stz;
static Stz* C_LE_DEFN = "/__root__/definitions/object/expr/operator/comparison/le"_Stz;

// What is the resulting type when we combine these inputs?
//  - The first parameter represents the type of the first operand
//  - The second parameter represents the type of the second operand
//  - The third parameter represents the type of the result from this operation.
//  - Any unspecified mappings are unsupported.
//  - Multiple mappings may be made in one go using the eTM values to represent a set of types.
static TypeMap2D* S_pTypeMap = (new TypeMap2D())
    ->pAddMapping(TypeId::xIdOf(CT_BOOLEAN),  TM_SAME,        TypeId::xIdOf(CT_BOOLEAN))
    ->pAddMapping(TypeId::xIdOf(CT_CHAR),     TM_ALL_STRINGS, TypeId::xIdOf(CT_BOOLEAN))
    ->pAddMapping(TypeId::xIdOf(CT_DATE),     TM_SAME,        TypeId::xIdOf(CT_BOOLEAN))
    ->pAddMapping(TypeId::xIdOf(CT_DECIMAL),  TM_SAME,        TypeId::xIdOf(CT_BOOLEAN))
    ->pAddMapping(TypeId::xIdOf(CT_FLOAT),    TM_SAME,        TypeId::xIdOf(CT_BOOLEAN))
    ->pAddMapping(TypeId::xIdOf(CT_INTEGER),  TM_SAME,        TypeId::xIdOf(CT_BOOLEAN))
    ->pAddMapping(TypeId::xIdOf(CT_TIME),     TM_SAME,        TypeId::xIdOf(CT_BOOLEAN))
    ->pAddMapping(TypeId::xIdOf(CT_TIMESTAMP),TM_SAME,        TypeId::xIdOf(CT_BOOLEAN))
    ->pAddMapping(TypeId::xIdOf(CT_VARCHAR),  TM_ALL_STRINGS, TypeId::xIdOf(CT_BOOLEAN))

        // All YM intervals are comparable with each other.
        // Refer [1] below.
    ->pAddMapping( TypeId::xIdOf(IV_YEAR),          TM_ALL_YM_INTERVALS, TypeId::xIdOf(CT_BOOLEAN))
    ->pAddMapping( TypeId::xIdOf(IV_MONTH),         TM_ALL_YM_INTERVALS, TypeId::xIdOf(CT_BOOLEAN))
    ->pAddMapping( TypeId::xIdOf(IV_YEAR_TO_MONTH), TM_ALL_YM_INTERVALS, TypeId::xIdOf(CT_BOOLEAN))

        // All DT intervals are comparable with each other.
        // Refer [1] below.
    ->pAddMapping( TypeId::xIdOf(IV_DAY),              TM_ALL_DT_INTERVALS,  TypeId::xIdOf(CT_BOOLEAN))
    ->pAddMapping( TypeId::xIdOf(IV_HOUR),             TM_ALL_DT_INTERVALS,  TypeId::xIdOf(CT_BOOLEAN))
    ->pAddMapping( TypeId::xIdOf(IV_MINUTE),           TM_ALL_DT_INTERVALS,  TypeId::xIdOf(CT_BOOLEAN))
    ->pAddMapping( TypeId::xIdOf(IV_SECOND),           TM_ALL_DT_INTERVALS,  TypeId::xIdOf(CT_BOOLEAN))
    ->pAddMapping( TypeId::xIdOf(IV_DAY_TO_HOUR),      TM_ALL_DT_INTERVALS,  TypeId::xIdOf(CT_BOOLEAN))
    ->pAddMapping( TypeId::xIdOf(IV_DAY_TO_MINUTE),    TM_ALL_DT_INTERVALS,  TypeId::xIdOf(CT_BOOLEAN))
    ->pAddMapping( TypeId::xIdOf(IV_DAY_TO_SECOND),    TM_ALL_DT_INTERVALS,  TypeId::xIdOf(CT_BOOLEAN))
    ->pAddMapping( TypeId::xIdOf(IV_HOUR_TO_MINUTE),   TM_ALL_DT_INTERVALS,  TypeId::xIdOf(CT_BOOLEAN))
    ->pAddMapping( TypeId::xIdOf(IV_HOUR_TO_SECOND),   TM_ALL_DT_INTERVALS,  TypeId::xIdOf(CT_BOOLEAN))
    ->pAddMapping( TypeId::xIdOf(IV_MINUTE_TO_SECOND), TM_ALL_DT_INTERVALS,  TypeId::xIdOf(CT_BOOLEAN))

        // All the unknown mappings here
    ->pAddMapping(TypeId::xIdOf(CT_UNKNOWN),  TM_ALL_INC_UNKNOWN,            TypeId::xIdOf(CT_BOOLEAN))
    ->pAddMapping(TM_ALL_INC_UNKNOWN,              TypeId::xIdOf(CT_UNKNOWN),TypeId::xIdOf(CT_BOOLEAN))
;

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

QueryNode* LeExpr::pCreate(QueryNode* pExpr1, QueryNode* pExpr2)
{
  _static_

  // Create a unique name for this object
  s256 sName;
  NodeNamer::vGenerateName(C_LE_DEFN, sName);

  QueryNode* pExpr = new QueryNode(sName, C_LE_DEFN);
  pExpr->vAppendChild(pExpr1);
  pExpr->vAppendChild(pExpr2);

  return pExpr;
}


/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void LeExpr::vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));

  QueryNode* pChild1 = LeExpr::pChild1(_pQueryNode(pNode));
  QueryNode* pChild2 = LeExpr::pChild2(_pQueryNode(pNode));

  if (pChild1->nChildren() == 0 || SubqueryRefExpr::bIsOfType(pChild1)) {
    Expr::vDescribe(pChild1, sBuf, pRootNode);

  } else {
    sBuf.vConcat("("_Stz);
    Expr::vDescribe(pChild1, sBuf, pRootNode);
    sBuf.vConcat(")"_Stz);
  }

  sBuf.vConcat(" <= "_Stz);

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
void LeExpr::vDescribeAsTypes(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));

  QueryNode* pChild1 = LeExpr::pChild1(_pQueryNode(pNode));
  QueryNode* pChild2 = LeExpr::pChild2(_pQueryNode(pNode));

  sBuf.vConcat(Expr::xTypeId(pChild1, pRootNode).szTypeName());
  sBuf.vConcat(" <= "_Stz);
  sBuf.vConcat(Expr::xTypeId(pChild2, pRootNode).szTypeName());
}

/*----------------------------------------------------------------------------*/
/* Return the first child expression of this operator                         */
/*----------------------------------------------------------------------------*/
QueryNode* LeExpr::pChild1(QueryNode *pNode)
{
  _static_
  ASSERT(bIsOfType(pNode));
  ASSERT(pNode->nChildren() == 2);

  return _pQueryNode(pNode->pFirstChild());
}

/*----------------------------------------------------------------------------*/
/* Return the second child expression of this operator                         */
/*----------------------------------------------------------------------------*/
QueryNode* LeExpr::pChild2(QueryNode *pNode)
{
  _static_
  ASSERT(bIsOfType(pNode));
  ASSERT(pNode->nChildren() == 2);

  return _pQueryNode(pNode->pSecondChild());
}


/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the expr definition         */
/*----------------------------------------------------------------------------*/
b1 LeExpr::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_LE_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 LeExpr::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_LE_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns the resulting type of the expression operation.                    */
/* A returned value of CT_NONE means that it is not supported or invalid.     */
/*----------------------------------------------------------------------------*/
TypeId LeExpr::xTypeId(QueryNode* pExprNode, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pExprNode));

  // Get the types of the two operands
  QueryNode* pChild1 = LeExpr::pChild1(pExprNode);
  QueryNode* pChild2 = LeExpr::pChild2(pExprNode);

  TypeId xType1 = Expr::xTypeId(pChild1, pRootNode);
  TypeId xType2 = Expr::xTypeId(pChild2, pRootNode);

  // Lookup the type of the result
  TypeId xResult = S_pTypeMap->xLookup(xType1, xType2);
  return xResult;
}

/*----------------------------------------------------------------------------*/
/* Returns the precision of the resulting expression or -1 if not set or n/a  */
/*----------------------------------------------------------------------------*/
i8 LeExpr::iPrecision(QueryNode* pExprNode, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pExprNode));
  ASSERT(pRootNode);
  UNUSED(pExprNode);
  UNUSED(pRootNode);

  // Precision returned is always n/a
  return -1;
}

/*----------------------------------------------------------------------------*/
/* Returns the scale of the resulting expression or -1 if not set or n/a      */
/*----------------------------------------------------------------------------*/
i8 LeExpr::iScale(QueryNode* pExprNode, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pExprNode));
  ASSERT(pRootNode);
  UNUSED(pExprNode);
  UNUSED(pRootNode);

  // Scale returned is always n/a
  return -1;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
