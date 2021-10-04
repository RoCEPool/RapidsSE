/*============================================================================*/
/* Shell.hpp -- shell commands                                                */
/*============================================================================*/
#pragma once

namespace BASE
{
  class Shell
  {
  public:
    static i8 iEdit(Object* pCaller, Stz* sFilePath, SysCall& SysCall);
    static i8 iExecute(Stz* sCommand, SysCall& SysCall);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
