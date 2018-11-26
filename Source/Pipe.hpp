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
    SysCall m_xSysCall;

  public:
    Pipe(void);
    Pipe(Stz* aFilePath);
   ~Pipe(void) noexcept(false);

    bool bCreate(void);
    bool bCreate(Stz* sPipePath);
 
    SysCall& rSysCall(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
