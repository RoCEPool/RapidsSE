/*============================================================================*/
/* Planner.hpp - Planner                                                      */
/*============================================================================*/
#pragma once

namespace Plans
{  
  class Planner : public Object
  {
    private:
  
    // must be last
    Footer m_xFooter;
    
    public:
    
    void* operator new(size_t);
    void  operator delete(void*);
    
    Planner(void);
    ~Planner(void);
  
    Footer* pFooter(void);
  };
  
  Planner* _pPlanner(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
