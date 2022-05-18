/*============================================================================*/
/* SumExpr.hpp - an SUM() aggregate expression                                */
/*============================================================================*/

#include "BASE.h"
#include "SumExpr.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "GraphNav.hpp"
#include "NodeNamer.hpp"
#include "TypeSystem.hpp"


using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_SUM_EXPR_NAME_PREFIX = "SumExpr"_Stz;
static Stz* C_SUM_EXPR_DEFN = "/__root__/definitions/object/expr/operator/aggregate/sum"_Stz;

// What is the resulting type when we combine these inputs?
// * The top header row represents the type of the first operand.
// * The corresponding cell in the matrix represents the type of the resultant from this operation.
// * Invalid/unsupported combinations are represented with eCT::CT_NONE.
// * Fields marked with /*?*/ are not currently supported but could be.  Refer to
//   the documentation below marked with this symbol for more information.
// * Fields marked with /*X*/ are not currently supported but should be.
static eCT aeTypeCalculus[eCT::CT_ENUMS] =

//    NONE     BOOLEAN     CHAR     DATE     DECIMAL     FLOAT     INTEGER     INTERVAL_DT     INTERVAL_YM     TIME     TIMESTAMP  VARCHAR  UNKNOWN
    { eCT::CT_NONE, eCT::CT_NONE,    eCT::CT_NONE, eCT::CT_NONE, eCT::CT_DECIMAL, eCT::CT_FLOAT, eCT::CT_INTEGER, eCT::CT_NONE/*X*/,   eCT::CT_NONE/*X*/,   eCT::CT_NONE, eCT::CT_NONE,   eCT::CT_NONE, eCT::CT_NONE };

// References:
//
//  The sum of an exact numeric is an implementation defined exact numeric with the same scale
//    - SQL standard 2008 (draft): Part 2 (Foundation), page 537, section 10.9, point (7)(g)(iii)
//
//  The sum of an approximate numeric is an implementation defined approximate numeric
//    - SQL standard 2008 (draft): Part 2 (Foundation), page 537, section 10.9, point (7)(g)(iv)
//
//  The sum of an interval type is an implementation defined interval type
//    - SQL standard 2008 (draft): Part 2 (Foundation), page 537, section 10.9, point (7)(g)(v)

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Helper function to create expression nodes.                                */
/*----------------------------------------------------------------------------*/
QueryNode* SumExpr::pCreate(b1 bIsDistinct, QueryNode* pChildExpr)
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
  NodeNamer::vGenerateName(C_SUM_EXPR_DEFN, sName);

  Value xVal;
  QueryNode* pSumExpr = new QueryNode(sName, C_SUM_EXPR_DEFN, static_cast<Value&&>(xVal), pProperties, pChildren);
  return pSumExpr;
}


/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void SumExpr::vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));

  sBuf.vConcat("SUM("_Stz);

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
/* Return the first child expression of this operator                         */
/*----------------------------------------------------------------------------*/
QueryNode* SumExpr::pChild1(QueryNode *pNode)
{
  _static_
  ASSERT(bIsOfType(pNode));
  ASSERT(pNode->nChildren() == 1);

  return _pQueryNode(pNode->pFirstChild());
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the expr definition         */
/*----------------------------------------------------------------------------*/
b1 SumExpr::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_SUM_EXPR_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 SumExpr::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_SUM_EXPR_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns the resulting type of the expression operation.                    */
/* A returned value of eCT::CT_NONE means that it is not supported or invalid.     */
/*----------------------------------------------------------------------------*/
i8 SumExpr::iType(QueryNode* pExprNode, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pExprNode));

  // Get the types of the operand
  QueryNode* pChild1 = SumExpr::pChild1(pExprNode);

  i8 iOperand1TypeId = Expr::iType(pChild1, pRootNode);
  eCT eOperand1 = TypeSystem::eCTVal(iOperand1TypeId);

  // Lookup the type of the result
  eCT eResult = aeTypeCalculus[eOperand1];

  return TypeSystem::iIdOf(eResult);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
