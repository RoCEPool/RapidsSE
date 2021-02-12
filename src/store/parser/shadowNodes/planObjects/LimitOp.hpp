/*============================================================================*/
/* LimitOp.hpp - operator that limits the maximum number of rows being returned*/
/*============================================================================*/
#pragma once

#include "SqlOperator.hpp"
#include "QueryNodeList.hpp"

namespace Parser
{
  class PlanNode;
  class QueryNode;

  class LimitOp : public SqlOperator
  {
  private:
    // Shalt never be instantiated. Static functions only.
    LimitOp();

  public:
    static QueryNode* pCreate(i8 iLimit, i8 iOffset);
    static void       vDescribe(PlanNode* pNode, s2K& sBuf);
    static void       vExplain(PlanNode* pNode, s48K& sBuf, i8 iIndent);

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1 bIsOfType(PlanNode* pNode);

    static i8 iLimit(QueryNode* pNode);
    static i8 iOffset(QueryNode* pNode);  // returns -1 if not set

  protected:

  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
