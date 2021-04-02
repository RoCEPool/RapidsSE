/*============================================================================*/
/* ExplainMarker.hpp - identifies the subgraph that follows is a SELECT stmt   */
/*============================================================================*/

#include "BASE.h"
#include "ExplainMarker.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "StatementMarker.hpp"

#include "GraphNav.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_DEFN = "/__root__/definitions/object/planObject/statementMarker/dml/explain"_Stz;

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Helper function to create statement nodes. These are the top level         */
/* "headings" in a plan graph that identify what comes below them.            */
/*----------------------------------------------------------------------------*/
QueryNode* ExplainMarker::pCreate(Stz* szQueryText)
{
  // Create it in the base class
  QueryNode* pMarker = StatementMarker::pCreate(ST_EXPLAIN, C_DEFN, szQueryText);
  return pMarker;
}


/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the statement definition    */
/*----------------------------------------------------------------------------*/
b1 ExplainMarker::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 ExplainMarker::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_DEFN);
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
