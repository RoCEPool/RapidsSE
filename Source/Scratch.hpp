/*============================================================================*/
/* Scratch.hpp -- scratch area                                                */
/*============================================================================*/
#pragma once

#include "Common.h"

namespace Common
{
  class Scratch : public Object,
                  public WspFxd<C_512KB>
  {
  private:
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void* operator new(size_t, Scratch* pScratch);
    void operator delete(void* pAddress);
    Scratch(void);
    ~Scratch(void);

    char* aScratchArea(void);
    
    Footer* pFooter(void);
  };
  
  Scratch* _pScratch(void* pAddress);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
