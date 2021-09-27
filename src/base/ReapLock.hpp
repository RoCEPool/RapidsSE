/*============================================================================*/
/* ReapLock.hpp -- Reap Lock                                                  */
/*============================================================================*/
#pragma once

#include "MasterLock.hpp"

namespace BASE
{
  class ReapLock : public MasterLock
  {
  private:
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    ReapLock(void);
   ~ReapLock(void) noexcept(false);
    
  private:
    Footer* aFooter(void);
  };
  
  ReapLock* _pReapLock(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
