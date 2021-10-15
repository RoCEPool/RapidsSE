/*============================================================================*/
/* Routing.hpp - how row data should be sent to the parent node               */
/*============================================================================*/
#pragma once

#include "Types.hpp"
#include "store/parser/shadowNodes/types/location/Location.hpp"

namespace Parser
{
  class PlanNode;
  class QueryNode;

  class Routing : public Types
  {
  private:
    // Shalt never be instantiated. Static functions only.
    Routing();


  public:
    enum eRT {
      RT_NONE = 0,
      RT_UNICAST,
      RT_BROADCAST,
      RT_HASHCAST,
      RT_LOCALPARTITION,
      RT_LOCALNODE,
      RT_NENUMS
    };

    static QueryNode* pCreate(eRT eType);

    static void vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode);

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1 bIsOfType(PlanNode* pNode);

    // Returns true if a given node is this exact given type.
    static b1 bIsExactType(PlanNode* pNode);

    // Get an enum that represents the type of routing
    static eRT eRoutingType(PlanNode* pNode);

    // Get a short string that represents the type of routing
    static Stz* szRoutingType(PlanNode* pNode);

    static eRT eDefaultRouting(Location::eLO eFromLocation, Location::eLO eToLocation);
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
