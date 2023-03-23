/*============================================================================*/
/* Shell.cpp -- shell commands                                                */
/*============================================================================*/

#include <stdio.h>
#include "BASE.h"
#include "Grid.hpp"

/*----------------------------------------------------------------------------*/
/*-- Static Functions---------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

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

  //char acSave[1024];
  //acSave[sizeof(acSave)-1] = 0;
  //strncpy(acSave,sCommand->azChar(), sizeof(acSave)-1);

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

/*----------------------------------------------------------------------------*/
/* Execute shell command, capture stdout (to a limit) & return exit code      */
/*----------------------------------------------------------------------------*/
i8 Shell::iExecute(Stz* sCommand, s48K& oStdOut)
{
  // Note: if you are expecting more than 48K of stdout then this will only
  // return the first 48K worth. Consider redirecting stdout to a file.
  // If you need stderr then either redirect it to a separate file, or redirect
  // it into the stdout stream in the command with 2>&1.

  FILE* pProcess = popen(sCommand->azChar(), "r");

  if (!pProcess) {
    vTrace("Shell::iExecute : popen() error; errno=%lld"_Stz,errno);
    return -1;
  }

//  bool bMoreToRead = true;
//  i8 iTotalSpace = oStdOut.nSpace() - 1;
  i8 iBytesRead;
  i8 iBytesToRead = oStdOut.nSpace() - 1;

  iBytesRead = (i8) fread(oStdOut.azChar(), 1, iBytesToRead, pProcess);
  *(oStdOut.azChar() + iBytesRead) = 0;

  // If there's any more to read then we will discard it
  s2K oDiscardStr;
  while (iBytesRead == iBytesToRead) {
    iBytesToRead = oDiscardStr.nSpace() - 1;
    iBytesRead = (i8) fread(oDiscardStr.azChar(), 1, iBytesToRead, pProcess);
  }

  i8 iReturnVal = pclose(pProcess);
  return iReturnVal;
}

/*===========================================================================*/
/* end of file                                                               */
/*===========================================================================*/
