/*============================================================================*/
/* AvgExpr.hpp - an AVG() aggregate expression                                */
/*============================================================================*/

#include "BASE.h"
#include "AvgExpr.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "GraphNav.hpp"
#include "NodeNamer.hpp"
#include "TypeSystem.hpp"
#include "TypeMap1D.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_AVG_EXPR_NAME_PREFIX = "AvgExpr"_Stz;
static Stz* C_AVG_EXPR_DEFN = "/__root__/definitions/object/expr/operator/aggregate/avg"_Stz;

// What is the resulting type when we combine these inputs?
//  - The first parameter represents the type of the first operand
//  - The second parameter represents the type of the result from this operation.
//  - Any unspecified mappings are unsupported.
static TypeMap1D* S_pTypeMap = (new TypeMap1D())
    ->pAddSelfMapping( TypeId::xIdOf(CT_DECIMAL))
    ->pAddSelfMapping( TypeId::xIdOf(CT_FLOAT))
    ->pAddMapping(     TypeId::xIdOf(CT_INTEGER),  TypeId::xIdOf(CT_DECIMAL)  )
    ->pAddSelfMapping( TypeId::xIdOf(IV_YEAR) )
    ->pAddSelfMapping( TypeId::xIdOf(IV_MONTH) )
    ->pAddSelfMapping( TypeId::xIdOf(IV_YEAR_TO_MONTH) )
    ->pAddSelfMapping( TypeId::xIdOf(IV_DAY) )
    ->pAddSelfMapping( TypeId::xIdOf(IV_HOUR) )
    ->pAddSelfMapping( TypeId::xIdOf(IV_MINUTE) )
    ->pAddSelfMapping( TypeId::xIdOf(IV_SECOND) )
    ->pAddSelfMapping( TypeId::xIdOf(IV_DAY_TO_HOUR) )
    ->pAddSelfMapping( TypeId::xIdOf(IV_DAY_TO_MINUTE) )
    ->pAddSelfMapping( TypeId::xIdOf(IV_DAY_TO_SECOND) )
    ->pAddSelfMapping( TypeId::xIdOf(IV_HOUR_TO_MINUTE) )
    ->pAddSelfMapping( TypeId::xIdOf(IV_HOUR_TO_SECOND) )
    ->pAddSelfMapping( TypeId::xIdOf(IV_MINUTE_TO_SECOND) )
    ->pAddSelfMapping( TypeId::xIdOf(CT_UNKNOWN))
    ;

// References:
//
//  The average of an exact numeric is an implementation defined exact numeric
//    - SQL standard 2008 (draft): Part 2 (Foundation), page 537, section 10.9, point (7)(g)(iii)
//
//  The average of an approximate numeric is an implementation defined approximate numeric
//    - SQL standard 2008 (draft): Part 2 (Foundation), page 537, section 10.9, point (7)(g)(iv)
//
//  The average of an interval type is an implementation defined interval type
//    - SQL standard 2008 (draft): Part 2 (Foundation), page 537, section 10.9, point (7)(g)(v)

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Helper function to create expression nodes.                                */
/*----------------------------------------------------------------------------*/
QueryNode* AvgExpr::pCreate(b1 bIsDistinct, QueryNode* pChildExpr)
{
  _static_

  // Create the properties
  Value xDistinctVal(bIsDistinct);
  QueryNode* pDistinct = new QueryNode(const_cast<Stz*>(C_DISTINCT_PROP_NAME),
                                       const_cast<Stz*>(C_DISTINCT_PROP_TYPE),
                                       static_cast<Value&&>(xDistinctVal));

  PlanNodeList* pProperties = new PlanNodeList;
  pProperties->vAppend(pDistinct);

  // and the children
  PlanNodeList* pChildren = new PlanNodeList;
  if (pChildExpr) {
    pChildren->vAppend(pChildExpr);
  }

  // Finally, create this object
  s256 sName;
  NodeNamer::vGenerateName(C_AVG_EXPR_DEFN, sName);

  Value xVal;
  QueryNode* pAvgExpr = new QueryNode(sName, C_AVG_EXPR_DEFN, static_cast<Value&&>(xVal), pProperties, pChildren);
  return pAvgExpr;
}


/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void AvgExpr::vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));

  sBuf.vConcat("AVG("_Stz);

  b1 bDistinct = bIsDistinct(pNode);
  if (bDistinct) {
    sBuf.vConcat("DISTINCT("_Stz);
  }

  ASSERT(pNode->nChildren() == 1);
  PlanNode* pChildExpr = pNode->pFirstChild();
  Expr::vDescribe(pChildExpr, sBuf, pRootNode);

  if (bDistinct) {
    sBuf.vConcat(")"_Stz);
  }

  sBuf.vConcat(")"_Stz);
}

/*----------------------------------------------------------------------------*/
/* Represent the expression as types instead of names. Append to the buffer   */
/*----------------------------------------------------------------------------*/
void AvgExpr::vDescribeAsTypes(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));

  sBuf.vConcat("AVG("_Stz);

  b1 bDistinct = bIsDistinct(pNode);
  if (bDistinct) {
    sBuf.vConcat("DISTINCT("_Stz);
  }

  ASSERT(pNode->nChildren() == 1);
  PlanNode* pChildExpr = pNode->pFirstChild();
  sBuf.vConcat(Expr::xTypeId(_pQueryNode(pChildExpr), pRootNode).szTypeName());

  if (bDistinct) {
    sBuf.vConcat(")"_Stz);
  }

  sBuf.vConcat(")"_Stz);
}

/*----------------------------------------------------------------------------*/
/* Return the first child expression of this operator                         */
/*----------------------------------------------------------------------------*/
QueryNode* AvgExpr::pChild1(QueryNode *pNode)
{
  _static_
  ASSERT(bIsOfType(pNode));
  ASSERT(pNode->nChildren() == 1);

  return _pQueryNode(pNode->pFirstChild());
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the expr definition         */
/*----------------------------------------------------------------------------*/
b1 AvgExpr::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_AVG_EXPR_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 AvgExpr::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_AVG_EXPR_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns the resulting type of the expression operation.                    */
/* A returned value of CT_NONE means that it is not supported or invalid.     */
/*----------------------------------------------------------------------------*/
TypeId AvgExpr::xTypeId(QueryNode* pExprNode, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pExprNode));

  // Get the types of the operand
  QueryNode* pChild1 = AvgExpr::pChild1(pExprNode);

  TypeId xOperand1TypeId = Expr::xTypeId(pChild1, pRootNode);

  TypeId xResult = S_pTypeMap->xLookup(xOperand1TypeId);
  return xResult;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
