/*============================================================================*/
/* XorExpr.hpp - a logical XOR expression operator                            */
/*============================================================================*/

#include "BASE.h"
#include "XorExpr.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "NodeNamer.hpp"
#include "SubqueryRefExpr.hpp"
#include "TypeSystem.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_XOR_NAME_PREFIX = "XorExpr"_Stz;
static Stz* C_XOR_DEFN = "/__root__/definitions/object/expr/operator/logic/xor"_Stz;


// What is the resulting type when we combine these inputs?
// * The left header column represents the type of the first operand.
// * The top header row represents the type of the second operand.
// * The corresponding cell in the matrix represents the type of the resultant from this operation.
// * Invalid/unsupported combinations are represented with CT_NONE.
// * Fields marked with /*?*/ are not currently supported but could be.  Refer to
//   the documentation below marked with this symbol for more information.
// * Fields marked with /*X*/ are not currently supported but should be.
static eCT aeTypeCalculus[CT_ENUMS][CT_ENUMS] = {

//                    NONE     BOOLEAN     CHAR     DATE          DECIMAL     FLOAT       INTEGER       INTERVAL_DT  INTERVAL_YM  TIME          TIMESTAMP     VARCHAR  UNKNOWN
/*  0 NONE       */ { CT_NONE, CT_NONE,    CT_NONE, CT_NONE,      CT_NONE,    CT_NONE,    CT_NONE,      CT_NONE,     CT_NONE,     CT_NONE,      CT_NONE,      CT_NONE, CT_NONE    },
/*  1 BOOLEAN    */ { CT_NONE, CT_BOOLEAN, CT_NONE, CT_NONE,      CT_NONE,    CT_NONE,    CT_NONE,      CT_NONE,     CT_NONE,     CT_NONE,      CT_NONE,      CT_NONE, CT_BOOLEAN },
/*  2 CHAR       */ { CT_NONE, CT_NONE,    CT_NONE, CT_NONE,      CT_NONE,    CT_NONE,    CT_NONE,      CT_NONE,     CT_NONE,     CT_NONE,      CT_NONE,      CT_NONE, CT_NONE    },
/*  3 DATE       */ { CT_NONE, CT_NONE,    CT_NONE, CT_NONE,      CT_NONE,    CT_NONE,    CT_NONE,      CT_NONE,     CT_NONE,     CT_NONE,      CT_NONE,      CT_NONE, CT_NONE    },
/*  4 DECIMAL    */ { CT_NONE, CT_NONE,    CT_NONE, CT_NONE,      CT_NONE,    CT_NONE,    CT_NONE,      CT_NONE,     CT_NONE,     CT_NONE,      CT_NONE,      CT_NONE, CT_NONE    },
/*  5 FLOAT      */ { CT_NONE, CT_NONE,    CT_NONE, CT_NONE,      CT_NONE,    CT_NONE,    CT_NONE,      CT_NONE,     CT_NONE,     CT_NONE,      CT_NONE,      CT_NONE, CT_NONE    },
/*  6 INTEGER    */ { CT_NONE, CT_NONE,    CT_NONE, CT_NONE,      CT_NONE,    CT_NONE,    CT_NONE,      CT_NONE,     CT_NONE,     CT_NONE,      CT_NONE,      CT_NONE, CT_NONE    },
/*  7 INTERVAL_DT*/ { CT_NONE, CT_NONE,    CT_NONE, CT_NONE,      CT_NONE,    CT_NONE,    CT_NONE,      CT_NONE,     CT_NONE,     CT_NONE,      CT_NONE,      CT_NONE, CT_NONE    },
/*  8 INTERVAL_YM*/ { CT_NONE, CT_NONE,    CT_NONE, CT_NONE,      CT_NONE,    CT_NONE,    CT_NONE,      CT_NONE,     CT_NONE,     CT_NONE,      CT_NONE,      CT_NONE, CT_NONE    },
/*  9 TIME       */ { CT_NONE, CT_NONE,    CT_NONE, CT_NONE,      CT_NONE,    CT_NONE,    CT_NONE,      CT_NONE,     CT_NONE,     CT_NONE,      CT_NONE,      CT_NONE, CT_NONE    },
/* 10 TIMESTAMP  */ { CT_NONE, CT_NONE,    CT_NONE, CT_NONE,      CT_NONE,    CT_NONE,    CT_NONE,      CT_NONE,     CT_NONE,     CT_NONE,      CT_NONE,      CT_NONE, CT_NONE    },
/* 11 VARCHAR    */ { CT_NONE, CT_NONE,    CT_NONE, CT_NONE,      CT_NONE,    CT_NONE,    CT_NONE,      CT_NONE,     CT_NONE,     CT_NONE,      CT_NONE,      CT_NONE, CT_NONE    },
/* 12 UNKNOWN    */ { CT_NONE, CT_BOOLEAN, CT_NONE, CT_NONE,      CT_NONE,    CT_NONE,    CT_NONE,      CT_NONE,     CT_NONE,     CT_NONE,      CT_NONE,      CT_NONE, CT_BOOLEAN }
};

// References:
// XOR of a NULL literal will produce a NULL *value*, but the output type will still be a boolean.

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Create the necessary plan graph nodes to represent this object             */
/*----------------------------------------------------------------------------*/
QueryNode* XorExpr::pCreate(QueryNode* pExpr1, QueryNode* pExpr2)
{
  _static_

  // Create a unique name for this object
  s256 sName;
  NodeNamer::vGenerateName(C_XOR_DEFN, sName);

  QueryNode* pExpr = new QueryNode(sName, C_XOR_DEFN);
  pExpr->vAppendChild(pExpr1);
  pExpr->vAppendChild(pExpr2);

  return pExpr;
}


/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void XorExpr::vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));

  QueryNode* pChild1 = XorExpr::pChild1(_pQueryNode(pNode));
  QueryNode* pChild2 = XorExpr::pChild2(_pQueryNode(pNode));

  if (pChild1->nChildren() == 0 || SubqueryRefExpr::bIsOfType(pChild1)) {
    Expr::vDescribe(pChild1, sBuf, pRootNode);

  } else {
    sBuf.vConcat("("_Stz);
    Expr::vDescribe(pChild1, sBuf, pRootNode);
    sBuf.vConcat(")"_Stz);
  }

  sBuf.vConcat(" XOR "_Stz);

  if (pChild2->nChildren() == 0 || SubqueryRefExpr::bIsOfType(pChild2)) {
    Expr::vDescribe(pChild2, sBuf, pRootNode);

  } else {
    sBuf.vConcat("("_Stz);
    Expr::vDescribe(pChild2, sBuf, pRootNode);
    sBuf.vConcat(")"_Stz);
  }
}

/*----------------------------------------------------------------------------*/
/* Return the first child expression of this operator                         */
/*----------------------------------------------------------------------------*/
QueryNode* XorExpr::pChild1(QueryNode *pNode)
{
  _static_
  ASSERT(bIsOfType(pNode));
  ASSERT(pNode->nChildren() == 2);

  return _pQueryNode(pNode->pFirstChild());
}

/*----------------------------------------------------------------------------*/
/* Return the second child expression of this operator                         */
/*----------------------------------------------------------------------------*/
QueryNode* XorExpr::pChild2(QueryNode *pNode)
{
  _static_
  ASSERT(bIsOfType(pNode));
  ASSERT(pNode->nChildren() == 2);

  return _pQueryNode(pNode->pSecondChild());
}


/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the expr definition         */
/*----------------------------------------------------------------------------*/
b1 XorExpr::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_XOR_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 XorExpr::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_XOR_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns the resulting type of the expression operation.                    */
/* A returned value of CT_NONE means that it is not supported or invalid.     */
/*----------------------------------------------------------------------------*/
TypeId XorExpr::xTypeId(QueryNode* pExprNode, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pExprNode));

  // Get the types of the two operands
  QueryNode* pChild1 = XorExpr::pChild1(pExprNode);
  QueryNode* pChild2 = XorExpr::pChild2(pExprNode);

  eCT eOperand1 = Expr::xTypeId(pChild1, pRootNode).eCTVal();
  eCT eOperand2 = Expr::xTypeId(pChild2, pRootNode).eCTVal();

  // Lookup the type of the result
  eCT eResult = aeTypeCalculus[eOperand1][eOperand2];

  return TypeId::xIdOf(eResult);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
