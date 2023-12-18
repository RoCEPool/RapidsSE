/*============================================================================*/
/* InnerJoin.hpp - represents a SQL inner join                                */
/*============================================================================*/
#pragma once

#include "JoinType.hpp"

namespace Parser
{
  class PlanNode;
  class QueryNode;

  class InnerJoin : public JoinType
  {

  private:
    // Shalt never be instantiated. Static functions only.
    InnerJoin();

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
