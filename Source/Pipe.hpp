/*============================================================================*/
/* Pipe.hpp -- os pipe                                                        */
/*============================================================================*/
#pragma once

namespace System
{
  class Pipe : public Object
  {
  private:
    s256    m_sPipePath;   // the path of the file (relative or absolute)
    SysCall m_oSysCall;

    // must be last
    Footer m_xFooter;

  public:
    Pipe(void);
    Pipe(Stz* aFilePath);
   ~Pipe(void) noexcept(false);

    static bool bCreate(Stz* sPipePath);
    static bool bRemove(Stz* sPipePath);

    SysCall& oSysCall(void);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
