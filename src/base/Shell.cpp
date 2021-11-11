/*============================================================================*/
/* Shell.cpp -- shell commands                                                */
/*============================================================================*/

#include "BASE.h"
#include "Node.hpp"
#include "Grid.hpp"

/*----------------------------------------------------------------------------*/
/* edit shell command (returns exit code)                                     */
/*----------------------------------------------------------------------------*/
i8 Shell::iEdit(Object* pCaller, Stz* sFilePath, SysCall& oSysCall)
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // shell command to edit the file
  s256 sShellCommand;
  sShellCommand.vSprint("vim %s"_Stz, sFilePath);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // open file via vim
  i8 iExitStatus = Shell::iExecute(sShellCommand.szStz(),oSysCall);
  if ( iExitStatus != 0 ) {
    vTrace("Unable to vim file: %s\n"_Stz,sFilePath);
    return false;
  }

  return true;
}

/*----------------------------------------------------------------------------*/
/* execute shell command (returns exit code)                                  */
/*----------------------------------------------------------------------------*/
i8 Shell::iExecute(Stz* sCommand, SysCall& oSysCall)
{
  oSysCall.vClear();

  char acSave[1024];
  acSave[sizeof(acSave)-1] = 0;
  strncpy(acSave,sCommand->azChar(), sizeof(acSave)-1);

  //printf("COMMAND SHELL Execute: %s\n",sCommand->azChar() );

  i8 iResult = system(sCommand->azChar());
  
  if (-1 == iResult || 127 == iResult) {
    oSysCall.vError(errno);
    vTrace("Shell::iExecute : system() error; errno=%lld"_Stz,errno);
    return iResult;
  }
  
  oSysCall.vSuccess();

  return iResult;
}

/*===========================================================================*/
/* end of file                                                               */
/*===========================================================================*/
