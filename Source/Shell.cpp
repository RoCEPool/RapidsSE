/*============================================================================*/
/* Shell.cpp -- shell commands                                                */
/*============================================================================*/

#include "System.h"

/*----------------------------------------------------------------------------*/
/* iExecute: execute shell command (returns exit code)                        */
/*----------------------------------------------------------------------------*/
i8 Shell::iExecute(Stz* sCommand, SysCall& xSysCall)
{
  xSysCall.vClear();

  i8 iExit = system(sCommand->aChar());
  
  if (-1 == iExit || 127 == iExit) {
    xSysCall.vError(errno);
    vWarn("Shell::iExecute : system() error; errno=%lld"_Stz,errno);
    return iExit;
  }
  
  xSysCall.vSuccess();
  
  return iExit;
}

/*===========================================================================*/
/* end of file                                                               */
/*===========================================================================*/
