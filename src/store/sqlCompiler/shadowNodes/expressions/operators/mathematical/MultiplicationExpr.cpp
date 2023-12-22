/*============================================================================*/
/* MultiplicationExpr.hpp - an multiplication (-) expression operator               */
/*============================================================================*/

#include "BASE.h"
#include "MultiplicationExpr.hpp"

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
static Stz* C_MULTIPLICATION_NAME_PREFIX = "MultiplicationExpr"_Stz;
static Stz* C_MULTIPLICATION_DEFN = "/__root__/definitions/object/expr/operator/mathematical/multiplication"_Stz;

// What is the resulting type when we combine these inputs?
//  - The first parameter represents the type of the first operand
//  - The second parameter represents the type of the second operand
//  - The third parameter represents the type of the result from this operation.
//  - Any unspecified mappings are unsupported.
//  - Multiple mappings may be made in one go using the eTM values to represent a set of types.
static TypeMap2D* S_pTypeMap = (new TypeMap2D())
    // Decimals
    ->pAddSelfMapping(TypeId::xIdOf(CT_DECIMAL))
    ->pAddMapping(TypeId::xIdOf(CT_DECIMAL),        TypeId::xIdOf(CT_UNKNOWN),            TypeId::xIdOf(CT_UNKNOWN))

    // Floats
    ->pAddSelfMapping(TypeId::xIdOf(CT_FLOAT))
    ->pAddMapping(TypeId::xIdOf(CT_FLOAT),          TypeId::xIdOf(CT_UNKNOWN),            TypeId::xIdOf(CT_UNKNOWN))

    // Integers
    ->pAddSelfMapping(TypeId::xIdOf(CT_INTEGER))
    ->pAddMapping(TypeId::xIdOf(CT_INTEGER),        TypeId::xIdOf(CT_UNKNOWN),            TypeId::xIdOf(CT_UNKNOWN))

    // Integer * Intervals
    ->pAddMapping(TypeId::xIdOf(CT_INTEGER),         TypeId::xIdOf(IV_YEAR),              TypeId::xIdOf(IV_YEAR))
    ->pAddMapping(TypeId::xIdOf(CT_INTEGER),         TypeId::xIdOf(IV_MONTH),             TypeId::xIdOf(IV_MONTH))
    ->pAddMapping(TypeId::xIdOf(CT_INTEGER),         TypeId::xIdOf(IV_YEAR_TO_MONTH),     TypeId::xIdOf(IV_YEAR_TO_MONTH))
    ->pAddMapping(TypeId::xIdOf(CT_INTEGER),         TypeId::xIdOf(IV_DAY),               TypeId::xIdOf(IV_DAY))
    ->pAddMapping(TypeId::xIdOf(CT_INTEGER),         TypeId::xIdOf(IV_HOUR),              TypeId::xIdOf(IV_HOUR))
    ->pAddMapping(TypeId::xIdOf(CT_INTEGER),         TypeId::xIdOf(IV_MINUTE),            TypeId::xIdOf(IV_MINUTE))
    ->pAddMapping(TypeId::xIdOf(CT_INTEGER),         TypeId::xIdOf(IV_SECOND),            TypeId::xIdOf(IV_SECOND))
    ->pAddMapping(TypeId::xIdOf(CT_INTEGER),         TypeId::xIdOf(IV_DAY_TO_HOUR),       TypeId::xIdOf(IV_DAY_TO_HOUR))
    ->pAddMapping(TypeId::xIdOf(CT_INTEGER),         TypeId::xIdOf(IV_DAY_TO_MINUTE),     TypeId::xIdOf(IV_DAY_TO_MINUTE))
    ->pAddMapping(TypeId::xIdOf(CT_INTEGER),         TypeId::xIdOf(IV_DAY_TO_SECOND),     TypeId::xIdOf(IV_DAY_TO_SECOND))
    ->pAddMapping(TypeId::xIdOf(CT_INTEGER),         TypeId::xIdOf(IV_HOUR_TO_MINUTE),    TypeId::xIdOf(IV_HOUR_TO_MINUTE))
    ->pAddMapping(TypeId::xIdOf(CT_INTEGER),         TypeId::xIdOf(IV_HOUR_TO_SECOND),    TypeId::xIdOf(IV_HOUR_TO_SECOND))
    ->pAddMapping(TypeId::xIdOf(CT_INTEGER),         TypeId::xIdOf(IV_MINUTE_TO_SECOND),  TypeId::xIdOf(IV_MINUTE_TO_SECOND))

    // Intervals * Integer
    ->pAddMapping(TypeId::xIdOf(IV_YEAR),            TypeId::xIdOf(CT_INTEGER),           TypeId::xIdOf(IV_YEAR))
    ->pAddMapping(TypeId::xIdOf(IV_MONTH),           TypeId::xIdOf(CT_INTEGER),           TypeId::xIdOf(IV_MONTH))
    ->pAddMapping(TypeId::xIdOf(IV_YEAR_TO_MONTH),   TypeId::xIdOf(CT_INTEGER),           TypeId::xIdOf(IV_YEAR_TO_MONTH))
    ->pAddMapping(TypeId::xIdOf(IV_DAY),             TypeId::xIdOf(CT_INTEGER),           TypeId::xIdOf(IV_DAY))
    ->pAddMapping(TypeId::xIdOf(IV_HOUR),            TypeId::xIdOf(CT_INTEGER),           TypeId::xIdOf(IV_HOUR))
    ->pAddMapping(TypeId::xIdOf(IV_MINUTE),          TypeId::xIdOf(CT_INTEGER),           TypeId::xIdOf(IV_MINUTE))
    ->pAddMapping(TypeId::xIdOf(IV_SECOND),          TypeId::xIdOf(CT_INTEGER),           TypeId::xIdOf(IV_SECOND))
    ->pAddMapping(TypeId::xIdOf(IV_DAY_TO_HOUR),     TypeId::xIdOf(CT_INTEGER),           TypeId::xIdOf(IV_DAY_TO_HOUR))
    ->pAddMapping(TypeId::xIdOf(IV_DAY_TO_MINUTE),   TypeId::xIdOf(CT_INTEGER),           TypeId::xIdOf(IV_DAY_TO_MINUTE))
    ->pAddMapping(TypeId::xIdOf(IV_DAY_TO_SECOND),   TypeId::xIdOf(CT_INTEGER),           TypeId::xIdOf(IV_DAY_TO_SECOND))
    ->pAddMapping(TypeId::xIdOf(IV_HOUR_TO_MINUTE),  TypeId::xIdOf(CT_INTEGER),           TypeId::xIdOf(IV_HOUR_TO_MINUTE))
    ->pAddMapping(TypeId::xIdOf(IV_HOUR_TO_SECOND),  TypeId::xIdOf(CT_INTEGER),           TypeId::xIdOf(IV_HOUR_TO_SECOND))
    ->pAddMapping(TypeId::xIdOf(IV_MINUTE_TO_SECOND),TypeId::xIdOf(CT_INTEGER),           TypeId::xIdOf(IV_MINUTE_TO_SECOND))

    // Intervals * NULLs
    ->pAddMapping(TM_ALL_YM_INTERVALS,                   TypeId::xIdOf(CT_UNKNOWN),          TypeId::xIdOf(CT_UNKNOWN))
    ->pAddMapping(TM_ALL_DT_INTERVALS,                   TypeId::xIdOf(CT_UNKNOWN),          TypeId::xIdOf(CT_UNKNOWN))

    // Nulls
    ->pAddMapping(TypeId::xIdOf(CT_UNKNOWN),          TypeId::xIdOf(CT_DECIMAL),          TypeId::xIdOf(CT_UNKNOWN))
    ->pAddMapping(TypeId::xIdOf(CT_UNKNOWN),          TypeId::xIdOf(CT_FLOAT),            TypeId::xIdOf(CT_UNKNOWN))
    ->pAddMapping(TypeId::xIdOf(CT_UNKNOWN),          TypeId::xIdOf(CT_INTEGER),          TypeId::xIdOf(CT_UNKNOWN))
    ->pAddMapping(TypeId::xIdOf(CT_UNKNOWN),        TM_ALL_YM_INTERVALS,                     TypeId::xIdOf(CT_UNKNOWN))
    ->pAddMapping(TypeId::xIdOf(CT_UNKNOWN),        TM_ALL_DT_INTERVALS,                     TypeId::xIdOf(CT_UNKNOWN))
    ->pAddSelfMapping(TypeId::xIdOf(CT_UNKNOWN))
;

// References:
//  Exact numeric * exact numeric = exact numeric
//    - SQL standard 2008 (draft): Part 2 (Foundation), page 251, point (1)
//
//  Approximate numeric * exact numeric (any order) = approximate numeric:
//    - SQL standard 2008 (draft): Part 2 (Foundation), page 251, point (2)
//
//  Datetime & Intervals arithmetic:
//    Interval  * or /  Numeric   = Interval
//    Numeric   *       Interval  = Interval
//     - SQL standard 2008 (draft): Part 2 (Foundation), page 39 (table 7)


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

QueryNode* MultiplicationExpr::pCreate(QueryNode* pExpr1, QueryNode* pExpr2)
{
  _static_

  // Create a unique name for this object
  s256 sName;
  NodeNamer::vGenerateName(C_MULTIPLICATION_DEFN, sName);

  QueryNode* pExpr = new QueryNode(sName, C_MULTIPLICATION_DEFN);
  pExpr->vAppendChild(pExpr1);
  pExpr->vAppendChild(pExpr2);

  return pExpr;
}


/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void MultiplicationExpr::vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));

  QueryNode* pChild1 = MultiplicationExpr::pChild1(_pQueryNode(pNode));
  QueryNode* pChild2 = MultiplicationExpr::pChild2(_pQueryNode(pNode));

  if (pChild1->nChildren() == 0 || SubqueryRefExpr::bIsOfType(pChild1)) {
    Expr::vDescribe(pChild1, sBuf, pRootNode);

  } else {
    sBuf.vConcat("("_Stz);
    Expr::vDescribe(pChild1, sBuf, pRootNode);
    sBuf.vConcat(")"_Stz);
  }

  sBuf.vConcat(" * "_Stz);

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
void MultiplicationExpr::vDescribeAsTypes(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));

  QueryNode* pChild1 = MultiplicationExpr::pChild1(_pQueryNode(pNode));
  QueryNode* pChild2 = MultiplicationExpr::pChild2(_pQueryNode(pNode));

  sBuf.vConcat(Expr::xTypeId(pChild1, pRootNode).szTypeName());
  sBuf.vConcat(" * "_Stz);
  sBuf.vConcat(Expr::xTypeId(pChild2, pRootNode).szTypeName());
}

/*----------------------------------------------------------------------------*/
/* Return the first child expression of this operator                         */
/*----------------------------------------------------------------------------*/
QueryNode* MultiplicationExpr::pChild1(QueryNode *pNode)
{
  _static_
  ASSERT(bIsOfType(pNode));
  ASSERT(pNode->nChildren() == 2);

  return _pQueryNode(pNode->pFirstChild());
}

/*----------------------------------------------------------------------------*/
/* Return the second child expression of this operator                         */
/*----------------------------------------------------------------------------*/
QueryNode* MultiplicationExpr::pChild2(QueryNode *pNode)
{
  _static_
  ASSERT(bIsOfType(pNode));
  ASSERT(pNode->nChildren() == 2);

  return _pQueryNode(pNode->pSecondChild());
}


/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the expr definition         */
/*----------------------------------------------------------------------------*/
b1 MultiplicationExpr::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_MULTIPLICATION_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 MultiplicationExpr::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_MULTIPLICATION_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns the resulting type of the expression operation.                    */
/* A returned value of CT_NONE means that it is not supported or invalid.     */
/*----------------------------------------------------------------------------*/
TypeId MultiplicationExpr::xTypeId(QueryNode* pExprNode, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pExprNode));

  // Get the types of the two operands
  QueryNode* pChild1 = MultiplicationExpr::pChild1(pExprNode);
  QueryNode* pChild2 = MultiplicationExpr::pChild2(pExprNode);

  TypeId xType1 = Expr::xTypeId(pChild1, pRootNode);
  TypeId xType2 = Expr::xTypeId(pChild2, pRootNode);

  // Lookup the type of the result
  TypeId xResult = S_pTypeMap->xLookup(xType1, xType2);
  return xResult;
}

/*----------------------------------------------------------------------------*/
/* Returns the precision of the resulting expression or -1 if not set or n/a  */
/*----------------------------------------------------------------------------*/
i8 MultiplicationExpr::iPrecision(QueryNode* pExprNode, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pExprNode));
  ASSERT(pRootNode);

  TypeId xResultType = xTypeId(pExprNode, pRootNode);
  eCT eResultType = xResultType.eCTVal();

  i8 iVal = -1;

  switch (eResultType) {

    case CT_NONE:
    case CT_BOOLEAN:
    case CT_CHAR:
    case CT_DATE:
      break;

    case CT_DECIMAL:
      iVal = iCalcDecimalPrecision(pChild1(pExprNode), pChild2(pExprNode), pRootNode);
      break;

    case CT_FLOAT:
      iVal = C_MAX_FLT_PRECISION;
      break;

    case CT_INTEGER:
      iVal = C_MAX_INT_PRECISION;
      break;

    case CT_INTERVAL_DT:
    case CT_INTERVAL_YM:
    case CT_TIME:
    case CT_TIMESTAMP:
    case CT_VARCHAR:
    case CT_UNKNOWN:
      break;

    case CT_ENUMS:
      NEVER_REACH;
  }

  return iVal;
}

/*----------------------------------------------------------------------------*/
/* Returns the scale of the resulting expression or -1 if not set or n/a      */
/*----------------------------------------------------------------------------*/
i8 MultiplicationExpr::iScale(QueryNode* pExprNode, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pExprNode));
  ASSERT(pRootNode);

  TypeId xResultType = xTypeId(pExprNode, pRootNode);
  eCT eResultType = xResultType.eCTVal();

  i8 iVal = -1;

  switch (eResultType) {

    case CT_NONE:
    case CT_BOOLEAN:
    case CT_CHAR:
    case CT_DATE:
      break;

    case CT_DECIMAL:
      iVal = iCalcDecimalScale(pChild1(pExprNode), pChild2(pExprNode), pRootNode);
      break;

    case CT_FLOAT:
    case CT_INTEGER:
    case CT_INTERVAL_DT:
    case CT_INTERVAL_YM:
    case CT_TIME:
    case CT_TIMESTAMP:
    case CT_VARCHAR:
    case CT_UNKNOWN:
      break;

    case CT_ENUMS:
      NEVER_REACH;
  }

  return iVal;
}


/*----------------------------------------------------------------------------*/
/* Calculates the resulting precision of adding two decimal operands          */
/*----------------------------------------------------------------------------*/
i8 MultiplicationExpr::iCalcDecimalPrecision(Parser::QueryNode *pChild1,
                                          Parser::QueryNode *pChild2,
                                          Parser::PlanNode *pRootNode)
{
  _static_
      ASSERT(pChild1);
  ASSERT(pChild2);
  ASSERT(pRootNode);
  ASSERT(Expr::xTypeId(pChild1,pRootNode) == TypeId::xIdOf(CT_DECIMAL));
  ASSERT(Expr::xTypeId(pChild2,pRootNode) == TypeId::xIdOf(CT_DECIMAL));

  // Precision = p1 + p2 + 1

  i8 iPrecision1 = Expr::iPrecision(pChild1, pRootNode);
  i8 iPrecision2 = Expr::iPrecision(pChild2, pRootNode);

  ASSERT(iPrecision1 > 0);
  ASSERT(iPrecision2 > 0);

  i8 iVal = iPrecision1 + iPrecision2 + 1;

  if (iVal > C_MAX_BCD_PRECISION) {
    iVal = C_MAX_BCD_PRECISION;
  }

  return iVal;
}
/*----------------------------------------------------------------------------*/
/* Calculates the resulting scale of adding two decimal operands              */
/*----------------------------------------------------------------------------*/
i8 MultiplicationExpr::iCalcDecimalScale(Parser::QueryNode *pChild1,
                                      Parser::QueryNode *pChild2,
                                      Parser::PlanNode *pRootNode)
{
  _static_
  ASSERT(pChild1);
  ASSERT(pChild2);
  ASSERT(pRootNode);
  ASSERT(Expr::xTypeId(pChild1,pRootNode) == TypeId::xIdOf(CT_DECIMAL));
  ASSERT(Expr::xTypeId(pChild2,pRootNode) == TypeId::xIdOf(CT_DECIMAL));

  // Scale = s1 + s2

  i8 iScale1 = Expr::iScale(pChild1, pRootNode);
  i8 iScale2 = Expr::iScale(pChild2, pRootNode);

  ASSERT(iScale1 >= 0);
  ASSERT(iScale2 >= 0);

  i8 iVal = iScale1 + iScale2;

  if (iVal > C_MAX_BCD_SCALE) {
    iVal = C_MAX_BCD_SCALE;
  }

  return iVal;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
