/*============================================================================*/
/* Shell.cpp -- shell commands                                                */
/*============================================================================*/

#include "Common.h"

/*----------------------------------------------------------------------------*/
/* iExecute: execute shell command (returns exit code)                        */
/*----------------------------------------------------------------------------*/
i8 Shell::iExecute(const char* sCommand, SysCall& xSysCall)
{
  xSysCall.vClear();

  i8 iExit = system(sCommand);
  
  if (-1 == iExit || 127 == iExit) {
    xSysCall.vError(errno);
    vWarn("Shell::iExecute : system() error; errno=%lld",errno);
    return iExit;
  }
  
  xSysCall.vSuccess();
  
  return iExit;
}

/*===========================================================================*/
/* end of file                                                               */
/*===========================================================================*/
