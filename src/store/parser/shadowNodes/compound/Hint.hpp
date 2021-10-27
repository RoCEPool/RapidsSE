/*============================================================================*/
/* Hint.hpp - a query hint                                                    */
/*============================================================================*/
#pragma once

#include "ShadowNode.hpp"
#include "PlanNode.hpp"

namespace Parser
{
  class QueryNode;

  class Hint : public ShadowNode
  {

  private:
    // Shalt never be instantiated. Static functions only.
    Hint();

  public:
    static QueryNode* pCreate(Text* pHintKey);
    static void vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode);

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1 bIsOfType(PlanNode* pNode);

    // Returns true if a given node is this exact given type.
    static b1 bIsExactType(PlanNode* pNode);

    static Stz* szHintKey(QueryNode* pNode);

    static void vAddHintValue(QueryNode* pNode, QueryNode* pValueNode);

    // Return the first hint value. May be null
    static QueryNode* pFirstValue(QueryNode* pNode);
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
