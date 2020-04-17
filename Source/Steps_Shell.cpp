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
void Step::vShellExecute(void)
{
  ENTRY();
  
  Datum xCommand;
  
  m_pDataStack->vPopVal(xCommand,__func__);
  
  SysCall oSysCall;
  
  i8 iExitStatus = Shell::iExecute(xCommand.szStz(),oSysCall);
  
  m_pDataStack->vPushInt(iExitStatus);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
