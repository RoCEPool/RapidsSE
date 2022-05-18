/*============================================================================*/
/* InExpr.hpp - an IN expression operator                                     */
/*============================================================================*/

#include "BASE.h"
#include "InExpr.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "NodeNamer.hpp"
#include "SubqueryRefExpr.hpp"
#include "SqlException.hpp"
#include "TypeSystem.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_NAME_PREFIX = "inExpr"_Stz;
static Stz* C_DEFN = "/__root__/definitions/object/expr/operator/comparison/in"_Stz;

// What is the resulting type when we combine these inputs?
// * The left header column represents the type of the first operand.
// * The top header row represents the type of the second operand.
// * The corresponding cell in the matrix represents the type of the resultant from this operation.
// * Invalid/unsupported combinations are represented with eCT::CT_NONE.
// * Fields marked with /*?*/ are not currently supported but could be.  Refer to
//   the documentation below marked with this symbol for more information.
// * Fields marked with /*X*/ are not currently supported but should be.
static eCT aeTypeCalculus[eCT::CT_ENUMS][eCT::CT_ENUMS] = {

//                    NONE     BOOLEAN     CHAR        DATE          DECIMAL     FLOAT       INTEGER       INTERVAL_DT  INTERVAL_YM     TIME          TIMESTAMP     VARCHAR     UNKNOWN
/*  0 NONE       */ { eCT::CT_NONE, eCT::CT_NONE,    eCT::CT_NONE,    eCT::CT_NONE,      eCT::CT_NONE,    eCT::CT_NONE,    eCT::CT_NONE,      eCT::CT_NONE,     eCT::CT_NONE,        eCT::CT_NONE,      eCT::CT_NONE,      eCT::CT_NONE,    eCT::CT_NONE    },
/*  1 BOOLEAN    */ { eCT::CT_NONE, eCT::CT_BOOLEAN, eCT::CT_NONE,    eCT::CT_NONE,      eCT::CT_NONE,    eCT::CT_NONE,    eCT::CT_NONE,      eCT::CT_NONE,     eCT::CT_NONE,        eCT::CT_NONE,      eCT::CT_NONE,      eCT::CT_NONE,    eCT::CT_BOOLEAN },
/*  2 CHAR       */ { eCT::CT_NONE, eCT::CT_NONE,    eCT::CT_BOOLEAN, eCT::CT_NONE,      eCT::CT_NONE,    eCT::CT_NONE,    eCT::CT_NONE,      eCT::CT_NONE,     eCT::CT_NONE,        eCT::CT_NONE,      eCT::CT_NONE,      eCT::CT_BOOLEAN, eCT::CT_BOOLEAN },
/*  3 DATE       */ { eCT::CT_NONE, eCT::CT_NONE,    eCT::CT_NONE,    eCT::CT_BOOLEAN,   eCT::CT_NONE,    eCT::CT_NONE,    eCT::CT_NONE,      eCT::CT_NONE,     eCT::CT_NONE,        eCT::CT_NONE,      eCT::CT_NONE,      eCT::CT_NONE,    eCT::CT_BOOLEAN },
/*  4 DECIMAL    */ { eCT::CT_NONE, eCT::CT_NONE,    eCT::CT_NONE,    eCT::CT_NONE,      eCT::CT_BOOLEAN, eCT::CT_NONE,    eCT::CT_NONE,      eCT::CT_NONE,     eCT::CT_NONE,        eCT::CT_NONE,      eCT::CT_NONE,      eCT::CT_NONE,    eCT::CT_BOOLEAN },
/*  5 FLOAT      */ { eCT::CT_NONE, eCT::CT_NONE,    eCT::CT_NONE,    eCT::CT_NONE,      eCT::CT_NONE,    eCT::CT_BOOLEAN, eCT::CT_NONE,      eCT::CT_NONE,     eCT::CT_NONE,        eCT::CT_NONE,      eCT::CT_NONE,      eCT::CT_NONE,    eCT::CT_BOOLEAN },
/*  6 INTEGER    */ { eCT::CT_NONE, eCT::CT_NONE,    eCT::CT_NONE,    eCT::CT_NONE,      eCT::CT_NONE,    eCT::CT_NONE,    eCT::CT_BOOLEAN,   eCT::CT_NONE,     eCT::CT_NONE,        eCT::CT_NONE,      eCT::CT_NONE,      eCT::CT_NONE,    eCT::CT_BOOLEAN },
/*  7 INTERVAL_DT*/ { eCT::CT_NONE, eCT::CT_NONE,    eCT::CT_NONE,    eCT::CT_NONE,      eCT::CT_NONE,    eCT::CT_NONE,    eCT::CT_NONE,      eCT::CT_BOOLEAN,  eCT::CT_NONE,        eCT::CT_NONE,      eCT::CT_NONE,      eCT::CT_NONE,    eCT::CT_BOOLEAN },
/*  8 INTERVAL_YM*/ { eCT::CT_NONE, eCT::CT_NONE,    eCT::CT_NONE,    eCT::CT_NONE,      eCT::CT_NONE,    eCT::CT_NONE,    eCT::CT_NONE,      eCT::CT_NONE,     eCT::CT_BOOLEAN,     eCT::CT_NONE,      eCT::CT_NONE,      eCT::CT_NONE,    eCT::CT_BOOLEAN },
/*  9 TIME       */ { eCT::CT_NONE, eCT::CT_NONE,    eCT::CT_NONE,    eCT::CT_NONE,      eCT::CT_NONE,    eCT::CT_NONE,    eCT::CT_NONE,      eCT::CT_NONE,     eCT::CT_NONE,        eCT::CT_BOOLEAN,   eCT::CT_NONE,      eCT::CT_NONE,    eCT::CT_BOOLEAN },
/* 10 TIMESTAMP  */ { eCT::CT_NONE, eCT::CT_NONE,    eCT::CT_NONE,    eCT::CT_NONE,      eCT::CT_NONE,    eCT::CT_NONE,    eCT::CT_NONE,      eCT::CT_NONE,     eCT::CT_NONE,        eCT::CT_NONE,      eCT::CT_BOOLEAN,   eCT::CT_NONE,    eCT::CT_BOOLEAN },
/* 11 VARCHAR    */ { eCT::CT_NONE, eCT::CT_NONE,    eCT::CT_BOOLEAN, eCT::CT_NONE,      eCT::CT_NONE,    eCT::CT_NONE,    eCT::CT_NONE,      eCT::CT_NONE,     eCT::CT_NONE,        eCT::CT_NONE,      eCT::CT_NONE,      eCT::CT_BOOLEAN, eCT::CT_BOOLEAN },
/* 12 UNKNOWN    */ { eCT::CT_NONE, eCT::CT_BOOLEAN, eCT::CT_BOOLEAN, eCT::CT_BOOLEAN,   eCT::CT_BOOLEAN, eCT::CT_BOOLEAN, eCT::CT_BOOLEAN,   eCT::CT_BOOLEAN,  eCT::CT_BOOLEAN,     eCT::CT_BOOLEAN,   eCT::CT_BOOLEAN,   eCT::CT_BOOLEAN, eCT::CT_BOOLEAN }
};

// References:


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

QueryNode* InExpr::pCreate(QueryNode* pLeftExpr)
{
  _static_
  ASSERT(pLeftExpr);
  ASSERT(Expr::bIsOfType(pLeftExpr));

  // Create a unique name for this object
  s256 sName;
  NodeNamer::vGenerateName(C_DEFN, sName);

  QueryNode* pInExpr = new QueryNode(sName, C_DEFN);
  pInExpr->vAppendChild(pLeftExpr);

  return pInExpr;
}


/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void InExpr::vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));

  i8 nChildren = pNode->nChildren();
  ASSERT(nChildren > 0);

  s2K sLeftExprBuf;
  s2K sRightExprBuf;

  Expr::vDescribe(pLeftExpr(pNode), sLeftExprBuf, pRootNode);

  if (nChildren == 2 && SubqueryRefExpr::bIsOfType(pNode->pSecondChild())) {
    QueryNode* pChild = _pQueryNode(pNode->pSecondChild());
    ASSERT(Expr::bIsOfType(pChild));

    Expr::vDescribe(pChild, sRightExprBuf, pRootNode);

  } else {
    sRightExprBuf.vConcat("("_Stz);

    PlanNodeIter* pIter = pRightExprIter(pNode);
    b1 bIsFirst = true;
    s2K sItemBuf;

    while (pIter->bHasNext()) {
      QueryNode* pChild = _pQueryNode(pIter->pNext());
      ASSERT(Expr::bIsOfType(pChild));

      if (bIsFirst) {
        bIsFirst = false;
      } else {
        sRightExprBuf.vConcat(", "_Stz);
      }

      sItemBuf.vReset();
      Expr::vDescribe(pChild, sItemBuf, pRootNode);
      sRightExprBuf.vConcat(sItemBuf.szStz());
    }

    delete pIter;
    sRightExprBuf.vConcat(")"_Stz);
  }

  sBuf.vCatSprint("%s IN %s"_Stz, sLeftExprBuf.szStz(), sRightExprBuf.szStz());
}


/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the expr definition         */
/*----------------------------------------------------------------------------*/
b1 InExpr::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 InExpr::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns the expression left of the IN clause that is being compared        */
/* e.g. leftExpr IN (rightExpr1, rightExpr2, ...)                             */
/*----------------------------------------------------------------------------*/
QueryNode* InExpr::pLeftExpr(PlanNode* pNode)
{
  ASSERT(bIsOfType(pNode));
  ASSERT(pNode->nChildren() >= 2);

  return _pQueryNode(pNode->pFirstChild());
}

/*----------------------------------------------------------------------------*/
/* Returns an iterator starting at the first expression to the right of       */
/* the IN clause. e.g. leftExpr IN (rightExpr1, rightExpr2, ...)              */
/* Iterator must be freed after use.                                          */
/*----------------------------------------------------------------------------*/
PlanNodeIter* InExpr::pRightExprIter(PlanNode* pNode)
{
  ASSERT(bIsOfType(pNode));
  ASSERT(pNode->nChildren() >= 2);

  PlanNodeIter* pIter = pNode->pNewChildrenIter();
  ASSERT(pIter->bHasNext());
  pIter->pNext();
  ASSERT(pIter->bHasNext());

  return pIter;
}

/*----------------------------------------------------------------------------*/
/* Returns the resulting type of the expression operation.                    */
/* A returned value of eCT::CT_NONE means that it is not supported or invalid.     */
/*----------------------------------------------------------------------------*/
i8 InExpr::iType(QueryNode* pExprNode, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pExprNode));

  // Get the types of the two operands
  // Assuming the type of the second child is the same as each subsequent child
  QueryNode* pChild1 = _pQueryNode(pExprNode->pFirstChild());
  QueryNode* pChild2 = _pQueryNode(pExprNode->pSecondChild());

  eCT eOperand1 = TypeSystem::eCTVal(Expr::iType(pChild1, pRootNode));
  eCT eOperand2 = TypeSystem::eCTVal(Expr::iType(pChild2, pRootNode));

  // Lookup the type of the result
  eCT eResult = aeTypeCalculus[eOperand1][eOperand2];

  if (eResult == eCT::CT_NONE || pExprNode->nChildren() <= 2) {
    return eResult;
  }

  // Check that all subsequent operands are of the same type as eOperand2
  PlanNodeIter* pIter = pExprNode->pNewChildrenIter();
  ASSERT(pIter->bHasNext()); pIter->pNext();  // pIter points to 1st child
  ASSERT(pIter->bHasNext()); pIter->pNext();  // pIter points to 2nd child

  while (pIter->bHasNext()) {
    QueryNode* pChild = _pQueryNode(pIter->pNext());
    ASSERT(Expr::bIsOfType(pChild));

    eCT eChildType = TypeSystem::eCTVal(Expr::iType(pChild, pRootNode));

    if (eChildType != eOperand2) {
      eResult = eCT::CT_NONE;
      break;
    }
  }

  delete pIter;
  return TypeSystem::iIdOf(eResult);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
