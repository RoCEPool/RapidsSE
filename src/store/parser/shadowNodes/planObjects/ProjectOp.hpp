/*============================================================================*/
/* ProjectOp.hpp - operator that changes the defn of rows that it processes   */
/*============================================================================*/
#pragma once

#include "SqlOperator.hpp"
#include "QueryNodeList.hpp"

namespace Parser
{
  class PlanNode;
  class QueryNode;

  class ProjectOp : public SqlOperator
  {
  private:
    // Shalt never be instantiated. Static functions only.
    ProjectOp();

  public:
    static QueryNode* pCreate(QueryNodeList* pSelectItems);
    static void       vDescribe(PlanNode* pNode, s2K& sBuf);
    static void       vExplain(PlanNode* pNode, s48K& sBuf, i8 iIndent);

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1 bIsOfType(PlanNode* pNode);

    static QueryNode* pProjectedExprs(QueryNode* pNode);

  protected:

  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
