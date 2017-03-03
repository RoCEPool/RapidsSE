/*============================================================================*/
/* Scratch.hpp -- scratch area                                                */
/*============================================================================*/
#pragma once

#include "System.h"

namespace System
{
  class Scratch : public Object
  {
  private:
    WspFxd<C_512KB> m_xWsp;
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void* operator new(size_t, Scratch* pScratch);
    void operator delete(void* aAddress);
    Scratch(void);
    ~Scratch(void);

    char* aWspArea(void);
    i8    nWspLength(void);
  
    Footer* pFooter(void);
  };
  
  Scratch* _pScratch(void* aAddress);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
