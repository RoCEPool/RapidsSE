/*============================================================================*/
/* NaturalFullOuterJoin.hpp - represents a SQL natural full outer join        */
/*============================================================================*/
#pragma once

#include "JoinType.hpp"

namespace Parser
{
  class PlanNode;
  class QueryNode;

  class NaturalFullOuterJoin : public JoinType
  {

  private:
    // Shalt never be instantiated. Static functions only.
    NaturalFullOuterJoin();

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
