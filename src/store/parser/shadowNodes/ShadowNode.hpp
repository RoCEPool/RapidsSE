/*============================================================================*/
/* ShadowNode.hpp - a base class of all logical graph nodes                   */
/*============================================================================*/
#pragma once

namespace Parser
{
  class PlanNode;

  class ShadowNode
  {

  private:
    static Atomic* ms_pObjId;

    // Shalt never be instantiated. Static functions only.
    ShadowNode();

  public:
    // Validate a node against its definition
    static b1 bValidate(PlanNode* pPlanNode);

    // Describe the object and write it into the buffer
    static void vDescribe(PlanNode* pPlanNode, s2K& sBuf);

  protected:
    // Creates a unique ID for an object name
    static void vObjName(Stz* szPrefix, s256& sBuf);

    // Returns true if a given node is a given type or is derived from that type.
    static b1 bIsOfType(PlanNode* pNode, Stz* szTypePath);

    // Returns true if a given node is has an exact given type.
    static b1 bIsExactType(PlanNode* pNode, Stz* szTypePath);

  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
