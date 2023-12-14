/*============================================================================*/
/* ShadowPlan.hpp - a shadow node of the main /__root__/plan node             */
/*============================================================================*/
#pragma once

#include "ShadowNode.hpp"

namespace Parser
{
  class QueryNode;

  class ShadowPlan : public ShadowNode
  {

  private:
    // Shalt never be instantiated. Static functions only.
    ShadowPlan();

  public:
    static QueryNode* pCreate();

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1 bIsOfType(PlanNode* pNode);

    // Returns true if a given node is this exact given type.
    static b1 bIsExactType(PlanNode* pNode);

    // Returns the one and only main subquery child node
    static QueryNode* pMainSubquery(QueryNode* pNode);
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
