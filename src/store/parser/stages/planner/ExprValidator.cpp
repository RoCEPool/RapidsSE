/*============================================================================*/
/* ExprValidator.hpp - Validates that expressions are being specified and     */
/* used correctly                                                             */
/*============================================================================*/

#include "BASE.h"
#include "ExprValidator.hpp"
#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "PlanConstants.hpp"
#include "GraphNav.hpp"
#include "MetadataNode.hpp"
#include "ShadowRoot.hpp"
#include "SqlException.hpp"

#include "SeApi.hpp"
#include "InfoSet.hpp"
#include "TableMetadata.hpp"

#include "Expr.hpp"
#include "FilterOp.hpp"


using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/



/*----------------------------------------------------------------------------*/
/* Walk the graph and check the semantics of the expressions.                 */
/* e.g. Check that the operation supports the input data types.               */
/* A SqlException will be thrown if an invalid semantic is found.             */
/*----------------------------------------------------------------------------*/
void ExprValidator::vValidateExprs(PlanNode *pRootNode)
{
  // Find the /plan node
  QueryNode* pPlanNode = ShadowRoot::pPlanNode(pRootNode);
  ASSERT(pPlanNode);

  // Validate that the result types of any expressions are not invalid.
  vValidateExprResultType(pRootNode, pPlanNode);

  // Validate that all predicate expressions have a boolean result type
  vValidatePredicatesAreBoolean(pRootNode, pPlanNode);
}

/*----------------------------------------------------------------------------*/
/* Validate that the result types of any expressions are not invalid.         */
/* An invalid combination of exprs is one that returns CT_NONE for its type.  */
/*----------------------------------------------------------------------------*/
void ExprValidator::vValidateExprResultType(PlanNode *pRootNode, QueryNode* pPlanNode)
{
  ASSERT(pRootNode);
  ASSERT(pPlanNode);

  // Walk the graph looking for all expressions
  auto fVisitor = [](PlanNode *pNode, s256 &sPath, i8 iDepth, void *pvArg) -> GraphNav::eWalkCtrl {

    PlanNode* pRootNode = static_cast<PlanNode*>(pvArg);

    if (!Expr::bIsOfType(pNode)) {
      return GraphNav::WALK_CONTINUE;   // Keep walking
    }

    QueryNode* pExprNode = static_cast<QueryNode*>(pNode);

    // This will throw an exception if an error is found.
    Expr::vValidateResultType(pExprNode, pRootNode);

    // No need to descend any further into the expression
    return GraphNav::WALK_SKIP_SUBTREE;
  };

  // Walk the /plan node, it's properties and its children
  GraphNav::bWalk(pPlanNode, true, true, fVisitor, pRootNode);
}


/*----------------------------------------------------------------------------*/
/* Validate that all predicate expressions (e.g. in filterOps) return a       */
/* boolean expression.                                                        */
/*----------------------------------------------------------------------------*/
void ExprValidator::vValidatePredicatesAreBoolean(PlanNode *pRootNode, QueryNode* pPlanNode)
{
  ASSERT(pRootNode);
  ASSERT(pPlanNode);

  // Walk the graph looking for all filterOps
  auto fVisitor = [](PlanNode *pNode, s256 &sPath, i8 iDepth, void *pvArg) -> GraphNav::eWalkCtrl {

    PlanNode* pRootNode = static_cast<PlanNode*>(pvArg);

    if (!FilterOp::bIsOfType(pNode)) {
      return GraphNav::WALK_CONTINUE;   // Keep walking
    }

    QueryNode* pFilterNode = static_cast<QueryNode*>(pNode);
    QueryNode* pExprNode = FilterOp::pFilterExpr(pFilterNode);

    eCT eType = Expr::eType(pExprNode, pRootNode);

    if (eType == CT_BOOLEAN) {
      // All good, keep looking for more filterOps
      return GraphNav::WALK_CONTINUE;
    }

    // Throw an error
    s2K sBuf;
    Expr::vDescribe(pExprNode, sBuf);

    SemanticException::vThrow(EC_EXPR_MUST_BE_BOOLEAN,
                              " : %s"_Stz,
                              sBuf.szStz());

    return GraphNav::WALK_ABORT; // Only required to silence a compiler warning
  };

  // Walk the /plan node and its children. We're not interested in walking
  // the properties because we are looking for filterOps
  GraphNav::bWalk(pPlanNode, false, true, fVisitor, pRootNode);
}



/*============================================================================*/

/*============================================================================*/



/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
