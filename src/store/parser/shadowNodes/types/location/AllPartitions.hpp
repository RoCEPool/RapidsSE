/*============================================================================*/
/* AllPartitions.hpp - Runs on each partition in the cluster                  */
/*============================================================================*/
#pragma once

#include "store/parser/shadowNodes/types/location/Location.hpp"


namespace Parser
{
  class PlanNode;
  class QueryNode;

  class AllPartitions : public Location
  {
  private:
    // Shalt never be instantiated. Static functions only.
    AllPartitions();


  public:
    static QueryNode* pCreate();
    static void vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode);

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1 bIsOfType(PlanNode* pNode);

    // Returns true if a given node is this exact given type.
    static b1 bIsExactType(PlanNode* pNode);
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
