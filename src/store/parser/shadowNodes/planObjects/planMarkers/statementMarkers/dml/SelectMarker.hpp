/*============================================================================*/
/* SelectMarker.hpp - identifies the subgraph that follows is a SELECT stmt   */
/*============================================================================*/
#pragma once

#include "PlanObject.hpp"
#include "DmlMarker.hpp"

namespace Parser
{
  class PlanNode;
  class QueryNode;

  class SelectMarker : public DmlMarker
  {
  private:
    // Shalt never be instantiated. Static functions only.
    SelectMarker();

  public:
    static QueryNode* pCreate(Stz* szQueryText);

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1 bIsOfType(PlanNode* pNode);
    static b1 bIsOfType(Stz* pNode);

    // Returns true if a given node is this exact given type.
    static b1 bIsExactType(PlanNode* pNode);
    static b1 bIsExactType(Stz* pNode);

    // Explains any common properties of parent definitions
    static void vExplainCommon(PlanNode *pNode, PlanNode* pRootNode, QueryNode* pExplainNode, Stz* szType, s48K &sBuf, i8 iIndent);
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
