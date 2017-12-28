/*============================================================================*/
/* PerformanceTrace.hpp -- Performance Trace                                  */
/*============================================================================*/
#pragma once

namespace Utilities
{
  class PerformanceTrace : public Object
  {
    private:
  
    Timer  m_xTimer; // timer to compute time delta's
  
    EntryBuffer* m_pFirstBuffer;  // first buffer in trace sequence
    EntryBuffer* m_pLastBuffer;   // last buffer in trace sequence
  
    // must be last
    Footer m_xFooter;
    
    public:
    
    void* operator new(size_t);
    void  operator delete(void*);
    
    PerformanceTrace(void);
    ~PerformanceTrace(void);
  
    Footer* aFooter(void);
  };
  
  PerformanceTrace* _pPerformanceTrace(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
