/*============================================================================*/
/* Shell.cpp -- shell commands                                                */
/*============================================================================*/

#include "BASE.h"
#include "Node.hpp"
#include "Grid.hpp"

/*----------------------------------------------------------------------------*/
/* execute shell command (returns exit code)                                  */
/*----------------------------------------------------------------------------*/
i8 Shell::iExecute(Stz* sCommand, SysCall& oSysCall)
{
  oSysCall.vClear();

  char acSave[1024];
  acSave[sizeof(acSave)-1] = 0;
  strncpy(acSave,sCommand->azChar(), sizeof(acSave)-1);

  printf("COMMAND SHELL Execute: %s\n",sCommand->azChar() );

  i8 iResult = system(sCommand->azChar());
  
  if (-1 == iResult || 127 == iResult) {
    oSysCall.vError(errno);
    vWarn("Shell::iExecute : system() error; errno=%lld"_Stz,errno);
    return iResult;
  }
  
  oSysCall.vSuccess();

  // Try to save the node command to restart
  char* pCommand = sCommand->azChar();
  char* token = strtok(pCommand, " ");

  while (token != NULL) {
    token = strtok(NULL, " ");
    if (strcmp(token,"--name") == 0) {
      token = strtok(NULL, " ");
      printf("FOUND %s\n", token);
      Node* pNode = AM::pGrid()->pFindNode((Stz*)token);
      if (pNode) {
        pNode->vSetStartCmd((Stz*)acSave);
      }
      break;
    }
  }

  
  return iResult;
}

/*===========================================================================*/
/* end of file                                                               */
/*===========================================================================*/
