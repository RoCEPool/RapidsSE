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

  i8 iExit = system(sCommand->aChar());
  
  if (-1 == iExit || 127 == iExit) {
    oSysCall.vError(errno);
    vWarn("Shell::iExecute : system() error; errno=%lld"_Stz,errno);
    return iExit;
  }
  
  oSysCall.vSuccess();
  
  return iExit;
}

/*===========================================================================*/
/* end of file                                                               */
/*===========================================================================*/
