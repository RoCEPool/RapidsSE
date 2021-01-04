/*============================================================================*/
/* TableScanOp.hpp - an operator that linearly reads all rows in a table      */
/*============================================================================*/
#pragma once

#include "SqlOperator.hpp"
#include "DataSource.hpp"

namespace Parser
{
  class PlanNode;
  class QueryNode;

  class TableScanOp : public DataSource
  {
  private:
    // Shalt never be instantiated. Static functions only.
    TableScanOp();

  public:
    static QueryNode* pCreate(QueryNode* pTableRef);
    static void       vDescribe(PlanNode* pNode, s2K& sBuf);

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1 bIsOfType(PlanNode* pNode);

  protected:

  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
