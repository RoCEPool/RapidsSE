/*============================================================================*/
/* LogicalNode.hpp - a base class of all logical graph nodes                  */
/*============================================================================*/
#pragma once

namespace Parser
{
  class LogicalNode
  {

  private:
    // Shalt never be instantiated. Static functions only.
    LogicalNode();

  public:
    // Validate a node against its definition
    static b1 bValidate(PlanNode* pPlanNode);

  public:


  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
