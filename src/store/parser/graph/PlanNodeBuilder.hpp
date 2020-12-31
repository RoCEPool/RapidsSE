/*============================================================================*/
/* PlanNodeBuilder.hpp - A mutable way to build PlanNodes                     */
/*============================================================================*/
#pragma once

#include "PlanNode.hpp"
#include "PlanNodeBuilderList.hpp"

namespace Parser
{
  class PlanNodeBuilder;

  class PlanNodeBuilder : public Object
  {
  public:
    s256  m_sName;
    s256  m_sType;
    Value m_xValue;
    PlanNodeBuilderList* m_pProperties;
    PlanNodeBuilderList* m_pChildren;
    s256  m_sPath;
    
  private:
    Footer m_xFooter;
    
   public:
    void* operator new(size_t);
    void  operator delete(void*);

    PlanNodeBuilder();
    ~PlanNodeBuilder(void);

    Footer* aFooter(void);

    PlanNode* pBuild();

  private:
    PlanNodeList* pBuildList(PlanNodeBuilderList* builderList);
    void vSetPath(s256& sParentPath);
  };
  
  PlanNodeBuilder* _pPlanNodeBuilder(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
