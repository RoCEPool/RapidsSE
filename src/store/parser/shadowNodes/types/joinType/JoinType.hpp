/*============================================================================*/
/* JoinType.hpp - an abstract base class of join types used in the plan       */
/*============================================================================*/
#pragma once

#include "Types.hpp"
#include "eJT.h"

namespace Parser
{
  class PlanNode;
  class QueryNode;

  class JoinType : public Types
  {
  private:
    // Shalt never be instantiated. Static functions only.
    JoinType();


  public:
    static QueryNode* pCreate(eJT eJoinType);
    static void vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode);

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1 bIsOfType(PlanNode* pNode);

    // Returns true if a given node is this exact given type.
    static b1 bIsExactType(PlanNode* pNode);

    static Stz* szTypeName(PlanNode* pNode);
    static eJT  eJoinType(PlanNode* pNode);
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
