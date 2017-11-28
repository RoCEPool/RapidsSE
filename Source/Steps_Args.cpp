/*============================================================================*/
/* Steps_Args.cpp - args step group                                           */
/*============================================================================*/

#include "StorageEngine.h"
#include "SePrompt.hpp"

extern AttachMap*      gpAttachMap;

extern Arguments       gxArguments;
extern NP_Task         gnpBootTask;

// args: ///////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Args(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  vAddPrimitive("args.print"_Stz,      &vArgsPrint,     false);
}

/*----------------------------------------------------------------------------*/
/* args.print -- print arguments                                              */
/*----------------------------------------------------------------------------*/
void Task::vArgsPrint(MP_Task mpTask)
{
ENTRY("vArgsPrint"_Stz);
  
  gxArguments.vPrint();
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
