/*============================================================================*/
/* InExpr.hpp - an IN expression operator                                     */
/*============================================================================*/

#include "BASE.h"
#include "InExpr.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "NodeNamer.hpp"
#include "SubqueryRefExpr.hpp"
#include "SqlException.hpp"
#include "TypeSystem.hpp"
#include "TypeMap2D.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_NAME_PREFIX = "inExpr"_Stz;
static Stz* C_DEFN = "/__root__/definitions/object/expr/operator/comparison/in"_Stz;

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

        // All YM intervals are comparable with each other, but they may need to be implicitly or explicitly cast.
        // Refer [1] below.
    ->pAddMapping( TypeId::xIdOf(IV_YEAR),          TM_SAME, TypeId::xIdOf(CT_BOOLEAN))
    ->pAddMapping( TypeId::xIdOf(IV_MONTH),         TM_SAME, TypeId::xIdOf(CT_BOOLEAN))
    ->pAddMapping( TypeId::xIdOf(IV_YEAR_TO_MONTH), TM_SAME, TypeId::xIdOf(CT_BOOLEAN))

        // All DT intervals are comparable with each other, but they may need to be implicitly or explicitly cast.
        // Refer [1] below.
    ->pAddMapping( TypeId::xIdOf(IV_DAY),              TM_SAME,  TypeId::xIdOf(CT_BOOLEAN))
    ->pAddMapping( TypeId::xIdOf(IV_HOUR),             TM_SAME,  TypeId::xIdOf(CT_BOOLEAN))
    ->pAddMapping( TypeId::xIdOf(IV_MINUTE),           TM_SAME,  TypeId::xIdOf(CT_BOOLEAN))
    ->pAddMapping( TypeId::xIdOf(IV_SECOND),           TM_SAME,  TypeId::xIdOf(CT_BOOLEAN))
    ->pAddMapping( TypeId::xIdOf(IV_DAY_TO_HOUR),      TM_SAME,  TypeId::xIdOf(CT_BOOLEAN))
    ->pAddMapping( TypeId::xIdOf(IV_DAY_TO_MINUTE),    TM_SAME,  TypeId::xIdOf(CT_BOOLEAN))
    ->pAddMapping( TypeId::xIdOf(IV_DAY_TO_SECOND),    TM_SAME,  TypeId::xIdOf(CT_BOOLEAN))
    ->pAddMapping( TypeId::xIdOf(IV_HOUR_TO_MINUTE),   TM_SAME,  TypeId::xIdOf(CT_BOOLEAN))
    ->pAddMapping( TypeId::xIdOf(IV_HOUR_TO_SECOND),   TM_SAME,  TypeId::xIdOf(CT_BOOLEAN))
    ->pAddMapping( TypeId::xIdOf(IV_MINUTE_TO_SECOND), TM_SAME,  TypeId::xIdOf(CT_BOOLEAN))

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

QueryNode* InExpr::pCreate(QueryNode* pLeftExpr)
{
  _static_
  ASSERT(pLeftExpr);
  ASSERT(Expr::bIsOfType(pLeftExpr));

  // Create a unique name for this object
  s256 sName;
  NodeNamer::vGenerateName(C_DEFN, sName);

  QueryNode* pInExpr = new QueryNode(sName, C_DEFN);
  pInExpr->vAppendChild(pLeftExpr);

  return pInExpr;
}


/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void InExpr::vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));

  i8 nChildren = pNode->nChildren();
  ASSERT(nChildren > 0);

  s2K sLeftExprBuf;
  s2K sRightExprBuf;

  Expr::vDescribe(pLeftExpr(pNode), sLeftExprBuf, pRootNode);

  if (nChildren == 2 && SubqueryRefExpr::bIsOfType(pNode->pSecondChild())) {
    QueryNode* pChild = _pQueryNode(pNode->pSecondChild());
    ASSERT(Expr::bIsOfType(pChild));

    Expr::vDescribe(pChild, sRightExprBuf, pRootNode);

  } else {
    sRightExprBuf.vConcat("("_Stz);

    PlanNodeIter* pIter = pRightExprIter(pNode);
    b1 bIsFirst = true;
    s2K sItemBuf;

    while (pIter->bHasNext()) {
      QueryNode* pChild = _pQueryNode(pIter->pNext());
      ASSERT(Expr::bIsOfType(pChild));

      if (bIsFirst) {
        bIsFirst = false;
      } else {
        sRightExprBuf.vConcat(", "_Stz);
      }

      sItemBuf.vReset();
      Expr::vDescribe(pChild, sItemBuf, pRootNode);
      sRightExprBuf.vConcat(sItemBuf.szStz());
    }

    delete pIter;
    sRightExprBuf.vConcat(")"_Stz);
  }

  sBuf.vCatSprint("%s IN %s"_Stz, sLeftExprBuf.szStz(), sRightExprBuf.szStz());
}

/*----------------------------------------------------------------------------*/
/* Represent the expression as types instead of names. Append to the buffer   */
/*----------------------------------------------------------------------------*/
void InExpr::vDescribeAsTypes(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));

  i8 nChildren = pNode->nChildren();
  ASSERT(nChildren > 0);

  s2K sLeftExprBuf;
  s2K sRightExprBuf;

  sLeftExprBuf.vConcat(Expr::xTypeId(pLeftExpr(pNode), pRootNode).szTypeName());

  if (nChildren == 2 && SubqueryRefExpr::bIsOfType(pNode->pSecondChild())) {
    QueryNode* pChild = _pQueryNode(pNode->pSecondChild());
    ASSERT(Expr::bIsOfType(pChild));

    sRightExprBuf.vConcat(Expr::xTypeId(pChild, pRootNode).szTypeName());

  } else {
    sRightExprBuf.vConcat("("_Stz);

    PlanNodeIter* pIter = pRightExprIter(pNode);
    b1 bIsFirst = true;

    while (pIter->bHasNext()) {
      QueryNode* pChild = _pQueryNode(pIter->pNext());
      ASSERT(Expr::bIsOfType(pChild));

      if (bIsFirst) {
        bIsFirst = false;
      } else {
        sRightExprBuf.vConcat(", "_Stz);
      }

      sRightExprBuf.vConcat(Expr::xTypeId(pChild, pRootNode).szTypeName());
    }

    delete pIter;
    sRightExprBuf.vConcat(")"_Stz);
  }

  sBuf.vCatSprint("%s IN %s"_Stz, sLeftExprBuf.szStz(), sRightExprBuf.szStz());
}


/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the expr definition         */
/*----------------------------------------------------------------------------*/
b1 InExpr::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 InExpr::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns the expression left of the IN clause that is being compared        */
/* e.g. leftExpr IN (rightExpr1, rightExpr2, ...)                             */
/*----------------------------------------------------------------------------*/
QueryNode* InExpr::pLeftExpr(PlanNode* pNode)
{
  ASSERT(bIsOfType(pNode));
  ASSERT(pNode->nChildren() >= 2);

  return _pQueryNode(pNode->pFirstChild());
}

/*----------------------------------------------------------------------------*/
/* Returns an iterator starting at the first expression to the right of       */
/* the IN clause. e.g. leftExpr IN (rightExpr1, rightExpr2, ...)              */
/* Iterator must be freed after use.                                          */
/*----------------------------------------------------------------------------*/
PlanNodeIter* InExpr::pRightExprIter(PlanNode* pNode)
{
  ASSERT(bIsOfType(pNode));
  ASSERT(pNode->nChildren() >= 2);

  PlanNodeIter* pIter = pNode->pNewChildrenIter();
  ASSERT(pIter->bHasNext());
  pIter->pNext();
  ASSERT(pIter->bHasNext());

  return pIter;
}

/*----------------------------------------------------------------------------*/
/* Returns the resulting type of the expression operation.                    */
/* A returned value of CT_NONE means that it is not supported or invalid.     */
/*----------------------------------------------------------------------------*/
TypeId InExpr::xTypeId(QueryNode* pExprNode, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pExprNode));

  // Get the types of the two operands
  // Assuming the type of the second child is the same as each subsequent child
  QueryNode* pChild1 = _pQueryNode(pExprNode->pFirstChild());
  QueryNode* pChild2 = _pQueryNode(pExprNode->pSecondChild());

  TypeId xType1 = Expr::xTypeId(pChild1, pRootNode);
  TypeId xType2 = Expr::xTypeId(pChild2, pRootNode);

  // Lookup the type of the result
  TypeId xResult = S_pTypeMap->xLookup(xType1, xType2);
  
  if (xResult == TypeId::xIdOf(CT_NONE) || pExprNode->nChildren() <= 2) {
    return xResult;
  }

  // Check that all subsequent operands are of the same type as eOperand2
  PlanNodeIter* pIter = pExprNode->pNewChildrenIter();
  ASSERT(pIter->bHasNext()); pIter->pNext();  // pIter points to 1st child
  ASSERT(pIter->bHasNext()); pIter->pNext();  // pIter points to 2nd child

  while (pIter->bHasNext()) {
    QueryNode* pChild = _pQueryNode(pIter->pNext());
    ASSERT(Expr::bIsOfType(pChild));

    TypeId xChildType = Expr::xTypeId(pChild, pRootNode);

    if (xChildType != xType2) {
      xResult = TypeId::xIdOf(CT_NONE);
      break;
    }
  }

  delete pIter;
  return xResult;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
