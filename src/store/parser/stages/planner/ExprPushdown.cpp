/*============================================================================*/
/* ExprPushdown.hpp - Pushes down expressions into lower ProjectOps as needed */
/*============================================================================*/

#include "BASE.h"
#include "ExprPushdown.hpp"
#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "RootNode.hpp"
#include "GraphNav.hpp"
#include "ShadowRoot.hpp"
#include "ShadowPlan.hpp"
#include "StatementMarker.hpp"
#include "SqlException.hpp"
#include "ProjectOp.hpp"
#include "FilterOp.hpp"
#include "SetOp.hpp"
#include "OrderOp.hpp"
#include "OrderItem.hpp"
#include "Expr.hpp"
#include "ProjectedColumns.hpp"


using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*-- Private classes/structs -------------------------------------------------*/
/*----------------------------------------------------------------------------*/




/*----------------------------------------------------------------------------*/
/* Pushes down any intermediate/working expressions that are not being        */
/* output into a lower ProjectOp/AggregateOp.                                 */
/*----------------------------------------------------------------------------*/
void ExprPushdown::vPushdownExprs(PlanNode* pRootNode)
{
  ASSERT(pRootNode);
  ASSERT(ShadowRoot::bIsOfType(pRootNode));

  PlanNode* pPlanNode = ShadowRoot::pPlanNode(pRootNode);
  ASSERT(pPlanNode);
  ASSERT(ShadowPlan::bIsOfType(pPlanNode));

  // Walk each node and look for the StatementMarkers
  //GraphNav::bGlobalWalk(pPlanNode->pFirstChild(), false, true, eStmtMarkerVisitor, pRootNode);

  // For each StatementMarker section
  //   Walk each node.
  //   If it is not a FilterOp, OrderOp then keep walking.
  //   Find the ProjectOp/AggregateOp below it in this StatementMarker section, if any.
  //   If none, then keep walking.
  //   Otherwise extract the expression from the FilterOp/OrderOp.
  //   Convert the expression to ordinal form. Determine if it is fully ordinal.
  //   If not fully ordinal then either:
  //      1. determine the largest non-ordinal subexpression; or
  //      2. use the whole expression; and
  //   Add this to the projected columns list of the ProjectOp identified above.
  //
  // Notes:
  //   It might be better to project the larges non-ordinal subexpression
  //   rather than projecting the whole expression. Rationale being that
  //   there may be a base part of the expression that is already being computed
  //   and is expensive to do so.
  //   e.g. assume that we have the expression (AVG(col2) / col1 > 0). Lets say
  //   that AVG(col2) is already being projected. We can either:
  //     1. Project col1 and let (AVG(col2) / col1 > 0) be calculated in the
  //        SQL operator upstream; or
  //     2. Project the entire expression (AVG(col2) / col1 > 0) so that the
  //        upstream SQL operator just has to evaluate the boolean result.
  //   In this example the AVG(col2) is probably an expensive operation so we
  //   don't want to calculate and project this twice. So let's only pushdown
  //   the largest non-ordinal subset that is required.
}


/*----------------------------------------------------------------------------*/
/* Validates that the right projected columns will be delivered to each       */
/* SQL operator that has expressions that use or build on those projected     */
/* columns. i.e. each expression in a SQL operator can be rewritten in        */
/* an ordinal form relative to the input projected columns.                   */
/*----------------------------------------------------------------------------*/
void ExprPushdown::vValidateExprVisibility(PlanNode* pRootNode)
{
  // Walk every SQL operator in the graph.
  // If the SQL operator has expressions then validate that each expression
  // can be converted to an ordinal format and that it is fully ordinal.
  // If not, throw and error about an expression not being visible.

  // Copy ExprValidator::vValidateOrderExprAboveSetOp() and generalise it.
}






/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
