/*============================================================================*/
/* SqlVarchar.hpp - an SQL varchar data type used in the plan                 */
/*============================================================================*/
#pragma once

#include "SqlType.hpp"

namespace Parser
{
  class PlanNode;
  class QueryNode;

  class SqlVarchar : public SqlType
  {

  private:
    // Shalt never be instantiated. Static functions only.
    SqlVarchar();

  public:
    static void vDescribe(PlanNode* pNode, s2K& sBuf);
    static PlanNode* pCreate();
    static PlanNode* pCreate(i8 iLength);

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1 bIsOfType(PlanNode* pNode);
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
