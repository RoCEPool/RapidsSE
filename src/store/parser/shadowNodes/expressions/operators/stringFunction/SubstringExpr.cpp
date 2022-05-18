/*============================================================================*/
/* SubstringExpr.hpp - a SUBSTRING expression operator                        */
/*============================================================================*/

#include "BASE.h"
#include "SubstringExpr.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "eCT.h"
#include "NodeNamer.hpp"
#include "SubqueryRefExpr.hpp"
#include "ShadowRoot.hpp"
#include "GraphEditor.hpp"
#include "TypeSystem.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_NAME_PREFIX = "SubstringExpr"_Stz;
static Stz* C_DEFN = "/__root__/definitions/object/expr/operator/stringFunction/substring"_Stz;

// What is the resulting type when the source expression is of a given type?
static eCT aeSrcExprTypeCalculus[eCT::CT_ENUMS] = {
/*  0 NONE       */  eCT::CT_NONE,
/*  1 BOOLEAN    */  eCT::CT_NONE,
/*  2 CHAR       */  eCT::CT_VARCHAR,
/*  3 DATE       */  eCT::CT_NONE,
/*  4 DECIMAL    */  eCT::CT_NONE,
/*  5 FLOAT      */  eCT::CT_NONE,
/*  6 INTEGER    */  eCT::CT_NONE,
/*  7 INTERVAL_DT*/  eCT::CT_NONE,
/*  8 INTERVAL_YM*/  eCT::CT_NONE,
/*  9 TIME       */  eCT::CT_NONE,
/* 10 TIMESTAMP  */  eCT::CT_NONE,
/* 11 VARCHAR    */  eCT::CT_VARCHAR,
/* 12 UNKNOWN    */  eCT::CT_UNKNOWN
};

// What is the resulting type when the FROM expression is of a given type?
static eCT aeFromExprTypeCalculus[eCT::CT_ENUMS] = {
/*  0 NONE       */  eCT::CT_NONE,
/*  1 BOOLEAN    */  eCT::CT_NONE,
/*  2 CHAR       */  eCT::CT_NONE,
/*  3 DATE       */  eCT::CT_NONE,
/*  4 DECIMAL    */  eCT::CT_NONE,
/*  5 FLOAT      */  eCT::CT_NONE,
/*  6 INTEGER    */  eCT::CT_INTEGER,
/*  7 INTERVAL_DT*/  eCT::CT_NONE,
/*  8 INTERVAL_YM*/  eCT::CT_NONE,
/*  9 TIME       */  eCT::CT_NONE,
/* 10 TIMESTAMP  */  eCT::CT_NONE,
/* 11 VARCHAR    */  eCT::CT_NONE,
/* 12 UNKNOWN    */  eCT::CT_UNKNOWN
};

// What is the resulting type when the LENGTH expression is of a given type?
static eCT aeLengthExprTypeCalculus[eCT::CT_ENUMS] = {
/*  0 NONE       */  eCT::CT_NONE,
/*  1 BOOLEAN    */  eCT::CT_NONE,
/*  2 CHAR       */  eCT::CT_NONE,
/*  3 DATE       */  eCT::CT_NONE,
/*  4 DECIMAL    */  eCT::CT_NONE,
/*  5 FLOAT      */  eCT::CT_NONE,
/*  6 INTEGER    */  eCT::CT_INTEGER,
/*  7 INTERVAL_DT*/  eCT::CT_NONE,
/*  8 INTERVAL_YM*/  eCT::CT_NONE,
/*  9 TIME       */  eCT::CT_NONE,
/* 10 TIMESTAMP  */  eCT::CT_NONE,
/* 11 VARCHAR    */  eCT::CT_NONE,
/* 12 UNKNOWN    */  eCT::CT_UNKNOWN
};


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

QueryNode* SubstringExpr::pCreate(QueryNode* pSrcExpr, QueryNode* pFromExpr, QueryNode* pLengthExpr)
{
  _static_
  ASSERT(pSrcExpr);
  ASSERT(Expr::bIsOfType(pSrcExpr));
  ASSERT(pFromExpr);
  ASSERT(Expr::bIsOfType(pFromExpr));
  ASSERT(!pLengthExpr || Expr::bIsOfType(pLengthExpr));

  // Create a unique name for this object
  s256 sName;
  NodeNamer::vGenerateName(C_DEFN, sName);

  QueryNode* pSubstringExpr = new QueryNode(sName, C_DEFN);
  pSubstringExpr->vAppendChild(pSrcExpr);
  pSubstringExpr->vAppendChild(pFromExpr);

  if (pLengthExpr) {
    pSubstringExpr->vAppendChild(pLengthExpr);
  }

  return pSubstringExpr;
}


/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void SubstringExpr::vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));

  QueryNode* pSrcExprNode    = SubstringExpr::pSrcExpr(_pQueryNode(pNode));
  QueryNode* pFromExprNode   = SubstringExpr::pFromExpr(_pQueryNode(pNode));
  QueryNode* pLengthExprNode = SubstringExpr::pLengthExpr(_pQueryNode(pNode));

  sBuf.vConcat("SUBSTRING("_Stz);

  if (pSrcExprNode->nChildren() == 0 || SubqueryRefExpr::bIsOfType(pSrcExprNode)) {
    Expr::vDescribe(pSrcExprNode, sBuf, pRootNode);

  } else {
    sBuf.vConcat("("_Stz);
    Expr::vDescribe(pSrcExprNode, sBuf, pRootNode);
    sBuf.vConcat(")"_Stz);
  }

  sBuf.vConcat(", "_Stz);

  if (pFromExprNode->nChildren() == 0 || SubqueryRefExpr::bIsOfType(pFromExprNode)) {
    Expr::vDescribe(pFromExprNode, sBuf, pRootNode);

  } else {
    sBuf.vConcat("("_Stz);
    Expr::vDescribe(pFromExprNode, sBuf, pRootNode);
    sBuf.vConcat(")"_Stz);
  }

  if (pLengthExprNode) {
    sBuf.vConcat(", "_Stz);

    if (pLengthExprNode->nChildren() == 0 || SubqueryRefExpr::bIsOfType(pLengthExprNode)) {
      Expr::vDescribe(pLengthExprNode, sBuf, pRootNode);

    } else {
      sBuf.vConcat("("_Stz);
      Expr::vDescribe(pLengthExprNode, sBuf, pRootNode);
      sBuf.vConcat(")"_Stz);
    }
  }

  sBuf.vConcat(")"_Stz);

}

/*----------------------------------------------------------------------------*/
/* Return the source string expression that is being substringed              */
/*----------------------------------------------------------------------------*/
QueryNode* SubstringExpr::pSrcExpr(QueryNode *pNode)
{
  _static_
  ASSERT(bIsOfType(pNode));
  ASSERT(pNode->nChildren() >= 2);
  ASSERT(pNode->nChildren() <= 3);

  QueryNode* pSrcExprNode = _pQueryNode(pNode->pFirstChild());
  ASSERT(Expr::bIsOfType(pSrcExprNode));
  
  return pSrcExprNode;
}

/*----------------------------------------------------------------------------*/
/* Return the offset to substring from                                        */
/*----------------------------------------------------------------------------*/
QueryNode* SubstringExpr::pFromExpr(QueryNode *pNode)
{
  _static_
  ASSERT(bIsOfType(pNode));
  ASSERT(pNode->nChildren() >= 2);
  ASSERT(pNode->nChildren() <= 3);

  QueryNode* pFromExprNode = _pQueryNode(pNode->pSecondChild());
  ASSERT(Expr::bIsOfType(pFromExprNode));

  return pFromExprNode;
}

/*----------------------------------------------------------------------------*/
/* Return the length to substring                                             */
/*----------------------------------------------------------------------------*/
QueryNode* SubstringExpr::pLengthExpr(QueryNode *pNode)
{
  _static_
  ASSERT(bIsOfType(pNode));
  ASSERT(pNode->nChildren() >= 2);
  ASSERT(pNode->nChildren() <= 3);
  
  QueryNode* pLengthExprNode = _pQueryNode(pNode->pNthChild(2)); // 0-based
  ASSERT(!pLengthExprNode || Expr::bIsOfType(pLengthExprNode));

  return pLengthExprNode;
}


/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the expr definition         */
/*----------------------------------------------------------------------------*/
b1 SubstringExpr::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 SubstringExpr::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns the resulting type of the expression operation.                    */
/* A returned value of eCT::CT_NONE means that it is not supported or invalid.     */
/*----------------------------------------------------------------------------*/
i8 SubstringExpr::iType(QueryNode* pExprNode, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pExprNode));

  // Get the types of the two operands
  QueryNode* pSrcExprNode    = SubstringExpr::pSrcExpr(pExprNode);
  QueryNode* pFromExprNode   = SubstringExpr::pFromExpr(pExprNode);
  QueryNode* pLengthExprNode = SubstringExpr::pLengthExpr(pExprNode);

  eCT eSrcType    = TypeSystem::eCTVal(Expr::iType(pSrcExprNode, pRootNode));
  eCT eFromType   = TypeSystem::eCTVal(Expr::iType(pFromExprNode, pRootNode));
  eCT eLengthType = (pLengthExprNode ? TypeSystem::eCTVal(Expr::iType(pLengthExprNode, pRootNode)) : eCT::CT_INTEGER);

  eCT eResultSrcType    = aeSrcExprTypeCalculus[eSrcType];
  eCT eResultFromType   = aeFromExprTypeCalculus[eFromType];
  eCT eResultLengthType = aeLengthExprTypeCalculus[eLengthType];
  
  if (eResultSrcType    == eCT::CT_NONE ||
      eResultFromType   == eCT::CT_NONE ||
      eResultLengthType == eCT::CT_NONE) {
    return TypeSystem::iIdOf(eCT::CT_NONE);
  }

  if (eResultSrcType    == eCT::CT_UNKNOWN ||
      eResultFromType   == eCT::CT_UNKNOWN ||
      eResultLengthType == eCT::CT_UNKNOWN) {
    return TypeSystem::iIdOf(eCT::CT_UNKNOWN);
  }

  return TypeSystem::iIdOf(eResultSrcType);
}

/*----------------------------------------------------------------------------*/
/* Modifies subexpressions by promoting/casting data types so that they       */
/* are type compatible with this expression. Modifies the expression          */
/* tree in place.                                                             */
/*----------------------------------------------------------------------------*/
void SubstringExpr::vCoerceSubExprs(QueryNode* pSubstringExpr, PlanNode* pRootNode)
{
  _static_
  ASSERT(pSubstringExpr);
  ASSERT(bIsOfType(pSubstringExpr));
  ASSERT(pRootNode);
  ASSERT(ShadowRoot::bIsOfType(pRootNode));

  // We need to check if any of the 3 children individually need to be coerced.

  // Substring Length
  QueryNode* pLength = pLengthExpr(pSubstringExpr);

  if (pLength && Expr::iType(pLength, pRootNode) != eCT::CT_INTEGER) {
    Expr::vCoerceSubExprs(pLength, pRootNode);
  }

  // Start Position
  QueryNode* pStartPos = pFromExpr(pSubstringExpr);
  ASSERT(pStartPos);

  if (Expr::iType(pStartPos, pRootNode) != eCT::CT_INTEGER) {
    Expr::vCoerceSubExprs(pStartPos, pRootNode);
  }

  // Source expression
  QueryNode* pStrSrc = pSrcExpr(pSubstringExpr);
  ASSERT(pStartPos);

  eCT eStrSrcType = TypeSystem::eCTVal(Expr::iType(pStrSrc, pRootNode));
  if (eStrSrcType != eCT::CT_CHAR &&
      eStrSrcType != eCT::CT_VARCHAR &&
      eStrSrcType != eCT::CT_UNKNOWN &&
      Expr::bIsImplicitlyCoercibleTo(eCT::CT_VARCHAR, pStrSrc, pRootNode)) {

    // We don't need to worry about pStrSrc being a compound expression where a
    // lower branch of it requires coercion because we walk the expression graphs
    // in nameValidate and exprValidate and this would have already been corrected
    // if needed. So we only need to ensure that the type of pStrSrc is a string.
    QueryNode* pNewStrSrc = Expr::pCoerceTo(eCT::CT_VARCHAR, pStrSrc, pRootNode);
    GraphEditor::vReplaceChild(pSubstringExpr, pStrSrc, pNewStrSrc);
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
