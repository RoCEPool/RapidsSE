/*============================================================================*/
/* StageRange.hpp - an object that identifies a range of SQL compiler stages  */
/*============================================================================*/
#pragma once

#include "ShadowNode.hpp"

namespace Parser
{
  class QueryNode;

  class StageRange : public ShadowNode
  {

  private:
    // Shalt never be instantiated. Static functions only.
    StageRange();

  public:
    static Stz* C_RANGE_ALL;

    static QueryNode* pCreate(QueryNode* pBegin, QueryNode* pEnd);
    static QueryNode* pCreateLastOnly();
    static void vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode);

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1 bIsOfType(PlanNode* pNode);

    // Returns true if a given node is this exact given type.
    static b1 bIsExactType(PlanNode* pNode);

    // Returns true if the stage num/name is contained in this range
    static b1 bInRange(QueryNode* pNode, i8 iStageNum, Stz* szStageName, QueryNode* pStageNameList);

    // Is the current stage equal to or beyond the end range?
    static b1 bAtOrBeyondEnd(QueryNode* pNode, i8 iStageNum, QueryNode* pStageNameList);

  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
