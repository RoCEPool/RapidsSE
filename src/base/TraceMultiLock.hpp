/*============================================================================*/
/* TraceMultiLock.hpp -- Trace Multi-Line Lock                                */
/*============================================================================*/
#pragma once

#include "MasterLock.hpp"

namespace BASE
{
  class TraceMultiLock : public MasterLock
  {
  private:
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    TraceMultiLock(void);
   ~TraceMultiLock(void) noexcept(false);
    
  private:
    Footer* aFooter(void);
  };
  
  TraceMultiLock* _pTraceMultiLock(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
