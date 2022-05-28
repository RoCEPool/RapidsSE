/*============================================================================*/
/* AliasRef.cpp - an expression representing a reference to a column         */
/*============================================================================*/

#include "BASE.h"
#include "AliasRef.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "PlanConstants.hpp"
#include "NodeNamer.hpp"

#include "ShadowRoot.hpp"
#include "MetadataNode.hpp"

#include "GraphNav.hpp"
#include "TypeSystem.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_ALIASREF_DEFN = "/__root__/definitions/object/expr/aliasRef"_Stz;
static Stz* C_ALIASREF_NAME_PREFIX = "ExprAliasRef"_Stz;

static Stz* C_ALIASNAME_PROP = "aliasName"_Stz;
static Stz* C_ALIASNAME_DEFN = "/__root__/definitions/object/valueObject/stringObject/nonNullableStringObject"_Stz;

static Stz* C_ALIASEXPR_PROP = "aliasExpr"_Stz;
static Stz* C_ALIASEXPR_DEFN = "/__root__/definitions/object/expr"_Stz;

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Helper function to create expression alias reference nodes.                */
/*----------------------------------------------------------------------------*/
QueryNode* AliasRef::pCreate(Text* pAliasName, QueryNode* pAliasExpr)
{
  _static_
  ASSERT(pAliasName);
  ASSERT(pAliasExpr);

  // Create a unique name for this object
  s256 sName;
  NodeNamer::vGenerateName(C_ALIASREF_DEFN, sName);

  // Create the aliasRef
  QueryNode* pAliasRef = new QueryNode(sName, C_ALIASREF_DEFN);

  // Create the name property
  // These Value instances will take ownership of the Text*s.
  Value xNameVal(pAliasName);
  QueryNode* pNameProp = new QueryNode(C_ALIASNAME_PROP, C_ALIASNAME_DEFN, static_cast<Value&&>(xNameVal));
  pAliasRef->vAppendProperty(pNameProp);

  // Append the aliased expression
  pAliasExpr->vSetName(C_ALIASEXPR_PROP);
  ASSERT(Expr::bIsOfType(pAliasExpr));
  pAliasRef->vAppendProperty(pAliasExpr);

  return pAliasRef;
}

/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void AliasRef::vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));
  UNUSED(pRootNode);

  QueryNode* pAliasRef = _pQueryNode(pNode);

  Stz* szAliasName = szAlias(pAliasRef);
  sBuf.vConcat(szAliasName);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the expr definition         */
/*----------------------------------------------------------------------------*/
b1 AliasRef::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_ALIASREF_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 AliasRef::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_ALIASREF_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns the resulting type of the expression operation                     */
/*----------------------------------------------------------------------------*/
TypeId AliasRef::xTypeId(QueryNode* pNode, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));
  ASSERT(pRootNode);
  ASSERT(ShadowRoot::bIsOfType(pRootNode));

  // Get the aliasExpr node
  QueryNode* pAliasExpr = _pQueryNode(GraphNav::pFindProperty(C_ALIASEXPR_PROP, pNode));
  ASSERT(pAliasExpr);

  TypeId xTypeId = Expr::xTypeId(pAliasExpr, pRootNode);
  return xTypeId;
}

/*----------------------------------------------------------------------------*/
/* Returns the alias name of the AliasRef                                     */
/*----------------------------------------------------------------------------*/
Stz* AliasRef::szAlias(QueryNode* pNode)
{
  ASSERT(pNode);
  ASSERT(bIsOfType(pNode));

  QueryNode* pNameProp = _pQueryNode(GraphNav::pFindProperty(C_ALIASNAME_PROP, pNode));
  ASSERT(pNameProp);

  Stz* szAliasName  = pNameProp->szValue();
  ASSERT(szAliasName);

  return szAliasName;
}

/*----------------------------------------------------------------------------*/
/* Return the expression that is being aliased                                */
/*----------------------------------------------------------------------------*/
QueryNode* AliasRef::pAliasedExpr(QueryNode* pNode)
{
  ASSERT(pNode);
  ASSERT(bIsOfType(pNode));

  QueryNode* pExprProp = _pQueryNode(GraphNav::pFindProperty(C_ALIASEXPR_PROP, pNode));
  ASSERT(pExprProp);

  return pExprProp;
}

/*----------------------------------------------------------------------------*/
/* Compare two expression trees and return true if they are equal with        */
/* respect to the operations and symbols used.                                */
/*----------------------------------------------------------------------------*/
b1 AliasRef::bIsSymbolicallyEqual(QueryNode* pAliasRef1, QueryNode* pAliasRef2)
{
  // This is called by Expr::bIsSymbolicallyEqual() if we are comparing two
  // AliasRef objects.
  // This is already validated
//  if (pExpr1 == pExpr2) return true;
//
//  // Is one of them not null and the other null?
//  if (pExpr1 && !pExpr2) return false;
//  if (!pExpr1 && pExpr2) return false;
//
//  // We don't compare names as they are allowed to be different
//  // (except in the case of properties, but see below).
//
//  // Types must match
//  if (!pExpr1->szType()->bEqual(pExpr2->szType())) return false;

  ASSERT(bIsOfType(pAliasRef1));
  ASSERT(bIsOfType(pAliasRef2));

  Stz* szAlias1 = szAlias(pAliasRef1);
  Stz* szAlias2 = szAlias(pAliasRef2);

  // Values must match. Note that this does a case insensitive comparison.
  if (!szAlias1->bSame(szAlias2)) return false;

  // We would expect the aliased expressions to also be equal
  QueryNode* pAliasedExpr1 = pAliasedExpr(pAliasRef1);
  QueryNode* pAliasedExpr2 = pAliasedExpr(pAliasRef2);
  ASSERT(Expr::bIsSymbolicallyEqual(pAliasedExpr1, pAliasedExpr2));
  UNUSED(pAliasedExpr1); // in release
  UNUSED(pAliasedExpr2); // in release

  return true;
}

/*----------------------------------------------------------------------------*/
/* Replace the current AliasRef node with its expanded expression, in place   */
/*----------------------------------------------------------------------------*/
void AliasRef::vExpandAlias(QueryNode* pNode)
{
  ASSERT(pNode);
  ASSERT(bIsOfType(pNode));

  // Remove the aliased expression
  QueryNode* pExpr = pNode->pRemoveProperty(C_ALIASEXPR_PROP);
  ASSERT(pExpr);

  // Replace pNode with pExpr
  pNode->vTransmogrifyTo(pExpr);

  // Delete what is left of pExpr
  delete pExpr;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
