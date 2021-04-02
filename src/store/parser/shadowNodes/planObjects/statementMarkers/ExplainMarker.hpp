/*============================================================================*/
/* ExplainMarker.hpp - identifies the subgraph that follows is an EXPLAIN stmt*/
/*============================================================================*/
#pragma once

#include "PlanObject.hpp"
#include "StatementMarker.hpp"

namespace Parser
{
  class PlanNode;
  class QueryNode;

  class ExplainMarker : public StatementMarker
  {
  private:
    // Shalt never be instantiated. Static functions only.
    ExplainMarker();

  public:
    static QueryNode* pCreate(Stz* szQueryText);

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1 bIsOfType(PlanNode* pNode);

    // Returns true if a given node is this exact given type.
    static b1 bIsExactType(PlanNode* pNode);
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
