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
  ENTRY(); // ENTRY("vShellExecute"_Stz);
  
  GenVar xCommand;
  
  if ( ! m_pDataStack->bPopGenVar(xCommand) )
    THROW_DSTACK();
  
  SysCall oSysCall;
  
  i8 iExitStatus = Shell::iExecute(xCommand.szStz(),oSysCall);
  
  m_pDataStack->vPushI8(iExitStatus);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
