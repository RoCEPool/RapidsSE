/*============================================================================*/
/* NotExpr.hpp - a logical NOT expression operator                            */
/*============================================================================*/

#include "BASE.h"
#include "NotExpr.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "NodeNamer.hpp"
#include "SubqueryRefExpr.hpp"
#include "TypeSystem.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_NOT_NAME_PREFIX = "NotExpr"_Stz;
static Stz* C_NOT_DEFN = "/__root__/definitions/object/expr/operator/logic/not"_Stz;

// What is the resulting type when we combine these inputs?
// * The top header row represents the type of the first operand.
// * The corresponding cell in the matrix represents the type of the resultant from this operation.
// * Invalid/unsupported combinations are represented with CT_NONE.
// * Fields marked with /*?*/ are not currently supported but could be.  Refer to
//   the documentation below marked with this symbol for more information.
// * Fields marked with /*X*/ are not currently supported but should be.
static eCT aeTypeCalculus[CT_ENUMS] =

//    NONE     BOOLEAN     CHAR     DATE     DECIMAL  FLOAT    INTEGER  INTERVAL_DT INTERVAL_YM TIME     TIMESTAMP  VARCHAR  UNKNOWN
    { CT_NONE, CT_BOOLEAN, CT_NONE, CT_NONE, CT_NONE, CT_NONE, CT_NONE, CT_NONE,    CT_NONE,    CT_NONE, CT_NONE,   CT_NONE, CT_BOOLEAN };

// References:
//  NOT of a NULL literal will produce a NULL value, but the output type will still be a boolean.

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Create the necessary plan graph nodes to represent this object             */
/*----------------------------------------------------------------------------*/
QueryNode* NotExpr::pCreate(QueryNode* pExpr1)
{
  _static_

  // Create a unique name for this object
  s256 sName;
  NodeNamer::vGenerateName(C_NOT_DEFN, sName);

  QueryNode* pExpr = new QueryNode(sName, C_NOT_DEFN);
  pExpr->vAppendChild(pExpr1);

  return pExpr;
}


/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void NotExpr::vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));

  QueryNode* pChild1 = NotExpr::pChild1(_pQueryNode(pNode));

  sBuf.vConcat("NOT "_Stz);

  if (pChild1->nChildren() == 0 || SubqueryRefExpr::bIsOfType(pChild1)) {
    Expr::vDescribe(pChild1, sBuf, pRootNode);

  } else {
    sBuf.vConcat("("_Stz);
    Expr::vDescribe(pChild1, sBuf, pRootNode);
    sBuf.vConcat(")"_Stz);
  }
}

/*----------------------------------------------------------------------------*/
/* Return the first child expression of this operator                         */
/*----------------------------------------------------------------------------*/
QueryNode* NotExpr::pChild1(QueryNode *pNode)
{
  _static_
  ASSERT(bIsOfType(pNode));
  ASSERT(pNode->nChildren() == 1);

  return _pQueryNode(pNode->pFirstChild());
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the expr definition         */
/*----------------------------------------------------------------------------*/
b1 NotExpr::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_NOT_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 NotExpr::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_NOT_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns the resulting type of the expression operation.                    */
/* A returned value of CT_NONE means that it is not supported or invalid.     */
/*----------------------------------------------------------------------------*/
TypeId NotExpr::xTypeId(QueryNode* pExprNode, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pExprNode));

  // Get the types of the two operands
  QueryNode* pChild1 = NotExpr::pChild1(pExprNode);

  eCT eOperand1 = Expr::xTypeId(pChild1, pRootNode).eCTVal();

  // Lookup the type of the result
  eCT eResult = aeTypeCalculus[eOperand1];

  return TypeId::xIdOf(eResult);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
