/*============================================================================*/
/* DdlLock.hpp -- Data Definition Lock                                        */
/*============================================================================*/
#pragma once

namespace STORE
{
  class DdlLock : public Object
  {
  private:
    Lock m_okLock;
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    DdlLock(i8 nPartitions);
   ~DdlLock(void) noexcept(false);
    
  private:
    Footer* aFooter(void);
  };
  
  DdlLock* _pDdlLock(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
