/*============================================================================*/
/* MinExpr.hpp - an MIN() aggregate expression                                */
/*============================================================================*/

#include "BASE.h"
#include "MinExpr.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "GraphNav.hpp"
#include "NodeNamer.hpp"
#include "TypeSystem.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_MIN_EXPR_NAME_PREFIX = "MinExpr"_Stz;
static Stz* C_MIN_EXPR_DEFN = "/__root__/definitions/object/expr/operator/aggregate/min"_Stz;

// What is the resulting type when we combine these inputs?
// * The top header row represents the type of the first operand.
// * The corresponding cell in the matrix represents the type of the resultant from this operation.
// * Invalid/unsupported combinations are represented with CT_NONE.
// * Fields marked with /*?*/ are not currently supported but could be.  Refer to
//   the documentation below marked with this symbol for more information.
// * Fields marked with /*X*/ are not currently supported but should be.
static eCT aeTypeCalculus[CT_ENUMS] =

//    NONE     BOOLEAN       CHAR        DATE     DECIMAL     FLOAT     INTEGER     INTERVAL_DT     INTERVAL_YM     TIME     TIMESTAMP     VARCHAR     UNKNOWN
    { CT_NONE, CT_NONE/*X*/, CT_VARCHAR, CT_DATE, CT_DECIMAL, CT_FLOAT, CT_INTEGER, CT_NONE/*X*/,   CT_NONE/*X*/,   CT_TIME, CT_TIMESTAMP, CT_VARCHAR, CT_NONE/*X*/ };

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
QueryNode* MinExpr::pCreate(b1 bIsDistinct, QueryNode* pChildExpr)
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
  NodeNamer::vGenerateName(C_MIN_EXPR_DEFN, sName);

  Value xVal;
  QueryNode* pMinExpr = new QueryNode(sName, C_MIN_EXPR_DEFN, static_cast<Value&&>(xVal), pProperties, pChildren);
  return pMinExpr;
}


/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void MinExpr::vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));

  sBuf.vConcat("MIN("_Stz);

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
QueryNode* MinExpr::pChild1(QueryNode *pNode)
{
  _static_
  ASSERT(bIsOfType(pNode));
  ASSERT(pNode->nChildren() == 1);

  return _pQueryNode(pNode->pFirstChild());
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the expr definition         */
/*----------------------------------------------------------------------------*/
b1 MinExpr::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_MIN_EXPR_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 MinExpr::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_MIN_EXPR_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns the resulting type of the expression operation.                    */
/* A returned value of CT_NONE means that it is not supported or invalid.     */
/*----------------------------------------------------------------------------*/
i8 MinExpr::iType(QueryNode* pExprNode, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pExprNode));

  // Get the types of the operand
  QueryNode* pChild1 = MinExpr::pChild1(pExprNode);

  i8 iOperand1TypeId = Expr::iType(pChild1, pRootNode);
  eCT eOperand1 = TypeSystem::eCTVal(iOperand1TypeId);

  // Lookup the type of the result
  eCT eResult = aeTypeCalculus[eOperand1];

  return TypeSystem::iIdOf(eResult);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
