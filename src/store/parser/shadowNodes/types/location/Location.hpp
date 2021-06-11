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
//    static QueryNode* pCreate(eCT eType);
    static void vDescribe(PlanNode* pNode, s2K& sBuf);

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1 bIsOfType(PlanNode* pNode);

    // Returns true if a given node is this exact given type.
    static b1 bIsExactType(PlanNode* pNode);

    // Returns a string representing the name of the location enumeration
    static Stz* szLocationName(PlanNode* pNode);
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
