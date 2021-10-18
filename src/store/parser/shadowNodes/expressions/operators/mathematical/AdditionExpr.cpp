/*============================================================================*/
/* AdditionExpr.hpp - an addition (+) expression operator                     */
/*============================================================================*/

#include "BASE.h"
#include "AdditionExpr.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "eCT.h"
#include "NodeNamer.hpp"
#include "SubqueryRefExpr.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_ADDITION_NAME_PREFIX = "AdditionExpr"_Stz;
static Stz* C_ADDITION_DEFN = "/__root__/definitions/object/expr/operator/mathematical/addition"_Stz;

// What is the resulting type when we combine these inputs?
// * The left header column represents the type of the first operand.
// * The top header row represents the type of the second operand.
// * The corresponding cell in the matrix represents the type of the resultant from this operation.
// * Invalid/unsupported combinations are represented with CT_NONE.
// * Fields marked with /*?*/ are not currently supported but could be.  Refer to
//   the documentation below marked with this symbol for more information.
// * Fields marked with /*X*/ are not currently supported but should be.
static eCT aeTypeCalculus[CT_ENUMS][CT_ENUMS] = {

                 //   NONE     BOOLEAN  CHAR     DATE          DECIMAL     FLOAT       INTEGER       INTERVAL_DT     INTERVAL_YM     TIME          TIMESTAMP     VARCHAR  UNKNOWN
/*  0 NONE       */ { CT_NONE, CT_NONE, CT_NONE, CT_NONE,      CT_NONE,    CT_NONE,    CT_NONE,      CT_NONE,        CT_NONE,        CT_NONE,      CT_NONE,      CT_NONE, CT_NONE    },
/*  1 BOOLEAN    */ { CT_NONE, CT_NONE, CT_NONE, CT_NONE,      CT_NONE,    CT_NONE,    CT_NONE,      CT_NONE,        CT_NONE,        CT_NONE,      CT_NONE,      CT_NONE, CT_NONE    },
/*  2 CHAR       */ { CT_NONE, CT_NONE, CT_NONE, CT_NONE,      CT_NONE,    CT_NONE,    CT_NONE,      CT_NONE,        CT_NONE,        CT_NONE,      CT_NONE,      CT_NONE, CT_NONE    },
/*  3 DATE       */ { CT_NONE, CT_NONE, CT_NONE, CT_NONE,      CT_NONE,    CT_NONE,    CT_NONE/*?*/, CT_DATE,        CT_DATE,        CT_NONE/*?*/, CT_NONE,      CT_NONE, CT_NONE    },
/*  4 DECIMAL    */ { CT_NONE, CT_NONE, CT_NONE, CT_NONE,      CT_DECIMAL, CT_FLOAT,   CT_DECIMAL,   CT_NONE,        CT_NONE,        CT_NONE,      CT_NONE,      CT_NONE, CT_UNKNOWN },
/*  5 FLOAT      */ { CT_NONE, CT_NONE, CT_NONE, CT_NONE,      CT_FLOAT,   CT_FLOAT,   CT_FLOAT,     CT_NONE,        CT_NONE,        CT_NONE,      CT_NONE,      CT_NONE, CT_UNKNOWN },
/*  6 INTEGER    */ { CT_NONE, CT_NONE, CT_NONE, CT_NONE/*?*/, CT_DECIMAL, CT_FLOAT,   CT_INTEGER,   CT_NONE,        CT_NONE,        CT_NONE,      CT_NONE,      CT_NONE, CT_UNKNOWN },
/*  7 INTERVAL_DT*/ { CT_NONE, CT_NONE, CT_NONE, CT_DATE,      CT_NONE,    CT_NONE,    CT_NONE,      CT_INTERVAL_DT, CT_NONE,        CT_TIME,      CT_TIMESTAMP, CT_NONE, CT_UNKNOWN },
/*  8 INTERVAL_YM*/ { CT_NONE, CT_NONE, CT_NONE, CT_DATE,      CT_NONE,    CT_NONE,    CT_NONE,      CT_NONE,        CT_INTERVAL_YM, CT_TIME,      CT_TIMESTAMP, CT_NONE, CT_UNKNOWN },
/*  9 TIME       */ { CT_NONE, CT_NONE, CT_NONE, CT_NONE/*?*/, CT_NONE,    CT_NONE,    CT_NONE/*?*/, CT_TIME,        CT_TIME,        CT_NONE,      CT_NONE,      CT_NONE, CT_NONE    },
/* 10 TIMESTAMP  */ { CT_NONE, CT_NONE, CT_NONE, CT_NONE,      CT_NONE,    CT_NONE,    CT_NONE,      CT_TIMESTAMP,   CT_TIMESTAMP,   CT_NONE,      CT_NONE,      CT_NONE, CT_NONE    },
/* 11 VARCHAR    */ { CT_NONE, CT_NONE, CT_NONE, CT_NONE,      CT_NONE,    CT_NONE,    CT_NONE,      CT_NONE,        CT_NONE,        CT_NONE,      CT_NONE,      CT_NONE, CT_NONE    },
/* 12 UNKNOWN    */ { CT_NONE, CT_NONE, CT_NONE, CT_NONE,      CT_UNKNOWN, CT_UNKNOWN, CT_UNKNOWN,   CT_UNKNOWN,     CT_UNKNOWN,     CT_NONE,      CT_NONE,      CT_NONE, CT_NONE    }
};

// References:
//  Exact numeric + exact numeric = exact numeric
//    - SQL standard 2008 (draft): Part 2 (Foundation), page 251, point (1)
//
//  Approximate numeric + exact numeric (any order) = approximate numeric:
//    - SQL standard 2008 (draft): Part 2 (Foundation), page 251, point (2)
//
//  Datetime & Intervals arithmetic:
//    Date      +/-     Interval  = Date
//    Time      +/-     Interval  = Time
//    Timestamp +/-     Interval  = Timestamp
//    Date      -       Date      = Interval_DT
//    Time      -       Time      = Interval_DT
//    Timestamp -       Timestamp = Interval_DT
//    Interval  +       Date      = Date
//    Interval  +       Time      = Time
//    Interval  +       Timestamp = Timestamp
//    Interval  +/-     Interval  = Interval
//    Interval  * or /  Numeric   = Interval
//    Numeric   *       Interval  = Interval
//     - SQL standard 2008 (draft): Part 2 (Foundation), page 39 (table 7)
//
//  /*?*/
//  Postgres seems to support other enhancements, marked as /*?*/ in the table above:
//    Date      +       Time      = Timestamp
//    Date      +       Integer   = Date  (integer is assumed to be in days)
//    Time      +       Date      = Timestamp

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

QueryNode* AdditionExpr::pCreate(QueryNode* pExpr1, QueryNode* pExpr2)
{
  _static_

  // Create a unique name for this object
  s256 sName;
  NodeNamer::vGenerateName(C_ADDITION_DEFN, sName);

  QueryNode* pExpr = new QueryNode(sName, C_ADDITION_DEFN);
  pExpr->vAppendChild(pExpr1);
  pExpr->vAppendChild(pExpr2);

  return pExpr;
}


/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void AdditionExpr::vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));

  QueryNode* pChild1 = AdditionExpr::pChild1(_pQueryNode(pNode));
  QueryNode* pChild2 = AdditionExpr::pChild2(_pQueryNode(pNode));

  if (pChild1->nChildren() == 0 || SubqueryRefExpr::bIsOfType(pChild1)) {
    Expr::vDescribe(pChild1, sBuf, pRootNode);

  } else {
    sBuf.vConcat("("_Stz);
    Expr::vDescribe(pChild1, sBuf, pRootNode);
    sBuf.vConcat(")"_Stz);
  }

  sBuf.vConcat(" + "_Stz);

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
QueryNode* AdditionExpr::pChild1(QueryNode *pNode)
{
  _static_
  ASSERT(bIsOfType(pNode));
  ASSERT(pNode->nChildren() == 2);

  return _pQueryNode(pNode->pFirstChild());
}

/*----------------------------------------------------------------------------*/
/* Return the second child expression of this operator                         */
/*----------------------------------------------------------------------------*/
QueryNode* AdditionExpr::pChild2(QueryNode *pNode)
{
  _static_
  ASSERT(bIsOfType(pNode));
  ASSERT(pNode->nChildren() == 2);

  return _pQueryNode(pNode->pSecondChild());
}


/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the expr definition         */
/*----------------------------------------------------------------------------*/
b1 AdditionExpr::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_ADDITION_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 AdditionExpr::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_ADDITION_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns the resulting type of the expression operation.                    */
/* A returned value of CT_NONE means that it is not supported or invalid.     */
/*----------------------------------------------------------------------------*/
eCT AdditionExpr::eType(QueryNode* pExprNode, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pExprNode));

  // Get the types of the two operands
  QueryNode* pChild1 = AdditionExpr::pChild1(pExprNode);
  QueryNode* pChild2 = AdditionExpr::pChild2(pExprNode);

  eCT eOperand1 = Expr::eType(pChild1, pRootNode);
  eCT eOperand2 = Expr::eType(pChild2, pRootNode);

  // Lookup the type of the result
  eCT eResult = aeTypeCalculus[eOperand1][eOperand2];

  return eResult;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
