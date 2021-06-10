/*============================================================================*/
/* Verbose.hpp - Verbose                                                      */
/*============================================================================*/
#pragma once

#include "eVB.h"

namespace BASE
{    
  class Verbose : public Object
  {
  private:
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Verbose(void);
   ~Verbose(void) noexcept(false);
   
  private:
    Footer* aFooter(void);
  };
    
  Verbose* _pVerbose(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
