/*============================================================================*/
/* Pipe.hpp -- os pipe                                                        */
/*============================================================================*/
#pragma once

namespace System
{
  class Pipe : public Object
  {
  private:
    s256    m_sFilePath;   // the path of the file (relative or absolute)
    SysCall m_xSysCall;

  public:
    Pipe(void);
    Pipe(Stz* aFilePath, i8 iTraceLevel=0);
  
    bool bCreate(void);
    bool bCreate(Stz* sPath);
 
    SysCall& rSysCall(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
