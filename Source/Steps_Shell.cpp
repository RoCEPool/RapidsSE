/*============================================================================*/
/* Steps_Shell.cpp - shell step group                                         */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern AttachMap*      gpAttachMap;

// shell: //////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddSteps_Shell(void)
{
  vAddStep("shell.execute"_Stz,  &Task::vShellExecute,  false);
}

/*----------------------------------------------------------------------------*/
/* shell.execute -- execute given shell command                               */
/*----------------------------------------------------------------------------*/
void Task::vShellExecute(MP_Task mpTask)
{
  ENTRY("vShellExecute"_Stz);
  
  ValWrk xCommand;
  
  if ( ! mpTask->m_pDataStack->bPop(xCommand) )
    THROW_INVALID_DSTACK("shell.execute"_Stz);
  
  SysCall xSysCall;
  
  i8 iExitStatus = Shell::iExecute(_pStrVal(xCommand.pNestedValueSelf())->aStr(),xSysCall);
  
  mpTask->m_pDataStack->vPushInt(iExitStatus);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
