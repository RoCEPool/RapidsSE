/*============================================================================*/
/* Fifo.hpp -- os fifo                                                        */
/*============================================================================*/
#pragma once

#include "Resource.hpp"

namespace BASE
{
  class Fifo : public Resource
  {
  private:
    SysCall m_oSysCall;

    // must be last
    Footer m_xFooter;

  public:
    Fifo(void);
    Fifo(Stz* szFifoPath);
   ~Fifo(void) noexcept(false);

    static bool bMake(Stz* szFifoPath);
    static bool bRemove(Stz* szFifoPath);

    SysCall& oSysCall(void);
  
    void vOutline(s4K& sOutline);
  
    Footer* aFooter(void);
  };
  
  Fifo* _pFifo(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
