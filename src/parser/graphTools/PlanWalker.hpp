/*============================================================================*/
/* PlanWalker.hpp - a utility for walking plan graphs                         */
/*============================================================================*/
#pragma once

namespace Parser
{
  class PlanWalker : public Object
  {
  private:
    Footer m_xFooter;

  public:

    void* operator new(size_t);
    void  operator delete(void*);

    PlanWalker();
    ~PlanWalker(void);

    Footer* aFooter(void);


  };

  PlanWalker* _pPlanWalker(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
