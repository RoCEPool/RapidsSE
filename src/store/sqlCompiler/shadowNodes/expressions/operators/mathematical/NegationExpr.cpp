/*============================================================================*/
/* NegationExpr.hpp - an negation (-) expression operator                     */
/*============================================================================*/

#include "BASE.h"
#include "NegationExpr.hpp"

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
static Stz* C_NEGATION_NAME_PREFIX = "NegationExpr"_Stz;
static Stz* C_NEGATION_DEFN = "/__root__/definitions/object/expr/operator/mathematical/negation"_Stz;

// What is the resulting type when we combine these inputs?
//  - The first parameter represents the type of the first operand
//  - The second parameter represents the type of the result from this operation.
//  - Any unspecified mappings are unsupported.
static TypeMap1D* S_pTypeMap = (new TypeMap1D())
    ->pAddSelfMapping( TypeId::xIdOf(CT_DECIMAL))
    ->pAddSelfMapping( TypeId::xIdOf(CT_FLOAT))
    ->pAddSelfMapping( TypeId::xIdOf(CT_INTEGER))
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
//  Postgres seems to support other enhancements:
//    negation of Time

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

QueryNode* NegationExpr::pCreate(QueryNode* pExpr1)
{
  _static_

  // Create a unique name for this object
  s256 sName;
  NodeNamer::vGenerateName(C_NEGATION_DEFN, sName);

  QueryNode* pExpr = new QueryNode(sName, C_NEGATION_DEFN);
  pExpr->vAppendChild(pExpr1);

  return pExpr;
}


/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void NegationExpr::vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));

  QueryNode* pChild1 = NegationExpr::pChild1(_pQueryNode(pNode));

  sBuf.vConcat("-"_Stz);

  if (pChild1->nChildren() == 0 || SubqueryRefExpr::bIsOfType(pChild1)) {
    Expr::vDescribe(pChild1, sBuf, pRootNode);

  } else {
    sBuf.vConcat("("_Stz);
    Expr::vDescribe(pChild1, sBuf, pRootNode);
    sBuf.vConcat(")"_Stz);
  }
}

/*----------------------------------------------------------------------------*/
/* Represent the expression as types instead of names. Append to the buffer   */
/*----------------------------------------------------------------------------*/
void NegationExpr::vDescribeAsTypes(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));

  QueryNode* pChild1 = NegationExpr::pChild1(_pQueryNode(pNode));

  sBuf.vConcat("-"_Stz);
  sBuf.vConcat(Expr::xTypeId(pChild1, pRootNode).szTypeName());
}

/*----------------------------------------------------------------------------*/
/* Return the first child expression of this operator                         */
/*----------------------------------------------------------------------------*/
QueryNode* NegationExpr::pChild1(QueryNode *pNode)
{
  _static_
  ASSERT(bIsOfType(pNode));
  ASSERT(pNode->nChildren() == 1);

  return _pQueryNode(pNode->pFirstChild());
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the expr definition         */
/*----------------------------------------------------------------------------*/
b1 NegationExpr::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_NEGATION_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 NegationExpr::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_NEGATION_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns the resulting type of the expression operation.                    */
/* A returned value of CT_NONE means that it is not supported or invalid.     */
/*----------------------------------------------------------------------------*/
TypeId NegationExpr::xTypeId(QueryNode* pExprNode, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pExprNode));

  // Get the type of the child operand
  QueryNode* pChild1 = NegationExpr::pChild1(pExprNode);

  TypeId xType1 = Expr::xTypeId(pChild1, pRootNode);

  // Lookup the type of the result
  TypeId xResult = S_pTypeMap->xLookup(xType1);
  return xResult;
}

/*----------------------------------------------------------------------------*/
/* Returns the precision of the resulting expression or -1 if not set or n/a  */
/*----------------------------------------------------------------------------*/
i8 NegationExpr::iPrecision(QueryNode* pExprNode, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pExprNode));
  ASSERT(pRootNode);

  QueryNode* pChild1 = NegationExpr::pChild1(pExprNode);
  i8 iVal = Expr::iPrecision(pChild1, pRootNode);
  return iVal;
}

/*----------------------------------------------------------------------------*/
/* Returns the scale of the resulting expression or -1 if not set or n/a      */
/*----------------------------------------------------------------------------*/
i8 NegationExpr::iScale(QueryNode* pExprNode, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pExprNode));
  ASSERT(pRootNode);

  QueryNode* pChild1 = NegationExpr::pChild1(pExprNode);
  i8 iVal = Expr::iScale(pChild1, pRootNode);
  return iVal;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
