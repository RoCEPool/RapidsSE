/*============================================================================*/
/* DicLock.hpp -- Dictionary Lock                                             */
/*============================================================================*/
#pragma once

#include "MasterLock.hpp"

namespace STORE
{
  class DicLock : public MasterLock
  {
  private:
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    DicLock(void);
   ~DicLock(void) noexcept(false);
    
  private:
    Footer* aFooter(void);
  };
  
  DicLock* _pDicLock(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
