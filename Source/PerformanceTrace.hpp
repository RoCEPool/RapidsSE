/*============================================================================*/
/* PerformanceTrace.hpp -- Performance Trace                                  */
/*============================================================================*/
#pragma once

namespace Measurement
{
  class PerformanceTrace : public Object
  {
    private:
  
    Timer  m_oTimer; // timer to compute time delta's
  
    PerformanceBuffer* m_pFirstBuffer;  // first buffer of performance entries in trace sequence
    PerformanceBuffer* m_pCurrentBuffer;// last buffer of performance entries in trace sequence
  
    // must be last
    Footer m_xFooter;
    
    public:
    
    void* operator new(size_t);
    void  operator delete(void*);
    
    PerformanceTrace(void);
    ~PerformanceTrace(void) noexcept(false);
  
    void vAdd(Stz* aWord, Stz* aNote);
    void vPrint(void);
    void vReset(void);
  
    PerformanceEntry* newPerformanceEntry(void);
  
    Footer* aFooter(void);
  };
  
  PerformanceTrace* _pPerformanceTrace(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
