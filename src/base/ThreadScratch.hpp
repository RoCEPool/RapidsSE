/*============================================================================*/
/* ThreadScratch.hpp -- Thread Scratch area                                   */
/*============================================================================*/
#pragma once

#include "PagingMetrics.hpp"

namespace BASE
{
  class ThreadScratch : public Object
  {
  private:
  
    Lock m_okLock;

    PagingMetrics* m_pPagingMetrics;

    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    ThreadScratch(void);
   ~ThreadScratch(void) noexcept(false);
  
    void vAddDerefMetrics(i8 iObjectNo, eCN eClassNo, i8 iTsc, eDR eAction);
  
    void vDropLock(void);
    void vGrabLock(void);
  
    PagingMetrics* pPagingMetrics(void);
    void vPagingMetrics(PagingMetrics* pPagingMetrics);
  private:

    Footer* aFooter(void);
  };
  
  ThreadScratch* _pThreadScratch(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
