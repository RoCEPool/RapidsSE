/*============================================================================*/
/* CaseExpr.hpp - a CASE expression is a container of WHEN and ELSE exprs     */
/*============================================================================*/

#include "BASE.h"
#include "CaseExpr.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "NodeNamer.hpp"
#include "SubqueryRefExpr.hpp"
#include "WhenExpr.hpp"
#include "ElseExpr.hpp"
#include "ConditionalQueryNodeIter.hpp"
#include "ShadowRoot.hpp"
#include "TypeSystem.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_NAME_PREFIX = "CaseExpr"_Stz;
static Stz* C_DEFN = "/__root__/definitions/object/expr/operator/conditional/case"_Stz;



/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Create the necessary plan graph nodes to represent this object             */
/*----------------------------------------------------------------------------*/
QueryNode* CaseExpr::pCreate(QueryNode* pElseExpr)
{
  _static_
  ASSERT(pElseExpr);
  ASSERT(ElseExpr::bIsOfType(pElseExpr));

  // Create a unique name for this object
  s256 sName;
  NodeNamer::vGenerateName(C_DEFN, sName);

  QueryNode* pExpr = new QueryNode(sName, C_DEFN);
  pExpr->vAppendChild(pElseExpr);

  return pExpr;
}


/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void CaseExpr::vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));


  sBuf.vConcat("CASE "_Stz);

  ConditionalQueryNodeIter* pWhenIter = CaseExpr::pWhenExprIter(_pQueryNode(pNode));
  while (pWhenIter->bHasNext()) {
    QueryNode* pWhenExpr = pWhenIter->pNext();
    ASSERT(WhenExpr::bIsOfType(pWhenExpr));

    WhenExpr::vDescribe(pWhenExpr, sBuf, pRootNode);

    sBuf.vConcat(" "_Stz);
  }

  QueryNode* pElseExpr = CaseExpr::pElseExpr(_pQueryNode(pNode));
  ElseExpr::vDescribe(pElseExpr, sBuf, pRootNode);

  sBuf.vConcat(" END"_Stz);

  delete pWhenIter;
}

/*----------------------------------------------------------------------------*/
/* Add a WHEN clause to this start of this CASE expression                    */
/*----------------------------------------------------------------------------*/
void CaseExpr::vPushWhenExpr(QueryNode* pCaseExpr, QueryNode *pWhenExpr)
{
  _static_
  ASSERT(bIsOfType(pCaseExpr));
  ASSERT(pCaseExpr->nChildren() >= 1);

  pCaseExpr->vPushChild(pWhenExpr);
}

/*----------------------------------------------------------------------------*/
/* Add a WHEN clause to this end of this CASE expression                      */
/*----------------------------------------------------------------------------*/
void CaseExpr::vAppendWhenExpr(QueryNode* pCaseExpr, QueryNode *pWhenExpr)
{
  _static_
  ASSERT(bIsOfType(pCaseExpr));
  ASSERT(pCaseExpr->nChildren() >= 1);

  QueryNode* pElseExpr = pCaseExpr->pPopLastChild();
  ASSERT(ElseExpr::bIsOfType(pElseExpr));

  pCaseExpr->vAppendChild(pWhenExpr);
  pCaseExpr->vAppendChild(pElseExpr);
}

/*----------------------------------------------------------------------------*/
/* An iterator of WHEN expressions. Must be deleted after use.                */
/*----------------------------------------------------------------------------*/
ConditionalQueryNodeIter* CaseExpr::pWhenExprIter(QueryNode *pNode)
{
  _static_
  ASSERT(bIsOfType(pNode));

  static auto fCond = [](PlanNode* pNode) -> b1 { return (pNode && WhenExpr::bIsOfType(pNode)); };

  PlanNodeIter* pChildrenIter = pNode->pNewChildrenIter();
  ConditionalQueryNodeIter* pWhenIter = new ConditionalQueryNodeIter(pChildrenIter, fCond);

  return pWhenIter;
}

/*----------------------------------------------------------------------------*/
/* Retrieve the result expr to be used if the test expr evaluates to true     */
/*----------------------------------------------------------------------------*/
QueryNode* CaseExpr::pElseExpr(QueryNode *pNode)
{
  _static_
  ASSERT(bIsOfType(pNode));
  ASSERT(pNode->nChildren() > 0);

  QueryNode* pLastChild = _pQueryNode(pNode->pLastChild());
  ASSERT(ElseExpr::bIsOfType(pLastChild));

  return pLastChild;
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the expr definition         */
/*----------------------------------------------------------------------------*/
b1 CaseExpr::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 CaseExpr::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns the resulting type of the expression operation.                    */
/* A returned value of CT_NONE means that it is not supported or invalid.     */
/*----------------------------------------------------------------------------*/
i8 CaseExpr::iType(QueryNode* pNode, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));

  // Inspect all of the types of the WHEN and ELSE statements.
  // Ensure they are consistent.
  // Look for the first non-null type otherwise return CT_UNKNOWN.

  eCT eResultType = CT_UNKNOWN;

  PlanNodeIter* pIter = pNode->pNewChildrenIter();
  while (pIter->bHasNext()) {
    QueryNode* pChild = _pQueryNode(pIter->pNext());
    ASSERT(WhenExpr::bIsOfType(pChild) || ElseExpr::bIsOfType(pChild));

    eCT eChildType = TypeSystem::eCTVal(Expr::iType(pChild, pRootNode));

    if (eChildType == CT_NONE) {
      // error
      delete pIter;
      return TypeSystem::iIdOf(eChildType);
    }

    if (eResultType == eChildType) {
      continue;
    }

    // So eResultType != eChildType

    if (eResultType == CT_UNKNOWN) {
      // Accept the return type of this child
      ASSERT(eChildType != CT_UNKNOWN);
      eResultType = eChildType;
      continue;

    } else if (eChildType == CT_UNKNOWN) {
      // No change to the overall return type
      continue;
    }

    // eResultType != eChildType,
    // eResultType != CT_UNKNOWN
    // eChildType  != CT_UNKNOWN
    // eChildType  != CT_NONE
    // eResultType != CT_NONE

    // Since eResultType != eChildType either an incorrect CASE statement has
    // been specified, or we will need to coerce some values. So return
    // CT_NONE to force this to happen.
    delete pIter;
    return TypeSystem::iIdOf(CT_NONE);
  }

  delete pIter;
  return TypeSystem::iIdOf(eResultType);
}

/*----------------------------------------------------------------------------*/
/* Modifies subexpressions by promoting/casting data types so that they       */
/* are type compatible with this expression. Modifies the expression          */
/* tree in place.                                                             */
/*----------------------------------------------------------------------------*/
void CaseExpr::vCoerceSubExprs(QueryNode* pCaseExpr, PlanNode* pRootNode)
{
  _static_
  ASSERT(pCaseExpr);
  ASSERT(bIsOfType(pCaseExpr));
  ASSERT(pRootNode);
  ASSERT(ShadowRoot::bIsOfType(pRootNode));

  // We don't need to check if the predicate needs coercing
  // because we will walk the expression graph bottom-up in
  // the ExprValidate stage and handle it there. Similarly,
  // each WHEN result expr should also be non-CT_NONE.
  // Where we may have problems is where the different WHEN
  // expressions return different result types.
  // We need to try to find a common coercible data type

  // An array with an occurrence count for each coercible data type
  i8 aiCoercibleTargets[CT_ENUMS] = { 0 };

  PlanNodeIter* pWhenOrElseIter = pCaseExpr->pNewChildrenIter();

  while (pWhenOrElseIter->bHasNext()) {
    QueryNode* pWhenOrElseExpr = _pQueryNode(pWhenOrElseIter->pNext());
    ASSERT(WhenExpr::bIsOfType(pWhenOrElseExpr) || ElseExpr::bIsOfType(pWhenOrElseExpr));

    QueryNode* pValueExpr;

    if (WhenExpr::bIsOfType(pWhenOrElseExpr)) {
      pValueExpr = WhenExpr::pResultExpr(pWhenOrElseExpr);

    } else if (ElseExpr::bIsOfType(pWhenOrElseExpr)) {
      pValueExpr = ElseExpr::pElseExpr(pWhenOrElseExpr);

    } else {
      NEVER_REACH
    }

    ASSERT(Expr::bIsOfType(pValueExpr));
    ASSERT(Expr::iType(pValueExpr, pRootNode) != CT_NONE);

    // Increment the aiCoercibleTargets array if this child can be coerced
    // to each of the data types.
    vIncrementCoercibleTargets(pValueExpr, aiCoercibleTargets, pRootNode);
  }

  // Determine the common coercible data type, if any
  eCT eCoercibleTarget = eCommonCoercibleTarget(aiCoercibleTargets, pCaseExpr->nChildren());

  if (eCoercibleTarget == CT_NONE) {
    delete pWhenOrElseIter;
    return;
  }

  // Now coerce the result expr of each WHEN/ELSE clause to this type
  pWhenOrElseIter->vBeforeFirst();
  while (pWhenOrElseIter->bHasNext()) {
    QueryNode* pWhenOrElseExpr = _pQueryNode(pWhenOrElseIter->pNext());

    if (Expr::iType(pWhenOrElseExpr, pRootNode) == eCoercibleTarget) {
      continue;
    }

    if (WhenExpr::bIsOfType(pWhenOrElseExpr)) {
      WhenExpr::vCoerceResultExprTo(eCoercibleTarget, pWhenOrElseExpr, pRootNode);

    } else if (ElseExpr::bIsOfType(pWhenOrElseExpr)) {
      ElseExpr::vCoerceResultExprTo(eCoercibleTarget, pWhenOrElseExpr, pRootNode);

    } else {
      NEVER_REACH
    }
  }


  delete pWhenOrElseIter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
