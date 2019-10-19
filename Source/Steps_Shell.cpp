/*============================================================================*/
/* Steps_Shell.cpp - shell step group                                         */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Shell(void)
{
  vAddStep("shell::execute"_Stz,  &Step::vShellExecute);
}

/*----------------------------------------------------------------------------*/
/* shell::execute -- execute given shell command                              */
/*----------------------------------------------------------------------------*/
void Step::vShellExecute(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vShellExecute"_Stz);
  
  GenVar xCommand;
  
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(xCommand) )
    vThrow(__func__,__FILE__,__LINE__,FK_DSTACK,__STEP__);
  
  SysCall oSysCall;
  
  i8 iExitStatus = Shell::iExecute(xCommand.szStz(),oSysCall);
  
  mpTask->m_pForth->m_pDataStack->vPushI8(iExitStatus);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
