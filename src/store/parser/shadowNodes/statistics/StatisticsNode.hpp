/*============================================================================*/
/* ShadowStatistics.hpp - a shadow node of the main /__root__/statistics node */
/*============================================================================*/
#pragma once

#include "ShadowNode.hpp"

namespace Parser
{
  class QueryNode;

  class StatisticsNode : public ShadowNode
  {

  private:
    // Shalt never be instantiated. Static functions only.
    StatisticsNode();

  public:
    static QueryNode* pCreate();

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1 bIsOfType(PlanNode* pNode);
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
