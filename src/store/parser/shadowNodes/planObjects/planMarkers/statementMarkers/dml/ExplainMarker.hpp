/*============================================================================*/
/* ExplainMarker.hpp - identifies the subgraph that follows is an EXPLAIN stmt*/
/*============================================================================*/
#pragma once

#include "PlanObject.hpp"
#include "DmlMarker.hpp"

namespace Parser
{
  class PlanNode;
  class QueryNode;

  class ExplainMarker : public DmlMarker
  {
  private:
    // Shalt never be instantiated. Static functions only.
    ExplainMarker();

  public:
    static QueryNode* pCreate(Stz* szQueryText);

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1 bIsOfType(PlanNode* pNode);
    static b1 bIsOfType(Stz* pNode);

    // Returns true if a given node is this exact given type.
    static b1 bIsExactType(PlanNode* pNode);
    static b1 bIsExactType(Stz* pNode);

    // Explains any common properties of parent definitions
    static void vExplainCommon(PlanNode *pNode, PlanNode* pRootNode, Stz* szType, s48K &sBuf, i8 iIndent);
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
