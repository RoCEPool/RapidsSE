/*============================================================================*/
/* PerformanceEntry.hpp -- Performance Entry                                  */
/*============================================================================*/
#pragma once

namespace Utilities
{
  class PerformanceEntry : public Entry
  {
    private:
  
    // must be last
    Footer m_xFooter;
    
    public:
    
    void* operator new(size_t);
    void  operator delete(void*);
    
    PerformanceEntry(void);
    ~PerformanceEntry(void);
  
    Footer* aFooter(void);
  };
  
  PerformanceEntry* _pPerformanceEntry(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
