/*============================================================================*/
/* NameValidator.hpp - Validates the names of objects used in the plan.       */
/*                     e.g. tables, columns, indexes, etc                     */
/*============================================================================*/

#include "BASE.h"
#include "NameValidator.hpp"
#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "DefinitionNode.hpp"
#include "PrimitiveNode.hpp"
#include "PlanConstants.hpp"
#include "GraphNav.hpp"

#include "ListExo_t.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/



/*----------------------------------------------------------------------------*/
/* Walk the graph and look up all names encountered in the system metadata.   */
/* This function will fill in any unresolved name parts as well as populate   */
/* the plan with metadata.                                                    */
/* A SqlException will be thrown if a name cannot be resolved.                */
/*----------------------------------------------------------------------------*/
void NameValidator::vValidateNames(PlanNode *pRootNode)
{

}

/*============================================================================*/

/*============================================================================*/



/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
