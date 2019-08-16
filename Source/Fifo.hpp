/*============================================================================*/
/* Fifo.hpp -- os fifo                                                        */
/*============================================================================*/
#pragma once

namespace Resources
{
  class Fifo : public Resource
  {
  private:
    s256    m_sFifoPath;   // the path of the file (relative or absolute)
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
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
