/*============================================================================*/
/* SqlBoolean.hpp - an SQL boolean data type used in the plan                 */
/*============================================================================*/
#pragma once

#include "SqlType.hpp"

namespace Parser
{
  class PlanNode;
  class QueryNode;

  class SqlBoolean : public SqlType
  {

  private:
    // Shalt never be instantiated. Static functions only.
    SqlBoolean();

  public:
    static void vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode);
    static PlanNode* pCreate();

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1 bIsOfType(PlanNode* pNode);

    // Returns true if a given node is this exact given type.
    static b1 bIsExactType(PlanNode* pNode);
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
