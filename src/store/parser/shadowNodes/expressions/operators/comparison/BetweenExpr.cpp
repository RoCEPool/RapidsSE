/*============================================================================*/
/* BetweenExpr.hpp - a BETWEEN expression operator                            */
/*============================================================================*/

#include "BASE.h"
#include "BetweenExpr.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "NodeNamer.hpp"
#include "SubqueryRefExpr.hpp"
#include "GraphNav.hpp"
#include "TypeSystem.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_NAME_PREFIX = "BetweenExpr"_Stz;
static Stz* C_DEFN = "/__root__/definitions/object/expr/operator/comparison/between"_Stz;


// What is the resulting type when we combine these inputs?
// * The left header column represents the type of the first operand (predicand)
// * The top header row represents the type of the second operand (lower/upper bound)
// * The corresponding cell in the matrix represents the type of the resultant from this operation.
// * Invalid/unsupported combinations are represented with eCT::CT_NONE.
// * Fields marked with /*?*/ are not currently supported but could be.  Refer to
//   the documentation below marked with this symbol for more information.
// * Fields marked with /*X*/ are not currently supported but should be.
static eCT aeTypeCalculus[eCT::CT_ENUMS][eCT::CT_ENUMS] = {

//                    NONE     BOOLEAN     CHAR        DATE          DECIMAL     FLOAT       INTEGER       INTERVAL_DT  INTERVAL_YM     TIME          TIMESTAMP     VARCHAR     UNKNOWN
/*  0 NONE       */ { eCT::CT_NONE, eCT::CT_NONE,    eCT::CT_NONE,    eCT::CT_NONE,      eCT::CT_NONE,    eCT::CT_NONE,    eCT::CT_NONE,      eCT::CT_NONE,     eCT::CT_NONE,        eCT::CT_NONE,      eCT::CT_NONE,      eCT::CT_NONE,    eCT::CT_NONE    },
/*  1 BOOLEAN    */ { eCT::CT_NONE, eCT::CT_NONE,    eCT::CT_NONE,    eCT::CT_NONE,      eCT::CT_NONE,    eCT::CT_NONE,    eCT::CT_NONE,      eCT::CT_NONE,     eCT::CT_NONE,        eCT::CT_NONE,      eCT::CT_NONE,      eCT::CT_NONE,    eCT::CT_NONE    },
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

QueryNode* BetweenExpr::pCreate(QueryNode* pPredicand, QueryNode* pLowerBound, QueryNode* pUpperBound)
{
  _static_
  ASSERT(pPredicand);
  ASSERT(Expr::bIsOfType(pPredicand));
  ASSERT(pLowerBound);
  ASSERT(Expr::bIsOfType(pLowerBound));
  ASSERT(pUpperBound);
  ASSERT(Expr::bIsOfType(pUpperBound));

  // Create a unique name for this object
  s256 sName;
  NodeNamer::vGenerateName(C_DEFN, sName);

  QueryNode* pExpr = new QueryNode(sName, C_DEFN);
  pExpr->vAppendChild(pPredicand);
  pExpr->vAppendChild(pLowerBound);
  pExpr->vAppendChild(pUpperBound);

  return pExpr;
}


/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void BetweenExpr::vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));

  QueryNode* pPredicand  = BetweenExpr::pPredicand(_pQueryNode(pNode));
  QueryNode* pLowerBound = BetweenExpr::pLowerBound(_pQueryNode(pNode));
  QueryNode* pUpperBound = BetweenExpr::pUpperBound(_pQueryNode(pNode));

  s2K sTempBuf;

  if (pPredicand->nChildren() == 0 || SubqueryRefExpr::bIsOfType(pPredicand)) {

    Expr::vDescribe(pPredicand, sTempBuf, pRootNode);
    sBuf.vConcat(sTempBuf.szStz());

  } else {
    sBuf.vConcat("("_Stz);
    Expr::vDescribe(pPredicand, sTempBuf, pRootNode);
    sBuf.vConcat(sTempBuf.szStz());
    sBuf.vConcat(")"_Stz);
  }

  sBuf.vConcat(" BETWEEN "_Stz);

  sTempBuf.vReset();
  if (pLowerBound->nChildren() == 0 || SubqueryRefExpr::bIsOfType(pLowerBound)) {
    Expr::vDescribe(pLowerBound, sTempBuf, pRootNode);
    sBuf.vConcat(sTempBuf.szStz());

  } else {
    sBuf.vConcat("("_Stz);
    Expr::vDescribe(pLowerBound, sTempBuf, pRootNode);
    sBuf.vConcat(sTempBuf.szStz());
    sBuf.vConcat(")"_Stz);
  }

  sBuf.vConcat(" AND "_Stz);

  sTempBuf.vReset();
  if (pUpperBound->nChildren() == 0 || SubqueryRefExpr::bIsOfType(pUpperBound)) {
    Expr::vDescribe(pUpperBound, sTempBuf, pRootNode);
    sBuf.vConcat(sTempBuf.szStz());

  } else {
    sBuf.vConcat("("_Stz);
    Expr::vDescribe(pUpperBound, sTempBuf, pRootNode);
    sBuf.vConcat(sTempBuf.szStz());
    sBuf.vConcat(")"_Stz);
  }
}

/*----------------------------------------------------------------------------*/
/* Return the expression being compared                                       */
/*----------------------------------------------------------------------------*/
QueryNode* BetweenExpr::pPredicand(QueryNode *pNode)
{
  _static_
  ASSERT(bIsOfType(pNode));
  ASSERT(pNode->nChildren() >= 2);
  ASSERT(pNode->nChildren() <= 3);

  QueryNode* pPredicand = _pQueryNode(pNode->pFirstChild());
  ASSERT(pPredicand);
  ASSERT(Expr::bIsOfType(pPredicand));

  return pPredicand;
}

/*----------------------------------------------------------------------------*/
/* The lower bound being compared to                                          */
/*----------------------------------------------------------------------------*/
QueryNode* BetweenExpr::pLowerBound(QueryNode *pNode)
{
  _static_
  ASSERT(bIsOfType(pNode));
  ASSERT(pNode->nChildren() == 3);

  QueryNode* pLowerBoundNode = _pQueryNode(pNode->pSecondChild());
  ASSERT(pLowerBoundNode);
  ASSERT(Expr::bIsOfType(pLowerBoundNode));

  return pLowerBoundNode;
}

/*----------------------------------------------------------------------------*/
/* The upper bound being compared to                                          */
/*----------------------------------------------------------------------------*/
QueryNode* BetweenExpr::pUpperBound(QueryNode *pNode)
{
  _static_
  ASSERT(bIsOfType(pNode));
  ASSERT(pNode->nChildren() == 3);

  QueryNode* pUpperBoundNode = _pQueryNode(pNode->pNthChild(2)); // 0-based index
  ASSERT(pUpperBoundNode);
  ASSERT(Expr::bIsOfType(pUpperBoundNode));

  return pUpperBoundNode;
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the expr definition         */
/*----------------------------------------------------------------------------*/
b1 BetweenExpr::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 BetweenExpr::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns the resulting type of the expression operation.                    */
/* A returned value of eCT::CT_NONE means that it is not supported or invalid.     */
/*----------------------------------------------------------------------------*/
i8 BetweenExpr::iType(QueryNode* pExprNode, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pExprNode));

  QueryNode* pPredicandNode = BetweenExpr::pPredicand(pExprNode);
  QueryNode* pLowerBound    = BetweenExpr::pLowerBound(pExprNode);
  QueryNode* pUpperBound    = BetweenExpr::pUpperBound(pExprNode);

  eCT ePredicand = TypeSystem::eCTVal(Expr::iType(pPredicandNode, pRootNode));
  eCT eLower     = TypeSystem::eCTVal(Expr::iType(pLowerBound, pRootNode));
  eCT eUpper     = TypeSystem::eCTVal(Expr::iType(pUpperBound, pRootNode));

  if (eLower != eUpper &&
      eLower != eCT::CT_UNKNOWN &&
      eUpper != eCT::CT_UNKNOWN &&
      ePredicand != eCT::CT_UNKNOWN) {
    // coercion is required
    return eCT::CT_NONE;
  }

  // Lookup the type of the result between the predicand and the lower bound
  eCT eResult = aeTypeCalculus[ePredicand][eLower];
  return TypeSystem::iIdOf(eResult);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
