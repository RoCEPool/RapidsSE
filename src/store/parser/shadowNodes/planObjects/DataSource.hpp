/*============================================================================*/
/* DataSource.hpp - a source of row data into the query                       */
/*============================================================================*/
#pragma once

#include "SqlOperator.hpp"

namespace Parser
{
  class PlanNode;

  class DataSource : public SqlOperator
  {

  private:
    // Shalt never be instantiated. Static functions only.
    DataSource();

  public:
    static void vDescribe(PlanNode* pNode, s2K& sBuf);

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1 bIsOfType(PlanNode* pNode);

  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
