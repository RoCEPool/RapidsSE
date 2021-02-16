/*============================================================================*/
/* DicLock.hpp -- Dictionary Lock                                             */
/*============================================================================*/
#pragma once

namespace STORE
{
  class DicLock : public Object
  {
  private:
    Lock m_okLock;
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    DicLock(i8 nPartitions);
   ~DicLock(void) noexcept(false);
    
  private:
    Footer* aFooter(void);
  };
  
  DicLock* _pDicLock(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
