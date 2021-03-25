/*============================================================================*/
/* UnionOp.hpp - a union of two SELECT queries                                */
/*============================================================================*/
#pragma once

#include "SetOp.hpp"

namespace Parser
{
  class PlanNode;
  class QueryNode;

  class UnionOp : public SetOp
  {
  private:
    // Shalt never be instantiated. Static functions only.
    UnionOp();

  public:
    static QueryNode* pCreate(b1 bIsDistinct, QueryNode* pCorrelatedByList);
    static void       vDescribe(PlanNode* pNode, s2K& sBuf);
    static void       vExplain(PlanNode *pNode, PlanNode *pRootNode, s48K &sBuf, i8 iIndent);

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1 bIsOfType(PlanNode* pNode);

    // Returns true if a given node is this exact given type.
    static b1 bIsExactType(PlanNode* pNode);

  protected:

  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
