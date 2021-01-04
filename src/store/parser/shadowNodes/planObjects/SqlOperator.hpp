/*============================================================================*/
/* SqlOperator.hpp - an abstract base class of all objects that process       */
/* streams of row data.                                                       */
/*============================================================================*/
#pragma once

#include "PlanObject.hpp"

namespace Parser
{
  class PlanNode;

  class SqlOperator : public PlanObject
  {

  private:
    // Shalt never be instantiated. Static functions only.
    SqlOperator();

  public:
    static void vDescribe(PlanNode* pNode, s2K& sBuf);

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1 bIsOfType(PlanNode* pNode);

  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
