/*============================================================================*/
/* Scratch.hpp -- scratch space                                               */
/*============================================================================*/
#pragma once

#include "Common.h"

namespace Common
{
  class Scratch : public Object
  {
  private:
    static const i8 C_WORKAREA = C_512KB;
    
    char m_wWorkarea[C_WORKAREA];
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void* operator new(size_t, Scratch* pScratch);
    void operator delete(void* pAddress);
    Scratch(void);
    ~Scratch(void);

    char* pWorkArea(void);
    
    Footer* pFooter(void);
  };
  
  Scratch* _pScratch(void* pAddress);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
