/*============================================================================*/
/* TraceLock.hpp -- Trace Lock                                                */
/*============================================================================*/
#pragma once

#include "MasterLock.hpp"

namespace STORE
{
  class TraceLock : public MasterLock
  {
  private:
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    TraceLock(void);
   ~TraceLock(void) noexcept(false);
    
  private:
    Footer* aFooter(void);
  };
  
  TraceLock* _pTraceLock(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
