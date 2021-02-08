/*============================================================================*/
/* SqlDecimal.hpp - an SQL exact-numeric decimal data type used in the plan   */
/*============================================================================*/
#pragma once

#include "SqlType.hpp"

namespace Parser
{
  class PlanNode;
  class QueryNode;

  class SqlDecimal : public SqlType
  {

  private:
    // Shalt never be instantiated. Static functions only.
    SqlDecimal();

  public:
    static void vDescribe(PlanNode* pNode, s2K& sBuf);
    PlanNode*   pCreate(i8 iPrecision, i8 iScale);

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1 bIsOfType(PlanNode* pNode);
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
