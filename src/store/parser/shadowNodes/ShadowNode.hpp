/*============================================================================*/
/* ShadowNode.hpp - a base class of all logical graph nodes                   */
/*============================================================================*/
#pragma once

#include <functional> // for std::function

namespace Parser
{
  class PlanNode;
  class QueryNode;

  class ShadowNode
  {

  private:
    static Atomic* ms_pObjId;

    // Shalt never be instantiated. Static functions only.
    ShadowNode();

  public:
    // Describe the object and write it into the buffer
    static void vDescribe(PlanNode* pPlanNode, s2K& sBuf, PlanNode* pRootNode);

  protected:
    // Validate that a node is of the expected type and that it matches its definition.
    // The fTypeValidator param is exected to be something like the bIsOfType() function.
    static b1 bValidate(QueryNode* pQueryNode, PlanNode* pPlanRoot,
                        std::function<b1(PlanNode*)> fTypeValidator);

    // Returns true if a given node is a given type or is derived from that type.
    static b1 bIsOfType(PlanNode* pNode, Stz* szTypePath);
    static b1 bIsOfType(Stz* szNodeType, Stz* szTypePath);

    // Returns true if a given node is this exact given type.
    static b1 bIsExactType(PlanNode* pNode, Stz* szTypePath);
    static b1 bIsExactType(Stz* szNodeType, Stz* szTypePath);

  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
