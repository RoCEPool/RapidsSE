/*============================================================================*/
/* Thing.hpp -- Arrangeable Thing                                             */
/*============================================================================*/
#pragma once

namespace Utilities
{
  class Thing : public Object
  {
  private:

    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Thing(void);
   ~Thing(void) noexcept(false);
  
    Footer* aFooter(void);
  };
  
  Thing* _pThing(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
