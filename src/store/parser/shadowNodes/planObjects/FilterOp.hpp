/*============================================================================*/
/* FilterOp.hpp - operator that removes rows that don't match the condition   */
/*============================================================================*/
#pragma once

#include "SqlOperator.hpp"
#include "QueryNodeList.hpp"

namespace Parser
{
  class PlanNode;
  class QueryNode;

  class FilterOp : public SqlOperator
  {
  private:
    // Shalt never be instantiated. Static functions only.
    FilterOp();

  public:
    static QueryNode* pCreate(QueryNode* pFilterExpr);
    static void       vDescribe(PlanNode* pNode, s2K& sBuf);
    static void       vExplain(PlanNode* pNode, s48K& sBuf, i8 iIndent);

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1 bIsOfType(PlanNode* pNode);

    static QueryNode* pFilterExpr(QueryNode* pNode);

  protected:

  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
