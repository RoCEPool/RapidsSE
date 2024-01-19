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
#include "TypeMap1D.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_NAME_PREFIX = "SubstringExpr"_Stz;
static Stz* C_DEFN = "/__root__/definitions/object/expr/operator/stringFunction/substring"_Stz;

// What is the resulting type when the source expression is of a given type?
static TypeMap1D* S_pSrcExprTypeCalculus = (new TypeMap1D())
    ->pAddMapping(TypeId::xIdOf(CT_CHAR),    TypeId::xIdOf(CT_VARCHAR))
    ->pAddMapping(TypeId::xIdOf(CT_VARCHAR), TypeId::xIdOf(CT_VARCHAR))
    ->pAddMapping(TypeId::xIdOf(CT_UNKNOWN), TypeId::xIdOf(CT_UNKNOWN))
;

// What is the resulting type when the FROM expression is of a given type?
static TypeMap1D* S_pFromExprTypeCalculus = (new TypeMap1D())
    ->pAddMapping(TypeId::xIdOf(CT_INTEGER), TypeId::xIdOf(CT_INTEGER))
    ->pAddMapping(TypeId::xIdOf(CT_UNKNOWN), TypeId::xIdOf(CT_UNKNOWN))
;

// What is the resulting type when the LENGTH expression is of a given type?
static TypeMap1D* S_pLengthExprTypeCalculus = (new TypeMap1D())
    ->pAddMapping(TypeId::xIdOf(CT_INTEGER), TypeId::xIdOf(CT_INTEGER))
    ->pAddMapping(TypeId::xIdOf(CT_UNKNOWN), TypeId::xIdOf(CT_UNKNOWN))
;


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
/* Represent the expression as types instead of names. Append to the buffer   */
/*----------------------------------------------------------------------------*/
void SubstringExpr::vDescribeAsTypes(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));

  QueryNode* pSrcExprNode    = SubstringExpr::pSrcExpr(_pQueryNode(pNode));
  QueryNode* pFromExprNode   = SubstringExpr::pFromExpr(_pQueryNode(pNode));
  QueryNode* pLengthExprNode = SubstringExpr::pLengthExpr(_pQueryNode(pNode));

  sBuf.vCatSprint("SUBSTRING(%s, %s"_Stz,
                  Expr::xTypeId(pSrcExprNode, pRootNode).szTypeName(),
                  Expr::xTypeId(pFromExprNode, pRootNode).szTypeName());


  if (pLengthExprNode) {
    sBuf.vCatSprint(", %s"_Stz, Expr::xTypeId(pLengthExprNode, pRootNode).szTypeName());
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
/* A returned value of CT_NONE means that it is not supported or invalid.     */
/*----------------------------------------------------------------------------*/
TypeId SubstringExpr::xTypeId(QueryNode* pExprNode, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pExprNode));

  // Get the types of the two operands
  QueryNode* pSrcExprNode    = SubstringExpr::pSrcExpr(pExprNode);
  QueryNode* pFromExprNode   = SubstringExpr::pFromExpr(pExprNode);
  QueryNode* pLengthExprNode = SubstringExpr::pLengthExpr(pExprNode);

  TypeId xSrcType    = Expr::xTypeId(pSrcExprNode, pRootNode);
  TypeId xFromType   = Expr::xTypeId(pFromExprNode, pRootNode);
  TypeId xLengthType = (pLengthExprNode ? Expr::xTypeId(pLengthExprNode, pRootNode) : TypeId::xIdOf(CT_INTEGER));

  TypeId xResultSrcType    = S_pSrcExprTypeCalculus->xLookup(xSrcType);
  TypeId xResultFromType   = S_pFromExprTypeCalculus->xLookup(xFromType);
  TypeId xResultLengthType = S_pLengthExprTypeCalculus->xLookup(xLengthType);
  
  if ( !xResultSrcType.bIsValid()  ||
       !xResultFromType.bIsValid() ||
       !xResultLengthType.bIsValid() ) {
    return TypeId::xIdOf(CT_NONE);
  }

  TypeId xUnknown = TypeId::xIdOf(CT_UNKNOWN);

  if (xResultSrcType    == xUnknown ||
      xResultFromType   == xUnknown ||
      xResultLengthType == xUnknown) {
    return xUnknown;
  }

  return xResultSrcType;
}

/*----------------------------------------------------------------------------*/
/* Returns the precision of the resulting expression or -1 if not set or n/a  */
/*----------------------------------------------------------------------------*/
i8 SubstringExpr::iPrecision(QueryNode* pExprNode, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pExprNode));
  ASSERT(pRootNode);

  // The precision of a substring is the precision of the underlying source string.
  // (Not entirely sure if this is correct).
  QueryNode* pSrcExprNode = SubstringExpr::pSrcExpr(pExprNode);

  i8 iVal = Expr::iPrecision(pSrcExprNode, pRootNode);
  return iVal;
}

/*----------------------------------------------------------------------------*/
/* Returns the scale of the resulting expression or -1 if not set or n/a      */
/*----------------------------------------------------------------------------*/
i8 SubstringExpr::iScale(QueryNode* pExprNode, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pExprNode));
  UNUSED(pExprNode);
  ASSERT(pRootNode);
  UNUSED(pRootNode);

  return -1;
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

  if (pLength && Expr::xTypeId(pLength, pRootNode) != TypeId::xIdOf(CT_INTEGER)) {
    Expr::vCoerceSubExprs(pLength, pRootNode);
  }

  // Start Position
  QueryNode* pStartPos = pFromExpr(pSubstringExpr);
  ASSERT(pStartPos);

  if (Expr::xTypeId(pStartPos, pRootNode) != TypeId::xIdOf(CT_INTEGER)) {
    Expr::vCoerceSubExprs(pStartPos, pRootNode);
  }

  // Source expression
  QueryNode* pStrSrc = pSrcExpr(pSubstringExpr);
  ASSERT(pStartPos);

  eCT eStrSrcType = Expr::xTypeId(pStrSrc, pRootNode).eCTVal();
  if (eStrSrcType != CT_CHAR &&
      eStrSrcType != CT_VARCHAR &&
      eStrSrcType != CT_UNKNOWN &&
      Expr::bIsImplicitlyCoercibleTo(TypeId::xIdOf(CT_VARCHAR), pStrSrc, pSubstringExpr, pRootNode)) {

    // We don't need to worry about pStrSrc being a compound expression where a
    // lower branch of it requires coercion because we walk the expression graphs
    // in nameValidate and exprValidate and this would have already been corrected
    // if needed. So we only need to ensure that the type of pStrSrc is a string.
    QueryNode* pNewStrSrc = Expr::pCoerceTo(TypeId::xIdOf(CT_VARCHAR), pStrSrc, pRootNode);
    GraphEditor::vReplaceChild(pSubstringExpr, pStrSrc, pNewStrSrc);
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
