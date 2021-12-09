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
eCT CaseExpr::eType(QueryNode* pNode, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));

  // Inspect all of the types of the WHEN and ELSE statements.
  // Ensure they are consistent.
  // Look for the first non-null type otherwise return null.
  //
  // Also consider section 9.3 of the SQL spec (Result of data type combinations).
  // Generally, if we encounter a non-null data type then we expect all WHEN results
  // to also return the same data type (or null).
  // However if all WHEN results are numerics:
  //  - if all of them are exact numerics then the result is an exact numeric when
  //    an implementation defined precision and a scale equal to the maximum of the scales.
  //  - if any of them are an approximate numeric then the result is an approximate numeric.

  eCT eResultType = CT_UNKNOWN;

  PlanNodeIter* pIter = pNode->pNewChildrenIter();
  while (pIter->bHasNext()) {
    QueryNode* pChild = _pQueryNode(pIter->pNext());
    eCT eChildType = Expr::eType(pChild, pRootNode);

    if (eChildType == CT_NONE) {
      // error
      delete pIter;
      return eChildType;
    }

    if (eResultType == eChildType) {
      continue;
    }

    // eResultType != eChildType

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

    switch (eChildType) {  // The (potentially) new type to be adopted
      // First the special cases

      case CT_INTEGER:
        switch (eResultType) {
          case CT_DECIMAL:  // fall through
          case CT_FLOAT:    // fall through
            // No change. Approximate numerics trump exact numerics and
            // exact numerics with non-zero scale trump integers (which have 0 scale).
            break;
          case CT_INTEGER:  // Case previously handled
          case CT_UNKNOWN:  // Case previously handled
            NEVER_REACH
          default:
            // error, we have a mismatch of types in this case statement
            delete pIter;
            return CT_NONE;
        }
        break;

      case CT_DECIMAL:
        switch (eResultType) {
          case CT_INTEGER:
            // Promote the integer to a decimal
            eResultType = CT_DECIMAL;
            break;
          case CT_FLOAT:
            // No change. Approximate numerics trump exact numerics
            break;
          case CT_DECIMAL:  // Case previously handled
          case CT_UNKNOWN:  // Case previously handled
            NEVER_REACH
          default:
            // error, we have a mismatch of types in this case statement
            delete pIter;
            return CT_NONE;
        }
        break;

      case CT_FLOAT:
        switch (eResultType) {
          case CT_INTEGER:  // fall through
          case CT_DECIMAL:  // fall through
            // We promote the integer or decimal to a float
            eResultType = CT_FLOAT;
            break;
          case CT_FLOAT:
          case CT_UNKNOWN:
            NEVER_REACH
          default:
            // error, we have a mismatch of types in this case statement
            delete pIter;
            return CT_NONE;
        }
        break;

      case CT_DATE:
        if (eResultType == CT_TIMESTAMP) {
          // No change. Timestamps trump dates

        } else {
          // error, we have a mismatch of types in this case statement
          delete pIter;
          return CT_NONE;
        }
        break;

      case CT_TIMESTAMP:
        if (eResultType == CT_DATE) {
          // Promote the date to a timestamp
          eResultType = CT_TIMESTAMP;

        } else {
          // error, we have a mismatch of types in this case statement
          delete pIter;
          return CT_NONE;
        }
        break;

      // TODO: Add in support for INTERVALS


      default:
        // eResultType != eChildType and neither of them are CT_UNKNOWN
        ASSERT(eResultType != eChildType);
        // error, we have a mismatch of types in this case statement
        delete pIter;
        return CT_NONE;
    }
  }

  delete pIter;
  return eResultType;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
