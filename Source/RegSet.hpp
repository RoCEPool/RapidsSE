/*============================================================================*/
/* RegSet.hpp -- task register set                                            */
/*============================================================================*/
#pragma once

namespace Tasks
{
  class RegSet : public Object
  {
  public:
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    RegSet(void);
   ~RegSet(void) noexcept(false);
        
    Footer* aFooter(void);
  };
  
  RegSet* _pRegSet(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
