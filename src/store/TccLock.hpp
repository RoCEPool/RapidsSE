/*============================================================================*/
/* TccLock.hpp -- Tiny-C Lock                                                 */
/*============================================================================*/
#pragma once

#include "MasterLock.hpp"

namespace STORE
{
  class TccLock : public MasterLock
  {
  private:
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    TccLock(void);
   ~TccLock(void) noexcept(false);
    
  private:
    Footer* aFooter(void);
  };
  
  TccLock* _pTccLock(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
