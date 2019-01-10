/*============================================================================*/
/* Steps_Shell.cpp - shell step group                                         */
/*============================================================================*/

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Shell(void)
{
  vAddStep("shell.execute"_Stz,  &Step::vShellExecute);
}

/*----------------------------------------------------------------------------*/
/* shell.execute -- execute given shell command                               */
/*----------------------------------------------------------------------------*/
void Step::vShellExecute(MP_Task mpTask)
{
  ENTRY("vShellExecute"_Stz);
  
  GenVar xCommand;
  
  if ( ! mpTask->m_pDataStack->bPop(xCommand) )
    THROW_INVALID_DSTACK("shell.execute"_Stz);
  
  SysCall oSysCall;
  
  i8 iExitStatus = Shell::iExecute(_pStrVal(xCommand.pNestedValueSelf())->aStz(),oSysCall);
  
  mpTask->m_pDataStack->vPushInt(iExitStatus);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
