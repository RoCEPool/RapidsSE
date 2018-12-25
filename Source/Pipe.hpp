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

    bool bCreate(void);
    bool bCreate(Stz* sPipePath);
  
    bool bRemove(void);
    bool bRemove(Stz* sPipePath);

    SysCall& oSysCall(void);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
