/*============================================================================*/
/* SetOpValidator.hpp - Simplifies and validates plan segments that contain   */
/* set operations (e.g. UNION, INSERSECT, etc)                                */
/*============================================================================*/

#include "BASE.h"
#include "SetOpValidator.hpp"
#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "GraphNav.hpp"
#include "ShadowRoot.hpp"
#include "SqlException.hpp"

#include "Expr.hpp"
#include "StatementMarker.hpp"


using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Returns true if there are any set operations in this plan.                 */
/* If not then we can ignore these other functions.                           */
/*----------------------------------------------------------------------------*/
b1 SetOpValidator::bHasSetOps(PlanNode *pRootNode)
{
  return false;
}

/*----------------------------------------------------------------------------*/
/* Tries to simplify the set operators, if needed, by handling                */
/* the CORRESPONDING BY list.                                                 */
/*----------------------------------------------------------------------------*/
void SetOpValidator::vSimplifySetOps(PlanNode *pRootNode)
{

}

/*----------------------------------------------------------------------------*/
/* Walk the graph and check the semantics of the expressions.                 */
/* e.g. Check that the operation supports the input data types.               */
/* A SqlException will be thrown if an invalid semantic is found.             */
/*----------------------------------------------------------------------------*/
void SetOpValidator::vValidateSetOps(PlanNode *pRootNode)
{

}

/*============================================================================*/

/*============================================================================*/



/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
