/*============================================================================*/
/* Steps_Args.cpp - args step group                                           */
/*============================================================================*/

#include "StorageEngine.h"
#include "SePrompt.hpp"

extern AttachMap* gpAttachMap;
extern Arguments  gxArguments;

// args: ///////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Args(void)
{
  vAddStep("args.oscillate"_Stz,  &Task::vArgsOscillate);
  vAddStep("args.print"_Stz,      &Task::vArgsPrint);
}

/*----------------------------------------------------------------------------*/
/* args.oscillate -- oscillate the dummy parameters                           */
/*----------------------------------------------------------------------------*/
void Task::vArgsOscillate(MP_Task mpTask)
{
  ENTRY("vArgsOscillate"_Stz);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* args.print -- print arguments                                              */
/*----------------------------------------------------------------------------*/
void Task::vArgsPrint(MP_Task mpTask)
{
ENTRY("vArgsPrint"_Stz);
  
  gxArguments.vPrintStartupInfo();
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
