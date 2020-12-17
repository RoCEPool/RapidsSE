/*============================================================================*/
/* MutablePlanNode.hpp - An node of the plan graph that can be modified       */
/*============================================================================*/
#pragma once

namespace Parser
{
  class MutablePlanNode : public PlanNode
  {
  protected:

  private:
    Footer m_xFooter;

  public:
    virtual ~MutablePlanNode(void) = 0;

  protected:
    MutablePlanNode(eCN eClassNo, Stz* szName, Stz* szType, Value xValue,
                    PlanNodeList* pProperties, PlanNodeList* pChildren);

    Footer* aFooter(void);

  public:
    void vSetName(Stz* szName);
    void vSetType(Stz* szType);
    void vSetValue(Value xValue);
    void vAppendProperty(PlanNode* pNode);
    void vAppendChild(PlanNode* pNode);

  };
  
  MutablePlanNode* _pMutablePlanNode(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
