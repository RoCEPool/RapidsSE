/*============================================================================*/
/* Arrange.hpp -- Arrange things in alphabetic order                          */
/*============================================================================*/
#pragma once

namespace Utilities
{
  class Arrange : public Object
  {
  private:
    Lock m_okLock;     // lock
  
    SkipListNL* m_pSkipList; // skip list of things to be arranged
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Arrange(CompareFp fpCompare);
   ~Arrange(void) noexcept(false);
    
    Footer* aFooter(void);
  };
  
  Arrange* _pArrange(void* aAddr);
}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
