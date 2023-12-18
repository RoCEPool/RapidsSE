/*============================================================================*/
/* ShadowStatistics.hpp - a shadow node of the main /__root__/statistics node     */
/*============================================================================*/

#include "BASE.h"
#include "StatisticsNode.hpp"

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
/* Create the statistics node                                                 */
/*----------------------------------------------------------------------------*/
QueryNode* StatisticsNode::pCreate()
{
  _static_

  QueryNode* pStatistics = new QueryNode(C_STATISTICS_NAME, const_cast<Stz*>(C_STATISTICS_DEFN));
  return pStatistics;
}


/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the statistics node defn    */
/*----------------------------------------------------------------------------*/
b1 StatisticsNode::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, const_cast<Stz*>(C_STATISTICS_DEFN));
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 StatisticsNode::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_STATISTICS_DEFN);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
