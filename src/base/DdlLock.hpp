/*============================================================================*/
/* DdlLock.hpp -- Data Definition Lock                                        */
/*============================================================================*/
#pragma once

#include "MasterLock.hpp"

namespace STORE
{
  class DdlLock : public MasterLock
  {
  private:
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    DdlLock(void);
   ~DdlLock(void) noexcept(false);
    
  private:
    Footer* aFooter(void);
  };
  
  DdlLock* _pDdlLock(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
