/*============================================================================*/
/* MaxExpr.hpp - an MAX() aggregate expression                                */
/*============================================================================*/

#include "BASE.h"
#include "MaxExpr.hpp"

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
static Stz* C_MAX_EXPR_NAME_PREFIX = "MaxExpr"_Stz;
static Stz* C_MAX_EXPR_DEFN = "/__root__/definitions/object/expr/operator/aggregate/max"_Stz;

// What is the resulting type when we combine these inputs?
//  - The first parameter represents the type of the first operand
//    as well as the result of the operation
//  - Any unspecified mappings are unsupported.
static TypeMap1D* S_pTypeMap = (new TypeMap1D())
    ->pAddSelfMapping( TypeId::xIdOf(CT_CHAR) )
    ->pAddSelfMapping( TypeId::xIdOf(CT_DATE) )
    ->pAddSelfMapping( TypeId::xIdOf(CT_DECIMAL) )
    ->pAddSelfMapping( TypeId::xIdOf(CT_FLOAT) )
    ->pAddSelfMapping( TypeId::xIdOf(CT_INTEGER) )
    ->pAddSelfMapping( TypeId::xIdOf(IV_YEAR_TO_MONTH) )
    ->pAddSelfMapping( TypeId::xIdOf(IV_YEAR) )
    ->pAddSelfMapping( TypeId::xIdOf(IV_MONTH) )
    ->pAddSelfMapping( TypeId::xIdOf(IV_DAY_TO_HOUR) )
    ->pAddSelfMapping( TypeId::xIdOf(IV_DAY_TO_MINUTE) )
    ->pAddSelfMapping( TypeId::xIdOf(IV_DAY_TO_SECOND) )
    ->pAddSelfMapping( TypeId::xIdOf(IV_HOUR_TO_MINUTE) )
    ->pAddSelfMapping( TypeId::xIdOf(IV_HOUR_TO_SECOND) )
    ->pAddSelfMapping( TypeId::xIdOf(IV_MINUTE_TO_SECOND) )
    ->pAddSelfMapping( TypeId::xIdOf(IV_DAY) )
    ->pAddSelfMapping( TypeId::xIdOf(IV_HOUR) )
    ->pAddSelfMapping( TypeId::xIdOf(IV_MINUTE) )
    ->pAddSelfMapping( TypeId::xIdOf(IV_SECOND) )
    ->pAddSelfMapping( TypeId::xIdOf(CT_TIME) )
    ->pAddSelfMapping( TypeId::xIdOf(CT_TIMESTAMP) )
    ->pAddSelfMapping( TypeId::xIdOf(CT_VARCHAR) )
    ->pAddSelfMapping( TypeId::xIdOf(CT_UNKNOWN) )
;

// NOTE: MAX(boolean) is not supported.

// References:
//  If MAX or MIN is specified, then the declared type of the result is the
//  declared type of the value expression.
//  - SQL standard 2008 (draft): Part 2 (Foundation), page 537, section 10.9, point (7)(g)

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Helper function to create expression nodes.                                */
/*----------------------------------------------------------------------------*/
QueryNode* MaxExpr::pCreate(b1 bIsDistinct, QueryNode* pChildExpr)
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
  NodeNamer::vGenerateName(C_MAX_EXPR_DEFN, sName);

  Value xVal;
  QueryNode* pMaxExpr = new QueryNode(sName, C_MAX_EXPR_DEFN, static_cast<Value&&>(xVal), pProperties, pChildren);
  return pMaxExpr;
}


/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void MaxExpr::vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));

  sBuf.vConcat("MAX("_Stz);

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
void MaxExpr::vDescribeAsTypes(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));

  sBuf.vConcat("MAX("_Stz);

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
QueryNode* MaxExpr::pChild1(QueryNode *pNode)
{
  _static_
  ASSERT(bIsOfType(pNode));
  ASSERT(pNode->nChildren() == 1);

  return _pQueryNode(pNode->pFirstChild());
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the expr definition         */
/*----------------------------------------------------------------------------*/
b1 MaxExpr::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_MAX_EXPR_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 MaxExpr::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_MAX_EXPR_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns the resulting type of the expression operation.                    */
/* A returned value of CT_NONE means that it is not supported or invalid.     */
/*----------------------------------------------------------------------------*/
TypeId MaxExpr::xTypeId(QueryNode* pExprNode, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pExprNode));

  // Get the types of the operand
  QueryNode* pChild1 = MaxExpr::pChild1(pExprNode);

  TypeId xOperand1TypeId = Expr::xTypeId(pChild1, pRootNode);

  TypeId xResult = S_pTypeMap->xLookup(xOperand1TypeId);
  return xResult;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
