/*============================================================================*/
/* IsNullExpr.hpp - a logical IsNull expression operator                      */
/*============================================================================*/

#include "BASE.h"
#include "IsNullExpr.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "NodeNamer.hpp"
#include "SubqueryRefExpr.hpp"
#include "TypeSystem.hpp"
#include "TypeMap1D.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_ISNULL_NAME_PREFIX = "IsNullExpr"_Stz;
static Stz* C_ISNULL_DEFN = "/__root__/definitions/object/expr/operator/logic/isnull"_Stz;

// What is the resulting type when we combine these inputs?
//  - The first parameter represents the type of the first operand
//  - The second parameter represents the type of the result from this operation.
//  - Any unspecified mappings are unsupported.
static TypeMap1D* S_pTypeMap = (new TypeMap1D())
    ->pAddMapping( TM_ALL_INC_UNKNOWN,  TypeId::xIdOf(CT_BOOLEAN) )
;

// References:

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Create the necessary plan graph nodes to represent this object             */
/*----------------------------------------------------------------------------*/
QueryNode* IsNullExpr::pCreate(QueryNode* pExpr1)
{
  _static_

  // Create a unique name for this object
  s256 sName;
  NodeNamer::vGenerateName(C_ISNULL_DEFN, sName);

  QueryNode* pExpr = new QueryNode(sName, C_ISNULL_DEFN);
  pExpr->vAppendChild(pExpr1);

  return pExpr;
}

/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void IsNullExpr::vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));

  QueryNode* pChild1 = IsNullExpr::pChild1(_pQueryNode(pNode));

  if (pChild1->nChildren() == 0 || SubqueryRefExpr::bIsOfType(pChild1)) {
    Expr::vDescribe(pChild1, sBuf, pRootNode);

  } else {
    sBuf.vConcat("("_Stz);
    Expr::vDescribe(pChild1, sBuf, pRootNode);
    sBuf.vConcat(")"_Stz);
  }

  sBuf.vConcat(" IS NULL"_Stz);
}

/*----------------------------------------------------------------------------*/
/* Represent the expression as types instead of names. Append to the buffer   */
/*----------------------------------------------------------------------------*/
void IsNullExpr::vDescribeAsTypes(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));

  QueryNode* pChild1 = IsNullExpr::pChild1(_pQueryNode(pNode));

  sBuf.vConcat(Expr::xTypeId(pChild1, pRootNode).szTypeName());
  sBuf.vConcat(" IS NULL"_Stz);
}

/*----------------------------------------------------------------------------*/
/* Return the first child expression of this operator                         */
/*----------------------------------------------------------------------------*/
QueryNode* IsNullExpr::pChild1(QueryNode *pNode)
{
  _static_
  ASSERT(bIsOfType(pNode));
  ASSERT(pNode->nChildren() == 1);

  return _pQueryNode(pNode->pFirstChild());
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the expr definition         */
/*----------------------------------------------------------------------------*/
b1 IsNullExpr::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_ISNULL_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 IsNullExpr::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_ISNULL_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns the resulting type of the expression operation.                    */
/* A returned value of CT_NONE means that it is not supported or invalid.     */
/*----------------------------------------------------------------------------*/
TypeId IsNullExpr::xTypeId(QueryNode* pExprNode, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pExprNode));

  // Get the types of the two operands
  QueryNode* pChild1 = IsNullExpr::pChild1(pExprNode);

  TypeId xOperand1TypeId = Expr::xTypeId(pChild1, pRootNode);

  if (xOperand1TypeId.eCTVal() == CT_NONE) {
    return TypeId::xIdOf(CT_NONE);
  }

  // Lookup the type of the result
  TypeId xResult = S_pTypeMap->xLookup(xOperand1TypeId);
  return xResult;
}

/*----------------------------------------------------------------------------*/
/* Returns the precision of the resulting expression or -1 if not set or n/a  */
/*----------------------------------------------------------------------------*/
i8 IsNullExpr::iPrecision(QueryNode* pExprNode, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pExprNode));
  ASSERT(pRootNode);
  UNUSED(pExprNode);
  UNUSED(pRootNode);

  // Precision returned is always n/a because the return type is either BOOLEAN or NONE
  return -1;
}

/*----------------------------------------------------------------------------*/
/* Returns the scale of the resulting expression or -1 if not set or n/a      */
/*----------------------------------------------------------------------------*/
i8 IsNullExpr::iScale(QueryNode* pExprNode, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pExprNode));
  ASSERT(pRootNode);
  UNUSED(pExprNode);
  UNUSED(pRootNode);

  // Scale returned is always n/a because the return type is either BOOLEAN or NONE
  return -1;
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
