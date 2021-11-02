/*============================================================================*/
/* LikeExpr.hpp - a LIKE expression operator                                  */
/*============================================================================*/

#include "BASE.h"
#include "LikeExpr.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "NodeNamer.hpp"
#include "SubqueryRefExpr.hpp"
#include "StringObject.hpp"
#include "GraphNav.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_NAME_PREFIX = "likeExpr"_Stz;
static Stz* C_DEFN = "/__root__/definitions/object/expr/operator/comparison/like"_Stz;

static Stz* C_ESCAPE_PROP = "escape"_Stz;

// What is the resulting type when we combine these inputs?
// * The left header column represents the type of the first operand (predicand)
// * The top header row represents the type of the second operand (pattern)
// * The corresponding cell in the matrix represents the type of the resultant from this operation.
// * Invalid/unsupported combinations are represented with CT_NONE.
// * Fields marked with /*?*/ are not currently supported but could be.  Refer to
//   the documentation below marked with this symbol for more information.
// * Fields marked with /*X*/ are not currently supported but should be.
static eCT aeTypeCalculus[CT_ENUMS][CT_ENUMS] = {

//                    NONE     BOOLEAN     CHAR        DATE          DECIMAL     FLOAT       INTEGER       INTERVAL_DT  INTERVAL_YM     TIME          TIMESTAMP     VARCHAR     UNKNOWN
/*  0 NONE       */ { CT_NONE, CT_NONE,    CT_NONE,    CT_NONE,      CT_NONE,    CT_NONE,    CT_NONE,      CT_NONE,     CT_NONE,        CT_NONE,      CT_NONE,      CT_NONE,    CT_NONE    },
/*  1 BOOLEAN    */ { CT_NONE, CT_NONE,    CT_NONE,    CT_NONE,      CT_NONE,    CT_NONE,    CT_NONE,      CT_NONE,     CT_NONE,        CT_NONE,      CT_NONE,      CT_NONE,    CT_NONE    },
/*  2 CHAR       */ { CT_NONE, CT_NONE,    CT_BOOLEAN, CT_NONE,      CT_NONE,    CT_NONE,    CT_NONE,      CT_NONE,     CT_NONE,        CT_NONE,      CT_NONE,      CT_BOOLEAN, CT_UNKNOWN },
/*  3 DATE       */ { CT_NONE, CT_NONE,    CT_NONE,    CT_NONE,      CT_NONE,    CT_NONE,    CT_NONE,      CT_NONE,     CT_NONE,        CT_NONE,      CT_NONE,      CT_NONE,    CT_NONE    },
/*  4 DECIMAL    */ { CT_NONE, CT_NONE,    CT_NONE,    CT_NONE,      CT_NONE,    CT_NONE,    CT_NONE,      CT_NONE,     CT_NONE,        CT_NONE,      CT_NONE,      CT_NONE,    CT_NONE    },
/*  5 FLOAT      */ { CT_NONE, CT_NONE,    CT_NONE,    CT_NONE,      CT_NONE,    CT_NONE,    CT_NONE,      CT_NONE,     CT_NONE,        CT_NONE,      CT_NONE,      CT_NONE,    CT_NONE    },
/*  6 INTEGER    */ { CT_NONE, CT_NONE,    CT_NONE,    CT_NONE,      CT_NONE,    CT_NONE,    CT_NONE,      CT_NONE,     CT_NONE,        CT_NONE,      CT_NONE,      CT_NONE,    CT_NONE    },
/*  7 INTERVAL_DT*/ { CT_NONE, CT_NONE,    CT_NONE,    CT_NONE,      CT_NONE,    CT_NONE,    CT_NONE,      CT_NONE,     CT_NONE,        CT_NONE,      CT_NONE,      CT_NONE,    CT_NONE    },
/*  8 INTERVAL_YM*/ { CT_NONE, CT_NONE,    CT_NONE,    CT_NONE,      CT_NONE,    CT_NONE,    CT_NONE,      CT_NONE,     CT_NONE,        CT_NONE,      CT_NONE,      CT_NONE,    CT_NONE    },
/*  9 TIME       */ { CT_NONE, CT_NONE,    CT_NONE,    CT_NONE,      CT_NONE,    CT_NONE,    CT_NONE,      CT_NONE,     CT_NONE,        CT_NONE,      CT_NONE,      CT_NONE,    CT_NONE    },
/* 10 TIMESTAMP  */ { CT_NONE, CT_NONE,    CT_NONE,    CT_NONE,      CT_NONE,    CT_NONE,    CT_NONE,      CT_NONE,     CT_NONE,        CT_NONE,      CT_NONE,      CT_NONE,    CT_NONE    },
/* 11 VARCHAR    */ { CT_NONE, CT_NONE,    CT_BOOLEAN, CT_NONE,      CT_NONE,    CT_NONE,    CT_NONE,      CT_NONE,     CT_NONE,        CT_NONE,      CT_NONE,      CT_BOOLEAN, CT_UNKNOWN },
/* 12 UNKNOWN    */ { CT_NONE, CT_NONE,    CT_UNKNOWN, CT_NONE,      CT_NONE,    CT_NONE,    CT_NONE,      CT_NONE,     CT_NONE,        CT_NONE,      CT_NONE,      CT_UNKNOWN, CT_UNKNOWN }
};

// References:


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

QueryNode* LikeExpr::pCreate(QueryNode* pPredicand, QueryNode* pPattern, QueryNode* pEscape)
{
  _static_
  ASSERT(pPredicand);
  ASSERT(Expr::bIsOfType(pPredicand));
  ASSERT(pPattern);
  ASSERT(Expr::bIsOfType(pPattern));
  ASSERT(!pEscape || Expr::bIsOfType(pEscape));

  // Create a unique name for this object
  s256 sName;
  NodeNamer::vGenerateName(C_DEFN, sName);

  QueryNode* pExpr = new QueryNode(sName, C_DEFN);
  pExpr->vAppendChild(pPredicand);
  pExpr->vAppendChild(pPattern);

  if (pEscape) {
    pExpr->vAppendChild(pEscape);
  }

  return pExpr;
}


/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void LikeExpr::vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));

  QueryNode* pPredicand = LikeExpr::pPredicand(_pQueryNode(pNode));
  QueryNode* pPattern   = LikeExpr::pPattern(_pQueryNode(pNode));
  QueryNode* pEscape    = LikeExpr::pEscape(_pQueryNode(pNode));

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

  sBuf.vConcat(" LIKE "_Stz);

  sTempBuf.vReset();
  if (pPattern->nChildren() == 0 || SubqueryRefExpr::bIsOfType(pPattern)) {
    Expr::vDescribe(pPattern, sTempBuf, pRootNode);
    sBuf.vConcat(sTempBuf.szStz());

  } else {
    sBuf.vConcat("("_Stz);
    Expr::vDescribe(pPattern, sTempBuf, pRootNode);
    sBuf.vConcat(sTempBuf.szStz());
    sBuf.vConcat(")"_Stz);
  }

  if (pEscape) {
    sBuf.vConcat(" ESCAPE "_Stz);

    sTempBuf.vReset();
    Expr::vDescribe(pEscape, sTempBuf, pRootNode);

    sBuf.vConcat(sTempBuf.szStz());
  }
}

/*----------------------------------------------------------------------------*/
/* Return the expression being compared                                       */
/*----------------------------------------------------------------------------*/
QueryNode* LikeExpr::pPredicand(QueryNode *pNode)
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
/* Return the pattern being used to compare the predicand to                  */
/*----------------------------------------------------------------------------*/
QueryNode* LikeExpr::pPattern(QueryNode *pNode)
{
  _static_
  ASSERT(bIsOfType(pNode));
  ASSERT(pNode->nChildren() >= 2);
  ASSERT(pNode->nChildren() <= 3);

  QueryNode* pPattern = _pQueryNode(pNode->pSecondChild());
  ASSERT(pPattern);
  ASSERT(Expr::bIsOfType(pPattern));

  return pPattern;
}

/*----------------------------------------------------------------------------*/
/* Return the node that contains the escape character                         */
/*----------------------------------------------------------------------------*/
QueryNode* LikeExpr::pEscape(QueryNode *pNode)
{
  _static_
  ASSERT(bIsOfType(pNode));
  ASSERT(pNode->nChildren() >= 2);
  ASSERT(pNode->nChildren() <= 3);

  if (pNode->nChildren() < 3) {
    return nullptr;
  }

  QueryNode* pEscapeNode = _pQueryNode(pNode->pNthChild(2)); // 0-based index
  ASSERT(pEscapeNode);
  ASSERT(Expr::bIsOfType(pEscapeNode));

  return pEscapeNode;
}


/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the expr definition         */
/*----------------------------------------------------------------------------*/
b1 LikeExpr::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 LikeExpr::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns the resulting type of the expression operation.                    */
/* A returned value of CT_NONE means that it is not supported or invalid.     */
/*----------------------------------------------------------------------------*/
eCT LikeExpr::eType(QueryNode* pExprNode, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pExprNode));

  QueryNode* pPredicandNode = pPredicand(pExprNode);
  QueryNode* pPatternNode   = pPattern(pExprNode);
  QueryNode* pEscapeNode    = pEscape(pExprNode);

  if (pEscapeNode) {
    eCT eEscapeType = Expr::eType(pEscapeNode, pRootNode);
  
    switch (eEscapeType) {
      case CT_UNKNOWN:
        return CT_UNKNOWN;

      case CT_CHAR:
      case CT_VARCHAR:
        break;

      default:
        return CT_NONE;
    }
  }

  eCT ePredicand = Expr::eType(pPredicandNode, pRootNode);
  eCT ePattern   = Expr::eType(pPatternNode, pRootNode);

  // Lookup the type of the result
  eCT eResult = aeTypeCalculus[ePredicand][ePattern];

  return eResult;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
