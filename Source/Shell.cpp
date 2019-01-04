/*============================================================================*/
/* Shell.cpp -- shell commands                                                */
/*============================================================================*/

#include "System.h"

/*----------------------------------------------------------------------------*/
/* iExecute: execute shell command (returns exit code)                        */
/*----------------------------------------------------------------------------*/
i8 Shell::iExecute(Stz* sCommand, SysCall& oSysCall)
{
  oSysCall.vClear();

  i8 iResult = system(sCommand->aChar());
  
  if (-1 == iResult || 127 == iResult) {
    oSysCall.vError(errno);
    vWarn("Shell::iExecute : system() error; errno=%lld"_Stz,errno);
    return iResult;
  }
  
  oSysCall.vSuccess();
  
  return iResult;
}

/*===========================================================================*/
/* end of file                                                               */
/*===========================================================================*/
