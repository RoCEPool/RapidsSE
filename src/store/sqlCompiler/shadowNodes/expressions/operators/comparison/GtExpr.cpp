/*============================================================================*/
/* GtExpr.hpp - a greater-than (>) expression operator                        */
/*============================================================================*/

#include "BASE.h"
#include "GtExpr.hpp"

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
static Stz* C_GT_NAME_PREFIX = "GtExpr"_Stz;
static Stz* C_GT_DEFN = "/__root__/definitions/object/expr/operator/comparison/gt"_Stz;

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

// References:
// [1] Comparison of intervals:
//    All subtypes of YM intervals are comparable with each other.
//    All subtypes of DT intervals are comparable with each other.
//    Interval comparison is done by converting each interval into an integer
//    that represents the least significant qualifier of each interval type.
//
//    e.g. if interval A is DAY TO HOUR and inteval B is HOUR TO MINUTE
//    then convert both intervals into integers representing minutes and then
//    compare those integers.
//    Be careful if the least significant qualifier is SECONDS because these
//    will need to be compared as microseconds.
//    - SQL standard 2008 (draft): Part 2 (Foundation), page 401, section 8.2 <comparison_predicate>, General Rules, point (7)


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

QueryNode* GtExpr::pCreate(QueryNode* pExpr1, QueryNode* pExpr2)
{
  _static_

  // Create a unique name for this object
  s256 sName;
  NodeNamer::vGenerateName(C_GT_DEFN, sName);

  QueryNode* pExpr = new QueryNode(sName, C_GT_DEFN);
  pExpr->vAppendChild(pExpr1);
  pExpr->vAppendChild(pExpr2);

  return pExpr;
}


/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void GtExpr::vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));

  QueryNode* pChild1 = GtExpr::pChild1(_pQueryNode(pNode));
  QueryNode* pChild2 = GtExpr::pChild2(_pQueryNode(pNode));

  if (pChild1->nChildren() == 0 || SubqueryRefExpr::bIsOfType(pChild1)) {
    Expr::vDescribe(pChild1, sBuf, pRootNode);

  } else {
    sBuf.vConcat("("_Stz);
    Expr::vDescribe(pChild1, sBuf, pRootNode);
    sBuf.vConcat(")"_Stz);
  }

  sBuf.vConcat(" > "_Stz);

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
void GtExpr::vDescribeAsTypes(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));

  QueryNode* pChild1 = GtExpr::pChild1(_pQueryNode(pNode));
  QueryNode* pChild2 = GtExpr::pChild2(_pQueryNode(pNode));

  sBuf.vConcat(Expr::xTypeId(pChild1, pRootNode).szTypeName());
  sBuf.vConcat(" > "_Stz);
  sBuf.vConcat(Expr::xTypeId(pChild2, pRootNode).szTypeName());
}

/*----------------------------------------------------------------------------*/
/* Return the first child expression of this operator                         */
/*----------------------------------------------------------------------------*/
QueryNode* GtExpr::pChild1(QueryNode *pNode)
{
  _static_
  ASSERT(bIsOfType(pNode));
  ASSERT(pNode->nChildren() == 2);

  return _pQueryNode(pNode->pFirstChild());
}

/*----------------------------------------------------------------------------*/
/* Return the second child expression of this operator                         */
/*----------------------------------------------------------------------------*/
QueryNode* GtExpr::pChild2(QueryNode *pNode)
{
  _static_
  ASSERT(bIsOfType(pNode));
  ASSERT(pNode->nChildren() == 2);

  return _pQueryNode(pNode->pSecondChild());
}


/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the expr definition         */
/*----------------------------------------------------------------------------*/
b1 GtExpr::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_GT_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 GtExpr::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_GT_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns the resulting type of the expression operation.                    */
/* A returned value of CT_NONE means that it is not supported or invalid.     */
/*----------------------------------------------------------------------------*/
TypeId GtExpr::xTypeId(QueryNode* pExprNode, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pExprNode));

  // Get the types of the two operands
  QueryNode* pChild1 = GtExpr::pChild1(pExprNode);
  QueryNode* pChild2 = GtExpr::pChild2(pExprNode);

  TypeId xType1 = Expr::xTypeId(pChild1, pRootNode);
  TypeId xType2 = Expr::xTypeId(pChild2, pRootNode);

  // Lookup the type of the result
  TypeId xResult = S_pTypeMap->xLookup(xType1, xType2);
  return xResult;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
