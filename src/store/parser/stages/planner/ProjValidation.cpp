/*============================================================================*/
/* ProjValidation.cpp - Validates that all expressions only use the projected */
/* columns from SQL operators below it.                                       */
/*============================================================================*/

#include "BASE.h"
#include "ProjValidation.hpp"
#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "GraphNav.hpp"
#include "ShadowRoot.hpp"
#include "StatementMarker.hpp"
#include "SqlException.hpp"
#include "ProjectOp.hpp"
#include "FilterOp.hpp"
#include "OrderOp.hpp"
#include "OrderItem.hpp"
#include "OrderItemList.hpp"
#include "Expr.hpp"
#include "ShadowPlan.hpp"
#include "ShadowRoot.hpp"


using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*-- Private classes/structs -------------------------------------------------*/
/*----------------------------------------------------------------------------*/




/*----------------------------------------------------------------------------*/
/* Validates that the right projected columns will be delivered to each       */
/* SQL operator that has expressions that use or build on those projected     */
/* columns. i.e. each expression in a SQL operator can be rewritten in        */
/* an ordinal form relative to the input projected columns.                   */
/*----------------------------------------------------------------------------*/
void ProjValidation::vValidateExprVisibility(PlanNode* pRootNode)
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
