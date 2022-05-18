/*============================================================================*/
/* ModulusExpr.hpp - an modulus (%) expression operator                       */
/*============================================================================*/

#include "BASE.h"
#include "ModulusExpr.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "NodeNamer.hpp"
#include "SubqueryRefExpr.hpp"
#include "TypeSystem.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_MODULUS_NAME_PREFIX = "ModulusExpr"_Stz;
static Stz* C_MODULUS_DEFN = "/__root__/definitions/object/expr/operator/mathematical/modulus"_Stz;

// What is the resulting type when we combine these inputs?
// * The left header column represents the type of the first operand.
// * The top header row represents the type of the second operand.
// * The corresponding cell in the matrix represents the type of the resultant from this operation.
// * Invalid/unsupported combinations are represented with eCT::CT_NONE.
// * Fields marked with /*?*/ are not currently supported but could be.  Refer to
//   the documentation below marked with this symbol for more information.
// * Fields marked with /*X*/ are not currently supported but should be.
static eCT aeTypeCalculus[eCT::CT_ENUMS][eCT::CT_ENUMS] = {

                 //   NONE     BOOLEAN  CHAR     DATE          DECIMAL       FLOAT       INTEGER       INTERVAL_DT   INTERVAL_YM   TIME          TIMESTAMP     VARCHAR  UNKNOWN
/*  0 NONE       */ { eCT::CT_NONE, eCT::CT_NONE, eCT::CT_NONE, eCT::CT_NONE,      eCT::CT_NONE,      eCT::CT_NONE,    eCT::CT_NONE,      eCT::CT_NONE,      eCT::CT_NONE,      eCT::CT_NONE,      eCT::CT_NONE,      eCT::CT_NONE, eCT::CT_NONE    },
/*  1 BOOLEAN    */ { eCT::CT_NONE, eCT::CT_NONE, eCT::CT_NONE, eCT::CT_NONE,      eCT::CT_NONE,      eCT::CT_NONE,    eCT::CT_NONE,      eCT::CT_NONE,      eCT::CT_NONE,      eCT::CT_NONE,      eCT::CT_NONE,      eCT::CT_NONE, eCT::CT_NONE    },
/*  2 CHAR       */ { eCT::CT_NONE, eCT::CT_NONE, eCT::CT_NONE, eCT::CT_NONE,      eCT::CT_NONE,      eCT::CT_NONE,    eCT::CT_NONE,      eCT::CT_NONE,      eCT::CT_NONE,      eCT::CT_NONE,      eCT::CT_NONE,      eCT::CT_NONE, eCT::CT_NONE    },
/*  3 DATE       */ { eCT::CT_NONE, eCT::CT_NONE, eCT::CT_NONE, eCT::CT_NONE,      eCT::CT_NONE,      eCT::CT_NONE,    eCT::CT_NONE,      eCT::CT_NONE,      eCT::CT_NONE,      eCT::CT_NONE,      eCT::CT_NONE,      eCT::CT_NONE, eCT::CT_NONE    },
/*  4 DECIMAL    */ { eCT::CT_NONE, eCT::CT_NONE, eCT::CT_NONE, eCT::CT_NONE,      eCT::CT_NONE,      eCT::CT_NONE,    eCT::CT_NONE,      eCT::CT_NONE,      eCT::CT_NONE,      eCT::CT_NONE,      eCT::CT_NONE,      eCT::CT_NONE, eCT::CT_NONE    },
/*  5 FLOAT      */ { eCT::CT_NONE, eCT::CT_NONE, eCT::CT_NONE, eCT::CT_NONE,      eCT::CT_NONE,      eCT::CT_NONE,    eCT::CT_NONE,      eCT::CT_NONE,      eCT::CT_NONE,      eCT::CT_NONE,      eCT::CT_NONE,      eCT::CT_NONE, eCT::CT_NONE    },
/*  6 INTEGER    */ { eCT::CT_NONE, eCT::CT_NONE, eCT::CT_NONE, eCT::CT_NONE,      eCT::CT_NONE,      eCT::CT_NONE,    eCT::CT_INTEGER,   eCT::CT_NONE,      eCT::CT_NONE,      eCT::CT_NONE,      eCT::CT_NONE,      eCT::CT_NONE, eCT::CT_UNKNOWN },
/*  7 INTERVAL_DT*/ { eCT::CT_NONE, eCT::CT_NONE, eCT::CT_NONE, eCT::CT_NONE,      eCT::CT_NONE,      eCT::CT_NONE,    eCT::CT_NONE,      eCT::CT_NONE,      eCT::CT_NONE,      eCT::CT_NONE,      eCT::CT_NONE,      eCT::CT_NONE, eCT::CT_NONE    },
/*  8 INTERVAL_YM*/ { eCT::CT_NONE, eCT::CT_NONE, eCT::CT_NONE, eCT::CT_NONE,      eCT::CT_NONE,      eCT::CT_NONE,    eCT::CT_NONE,      eCT::CT_NONE,      eCT::CT_NONE,      eCT::CT_NONE,      eCT::CT_NONE,      eCT::CT_NONE, eCT::CT_NONE    },
/*  8 TIME       */ { eCT::CT_NONE, eCT::CT_NONE, eCT::CT_NONE, eCT::CT_NONE,      eCT::CT_NONE,      eCT::CT_NONE,    eCT::CT_NONE,      eCT::CT_NONE,      eCT::CT_NONE,      eCT::CT_NONE,      eCT::CT_NONE,      eCT::CT_NONE, eCT::CT_NONE    },
/*  9 TIMESTAMP  */ { eCT::CT_NONE, eCT::CT_NONE, eCT::CT_NONE, eCT::CT_NONE,      eCT::CT_NONE,      eCT::CT_NONE,    eCT::CT_NONE,      eCT::CT_NONE,      eCT::CT_NONE,      eCT::CT_NONE,      eCT::CT_NONE,      eCT::CT_NONE, eCT::CT_NONE    },
/* 10 VARCHAR    */ { eCT::CT_NONE, eCT::CT_NONE, eCT::CT_NONE, eCT::CT_NONE,      eCT::CT_NONE,      eCT::CT_NONE,    eCT::CT_NONE,      eCT::CT_NONE,      eCT::CT_NONE,      eCT::CT_NONE,      eCT::CT_NONE,      eCT::CT_NONE, eCT::CT_NONE    },
/* 11 UNKNOWN    */ { eCT::CT_NONE, eCT::CT_NONE, eCT::CT_NONE, eCT::CT_NONE,      eCT::CT_NONE,      eCT::CT_NONE,    eCT::CT_UNKNOWN,   eCT::CT_NONE,      eCT::CT_NONE,      eCT::CT_NONE,      eCT::CT_NONE,      eCT::CT_NONE, eCT::CT_NONE    }
};

// References:
//  Exact numeric % exact numeric = exact numeric
//    - SQL standard 2008 (draft): Part 2 (Foundation), section 4.4.3, page 32


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

QueryNode* ModulusExpr::pCreate(QueryNode* pExpr1, QueryNode* pExpr2)
{
  _static_

  // Create a unique name for this object
  s256 sName;
  NodeNamer::vGenerateName(C_MODULUS_DEFN, sName);

  QueryNode* pExpr = new QueryNode(sName, C_MODULUS_DEFN);
  pExpr->vAppendChild(pExpr1);
  pExpr->vAppendChild(pExpr2);

  return pExpr;
}


/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void ModulusExpr::vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));

  QueryNode* pChild1 = ModulusExpr::pChild1(_pQueryNode(pNode));
  QueryNode* pChild2 = ModulusExpr::pChild2(_pQueryNode(pNode));

  if (pChild1->nChildren() == 0 || SubqueryRefExpr::bIsOfType(pChild1)) {
    Expr::vDescribe(pChild1, sBuf, pRootNode);

  } else {
    sBuf.vConcat("("_Stz);
    Expr::vDescribe(pChild1, sBuf, pRootNode);
    sBuf.vConcat(")"_Stz);
  }

  sBuf.vConcat(" % "_Stz);

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
QueryNode* ModulusExpr::pChild1(QueryNode *pNode)
{
  _static_
  ASSERT(bIsOfType(pNode));
  ASSERT(pNode->nChildren() == 2);

  return _pQueryNode(pNode->pFirstChild());
}

/*----------------------------------------------------------------------------*/
/* Return the second child expression of this operator                         */
/*----------------------------------------------------------------------------*/
QueryNode* ModulusExpr::pChild2(QueryNode *pNode)
{
  _static_
  ASSERT(bIsOfType(pNode));
  ASSERT(pNode->nChildren() == 2);

  return _pQueryNode(pNode->pSecondChild());
}


/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the expr definition         */
/*----------------------------------------------------------------------------*/
b1 ModulusExpr::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_MODULUS_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 ModulusExpr::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_MODULUS_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns the resulting type of the expression operation.                    */
/* A returned value of eCT::CT_NONE means that it is not supported or invalid.     */
/*----------------------------------------------------------------------------*/
i8 ModulusExpr::iType(QueryNode* pExprNode, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pExprNode));

  // Get the types of the two operands
  QueryNode* pChild1 = ModulusExpr::pChild1(pExprNode);
  QueryNode* pChild2 = ModulusExpr::pChild2(pExprNode);

  eCT eOperand1 = TypeSystem::eCTVal(Expr::iType(pChild1, pRootNode));
  eCT eOperand2 = TypeSystem::eCTVal(Expr::iType(pChild2, pRootNode));

  // Lookup the type of the result
  eCT eResult = aeTypeCalculus[eOperand1][eOperand2];

  return TypeSystem::iIdOf(eResult);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
