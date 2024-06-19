/*============================================================================*/
/* Steps_Shell.cpp - shell step group                                         */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"


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
  
  SenCell xCommand;
  
  m_oDataStack.vPopSenCell(xCommand, __func__);
  
  SysCall oSysCall;
  
  i8 iExitStatus = Shell::iExecute(xCommand.szStz(),oSysCall);
  
  m_oDataStack.vPushInt(iExitStatus);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
