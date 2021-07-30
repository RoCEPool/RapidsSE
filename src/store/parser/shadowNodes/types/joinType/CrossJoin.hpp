/*============================================================================*/
/* CrossJoin.hpp - represents a SQL cross join                                */
/*============================================================================*/
#pragma once

#include "JoinType.hpp"

namespace Parser
{
  class PlanNode;
  class QueryNode;

  class CrossJoin : public JoinType
  {

  private:
    // Shalt never be instantiated. Static functions only.
    CrossJoin();

  public:
    static PlanNode* pCreate();

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1 bIsOfType(PlanNode* pNode);

    // Returns true if a given node is this exact given type.
    static b1 bIsExactType(PlanNode* pNode);
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
