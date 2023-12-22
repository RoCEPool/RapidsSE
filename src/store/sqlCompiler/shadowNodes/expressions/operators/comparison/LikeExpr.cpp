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
#include "TypeSystem.hpp"
#include "TypeMap2D.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_NAME_PREFIX = "likeExpr"_Stz;
static Stz* C_DEFN = "/__root__/definitions/object/expr/operator/comparison/like"_Stz;


// What is the resulting type when we combine these inputs?
//  - The first parameter represents the type of the first operand
//  - The second parameter represents the type of the second operand
//  - The third parameter represents the type of the result from this operation.
//  - Any unspecified mappings are unsupported.
//  - Multiple mappings may be made in one go using the eTM values to represent a set of types.
static TypeMap2D* S_pTypeMap = (new TypeMap2D())
    ->pAddMapping(TypeId::xIdOf(CT_CHAR),   TM_ALL_STRINGS,                 TypeId::xIdOf(CT_BOOLEAN))
    ->pAddMapping(TypeId::xIdOf(CT_CHAR),     TypeId::xIdOf(CT_UNKNOWN), TypeId::xIdOf(CT_UNKNOWN))
    ->pAddMapping(TypeId::xIdOf(CT_VARCHAR),TM_ALL_STRINGS,                 TypeId::xIdOf(CT_BOOLEAN))
    ->pAddMapping(TypeId::xIdOf(CT_VARCHAR),  TypeId::xIdOf(CT_UNKNOWN), TypeId::xIdOf(CT_UNKNOWN))

    // All the unknown mappings here
    ->pAddMapping(TypeId::xIdOf(CT_UNKNOWN),TM_ALL_STRINGS,                TypeId::xIdOf(CT_UNKNOWN))
    ->pAddMapping(TypeId::xIdOf(CT_UNKNOWN),  TypeId::xIdOf(CT_UNKNOWN),TypeId::xIdOf(CT_UNKNOWN))
;

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
/* Represent the expression as types instead of names. Append to the buffer   */
/*----------------------------------------------------------------------------*/
void LikeExpr::vDescribeAsTypes(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));

  QueryNode* pPredicand = LikeExpr::pPredicand(_pQueryNode(pNode));
  QueryNode* pPattern   = LikeExpr::pPattern(_pQueryNode(pNode));
  QueryNode* pEscape    = LikeExpr::pEscape(_pQueryNode(pNode));

  sBuf.vConcat(Expr::xTypeId(pPredicand, pRootNode).szTypeName());
  sBuf.vConcat(" LIKE "_Stz);
  sBuf.vConcat(Expr::xTypeId(pPattern, pRootNode).szTypeName());

  if (pEscape) {
    sBuf.vConcat(" ESCAPE "_Stz);

    sBuf.vConcat(Expr::xTypeId(pEscape, pRootNode).szTypeName());
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
TypeId LikeExpr::xTypeId(QueryNode* pExprNode, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pExprNode));

  QueryNode* pPredicandNode = pPredicand(pExprNode);
  QueryNode* pPatternNode   = pPattern(pExprNode);
  QueryNode* pEscapeNode    = pEscape(pExprNode);

  if (pEscapeNode) {
    eCT eEscapeType = Expr::xTypeId(pEscapeNode, pRootNode).eCTVal();
  
    switch (eEscapeType) {
      case CT_UNKNOWN:
        return TypeId::xIdOf(CT_UNKNOWN);

      case CT_CHAR:
      case CT_VARCHAR:
        break;

      default:
        return TypeId::xIdOf(CT_NONE);
    }
  }

  TypeId xPredicandType = Expr::xTypeId(pPredicandNode, pRootNode);
  TypeId xPatternType   = Expr::xTypeId(pPatternNode, pRootNode);

  // Lookup the type of the result
  TypeId xResult = S_pTypeMap->xLookup(xPredicandType, xPatternType);
  return xResult;
}

/*----------------------------------------------------------------------------*/
/* Returns the precision of the resulting expression or -1 if not set or n/a  */
/*----------------------------------------------------------------------------*/
i8 LikeExpr::iPrecision(QueryNode* pExprNode, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pExprNode));
  ASSERT(pRootNode);
  UNUSED(pExprNode);
  UNUSED(pRootNode);

  // Precision returned is always n/a
  return -1;
}

/*----------------------------------------------------------------------------*/
/* Returns the scale of the resulting expression or -1 if not set or n/a      */
/*----------------------------------------------------------------------------*/
i8 LikeExpr::iScale(QueryNode* pExprNode, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pExprNode));
  ASSERT(pRootNode);
  UNUSED(pExprNode);
  UNUSED(pRootNode);

  // Scale returned is always n/a
  return -1;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
