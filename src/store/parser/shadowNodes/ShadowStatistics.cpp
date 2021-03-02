/*============================================================================*/
/* ShadowStatistics.hpp - a shadow node of the main /__root__/statistics node     */
/*============================================================================*/

#include "BASE.h"
#include "ShadowStatistics.hpp"

#include "QueryNode.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_STATISTICS_NAME = "statistics"_Stz;
static Stz* C_STATISTICS_DEFN = "/__root__/definitions/object/statisticsNode"_Stz;



/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Appends a description of the ProjectedExpr into a buffer                   */
/*----------------------------------------------------------------------------*/
QueryNode* ShadowStatistics::pCreate()
{
  _static_

  QueryNode* pStatistics = new QueryNode(C_STATISTICS_NAME, const_cast<Stz*>(C_STATISTICS_DEFN));
  return pStatistics;
}


/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the statistics node defn    */
/*----------------------------------------------------------------------------*/
b1 ShadowStatistics::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, const_cast<Stz*>(C_STATISTICS_DEFN));
}



/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
