/*============================================================================*/
/* BetweenExpr.hpp - a BETWEEN expression operator                            */
/*============================================================================*/

#include "BASE.h"
#include "BetweenExpr.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "NodeNamer.hpp"
#include "SubqueryRefExpr.hpp"
#include "GraphNav.hpp"
#include "TypeSystem.hpp"
#include "TypeMap2D.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_NAME_PREFIX = "BetweenExpr"_Stz;
static Stz* C_DEFN = "/__root__/definitions/object/expr/operator/comparison/between"_Stz;


// What is the resulting type when we combine these inputs?
//  - The first parameter represents the type of the first operand
//  - The second parameter represents the type of the second operand
//  - The third parameter represents the type of the result from this operation.
//  - Any unspecified mappings are unsupported.
//  - Multiple mappings may be made in one go using the eTM values to represent a set of types.
static TypeMap2D* S_pTypeMap = (new TypeMap2D())
    ->pAddMapping( TypeId::xIdOf(CT_CHAR),    TM_ALL_STRINGS,                   TypeId::xIdOf(CT_BOOLEAN))
    ->pAddMapping( TypeId::xIdOf(CT_DATE),      TypeId::xIdOf(CT_DATE),      TypeId::xIdOf(CT_BOOLEAN))
    ->pAddMapping( TypeId::xIdOf(CT_DECIMAL),   TypeId::xIdOf(CT_DECIMAL),   TypeId::xIdOf(CT_BOOLEAN))
    ->pAddMapping( TypeId::xIdOf(CT_FLOAT),     TypeId::xIdOf(CT_FLOAT),     TypeId::xIdOf(CT_BOOLEAN))
    ->pAddMapping( TypeId::xIdOf(CT_INTEGER),   TypeId::xIdOf(CT_INTEGER),   TypeId::xIdOf(CT_BOOLEAN))
    ->pAddMapping( TypeId::xIdOf(CT_TIME),      TypeId::xIdOf(CT_TIME),      TypeId::xIdOf(CT_BOOLEAN))
    ->pAddMapping( TypeId::xIdOf(CT_TIMESTAMP), TypeId::xIdOf(CT_TIMESTAMP), TypeId::xIdOf(CT_BOOLEAN))
    ->pAddMapping( TypeId::xIdOf(CT_VARCHAR), TM_ALL_STRINGS,                   TypeId::xIdOf(CT_BOOLEAN))

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
    ->pAddMapping(TypeId::xIdOf(CT_UNKNOWN), TM_ALL_INC_UNKNOWN,             TypeId::xIdOf(CT_BOOLEAN))
    ->pAddMapping(TM_ALL_INC_UNKNOWN,             TypeId::xIdOf(CT_UNKNOWN), TypeId::xIdOf(CT_BOOLEAN))
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

QueryNode* BetweenExpr::pCreate(QueryNode* pPredicand, QueryNode* pLowerBound, QueryNode* pUpperBound)
{
  _static_
  ASSERT(pPredicand);
  ASSERT(Expr::bIsOfType(pPredicand));
  ASSERT(pLowerBound);
  ASSERT(Expr::bIsOfType(pLowerBound));
  ASSERT(pUpperBound);
  ASSERT(Expr::bIsOfType(pUpperBound));

  // Create a unique name for this object
  s256 sName;
  NodeNamer::vGenerateName(C_DEFN, sName);

  QueryNode* pExpr = new QueryNode(sName, C_DEFN);
  pExpr->vAppendChild(pPredicand);
  pExpr->vAppendChild(pLowerBound);
  pExpr->vAppendChild(pUpperBound);

  return pExpr;
}


/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void BetweenExpr::vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));

  QueryNode* pPredicand  = BetweenExpr::pPredicand(_pQueryNode(pNode));
  QueryNode* pLowerBound = BetweenExpr::pLowerBound(_pQueryNode(pNode));
  QueryNode* pUpperBound = BetweenExpr::pUpperBound(_pQueryNode(pNode));

  s2K sTempBuf;

  if (pPredicand->nChildren() == 0 || SubqueryRefExpr::bIsOfType(pPredicand)) {

    Expr::vDescribe(pPredicand, sTempBuf, pRootNode);
    sBuf.vConcat(sTempBuf.szStz());

  } else {
    sBuf.vConcat("("_Stz);
    Expr::vDescribe(pPredicand, sTempBuf, pRootNode);
    sBuf.vConcat(sTempBuf.szStz());
    sBuf.vConcat(")"_Stz);
  }

  sBuf.vConcat(" BETWEEN "_Stz);

  sTempBuf.vReset();
  if (pLowerBound->nChildren() == 0 || SubqueryRefExpr::bIsOfType(pLowerBound)) {
    Expr::vDescribe(pLowerBound, sTempBuf, pRootNode);
    sBuf.vConcat(sTempBuf.szStz());

  } else {
    sBuf.vConcat("("_Stz);
    Expr::vDescribe(pLowerBound, sTempBuf, pRootNode);
    sBuf.vConcat(sTempBuf.szStz());
    sBuf.vConcat(")"_Stz);
  }

  sBuf.vConcat(" AND "_Stz);

  sTempBuf.vReset();
  if (pUpperBound->nChildren() == 0 || SubqueryRefExpr::bIsOfType(pUpperBound)) {
    Expr::vDescribe(pUpperBound, sTempBuf, pRootNode);
    sBuf.vConcat(sTempBuf.szStz());

  } else {
    sBuf.vConcat("("_Stz);
    Expr::vDescribe(pUpperBound, sTempBuf, pRootNode);
    sBuf.vConcat(sTempBuf.szStz());
    sBuf.vConcat(")"_Stz);
  }
}

/*----------------------------------------------------------------------------*/
/* Represent the expression as types instead of names. Append to the buffer   */
/*----------------------------------------------------------------------------*/
void BetweenExpr::vDescribeAsTypes(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));

  QueryNode* pPredicand  = BetweenExpr::pPredicand(_pQueryNode(pNode));
  QueryNode* pLowerBound = BetweenExpr::pLowerBound(_pQueryNode(pNode));
  QueryNode* pUpperBound = BetweenExpr::pUpperBound(_pQueryNode(pNode));

  sBuf.vConcat(Expr::xTypeId(pPredicand, pRootNode).szTypeName());
  sBuf.vConcat(" BETWEEN "_Stz);
  sBuf.vConcat(Expr::xTypeId(pLowerBound, pRootNode).szTypeName());
  sBuf.vConcat(" AND "_Stz);
  sBuf.vConcat(Expr::xTypeId(pUpperBound, pRootNode).szTypeName());
}

/*----------------------------------------------------------------------------*/
/* Return the expression being compared                                       */
/*----------------------------------------------------------------------------*/
QueryNode* BetweenExpr::pPredicand(QueryNode *pNode)
{
  _static_
  ASSERT(bIsOfType(pNode));
  ASSERT(pNode->nChildren() >= 2);
  ASSERT(pNode->nChildren() <= 3);

  QueryNode* pPredicand = _pQueryNode(pNode->pFirstChild());
  ASSERT(pPredicand);
  ASSERT(Expr::bIsOfType(pPredicand));

  return pPredicand;
}

/*----------------------------------------------------------------------------*/
/* The lower bound being compared to                                          */
/*----------------------------------------------------------------------------*/
QueryNode* BetweenExpr::pLowerBound(QueryNode *pNode)
{
  _static_
  ASSERT(bIsOfType(pNode));
  ASSERT(pNode->nChildren() == 3);

  QueryNode* pLowerBoundNode = _pQueryNode(pNode->pSecondChild());
  ASSERT(pLowerBoundNode);
  ASSERT(Expr::bIsOfType(pLowerBoundNode));

  return pLowerBoundNode;
}

/*----------------------------------------------------------------------------*/
/* The upper bound being compared to                                          */
/*----------------------------------------------------------------------------*/
QueryNode* BetweenExpr::pUpperBound(QueryNode *pNode)
{
  _static_
  ASSERT(bIsOfType(pNode));
  ASSERT(pNode->nChildren() == 3);

  QueryNode* pUpperBoundNode = _pQueryNode(pNode->pNthChild(2)); // 0-based index
  ASSERT(pUpperBoundNode);
  ASSERT(Expr::bIsOfType(pUpperBoundNode));

  return pUpperBoundNode;
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the expr definition         */
/*----------------------------------------------------------------------------*/
b1 BetweenExpr::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 BetweenExpr::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns the resulting type of the expression operation.                    */
/* A returned value of CT_NONE means that it is not supported or invalid.     */
/*----------------------------------------------------------------------------*/
TypeId BetweenExpr::xTypeId(QueryNode* pExprNode, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pExprNode));

  QueryNode* pPredicandNode = BetweenExpr::pPredicand(pExprNode);
  QueryNode* pLowerBound    = BetweenExpr::pLowerBound(pExprNode);
  QueryNode* pUpperBound    = BetweenExpr::pUpperBound(pExprNode);

  TypeId xPredicand = Expr::xTypeId(pPredicandNode, pRootNode);
  TypeId xLower     = Expr::xTypeId(pLowerBound, pRootNode);
  TypeId xUpper     = Expr::xTypeId(pUpperBound, pRootNode);

  if (xLower != xUpper &&
      xLower.eCTVal() != xUpper.eCTVal() &&     // caters for compatible interval types
      xLower.eCTVal() != CT_UNKNOWN &&
      xUpper.eCTVal() != CT_UNKNOWN &&
      xPredicand.eCTVal() != CT_UNKNOWN) {
    // coercion is required
    return TypeId::xIdOf(CT_NONE);
  }

  // Lookup the type of the result between the predicand and the lower bound
  TypeId xResult = S_pTypeMap->xLookup(xPredicand, xLower);
  return xResult;
}

/*----------------------------------------------------------------------------*/
/* Returns the precision of the resulting expression or -1 if not set or n/a  */
/*----------------------------------------------------------------------------*/
i8 BetweenExpr::iPrecision(QueryNode* pExprNode, PlanNode* pRootNode)
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
i8 BetweenExpr::iScale(QueryNode* pExprNode, PlanNode* pRootNode)
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
