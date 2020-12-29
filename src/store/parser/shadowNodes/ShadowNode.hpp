/*============================================================================*/
/* ShadowNode.hpp - a base class of all logical graph nodes                   */
/*============================================================================*/
#pragma once

namespace Parser
{
  class ShadowNode
  {

  private:
    static Atomic* ms_pObjId;

    // Shalt never be instantiated. Static functions only.
    ShadowNode();

  public:
    // Validate a node against its definition
    static b1 bValidate(PlanNode* pPlanNode);

  protected:
    // Creates a unique ID for an object name
    static void vObjName(Stz* szPrefix, s256& sBuf);

  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
