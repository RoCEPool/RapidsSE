/*============================================================================*/
/* Location.hpp - cardinality & where sql operators can execute in the cluster*/
/*============================================================================*/
#pragma once

#include "Types.hpp"

namespace Parser
{
  class PlanNode;
  class QueryNode;

  class Location : public Types
  {
  private:
    // Shalt never be instantiated. Static functions only.
    Location();


  public:
    enum eLO {
      LO_NONE = 0,
      LO_ORIGIN,
      LO_ALL_NODES,
      LO_ALL_PARTITIONS,
      LO_NENUMS
    };

    static QueryNode* pCreate(eLO eType);

    static void vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode);

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1 bIsOfType(PlanNode* pNode);

    // Returns true if a given node is this exact given type.
    static b1 bIsExactType(PlanNode* pNode);

    // Get an enum that represents the type of location
    static eLO eLocationType(PlanNode* pNode);

    // Get a short string that represents the type of location
    static Stz* szLocationType(PlanNode* pNode);

  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
