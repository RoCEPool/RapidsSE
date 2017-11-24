/*============================================================================*/
/* Steps_DicLock.cpp - dic lock step group                                    */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern StepArray*      gpStepArray;
extern ThreadList*     gpThreadList;
extern TaskList*       gpTaskList;

extern NP_Task         gnpBootTask;
extern Lock            gxDdlLock;
extern Lock            gxDicLock;
extern SEitfList*      gpSEitfList;

// diclock: ////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_DicLock(void)
{
  vAddPrimitive("dicLock.dropWrite"_Stz,&vDicLockDropWrite, false);
  vAddPrimitive("dicLock.grabWrite"_Stz,&vDicLockGrabWrite, false);
}

/*----------------------------------------------------------------------------*/
/* diclLock.dropWrite -- drop dic lock for write                              */
/*----------------------------------------------------------------------------*/
void Task::vDicLockDropWrite(MP_Task mpTask)
{
  ENTRY("vDicLockDropWrite"_Stz);
  
  gxDicLock.vDropWrite();
  
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dicLock.grabWrite -- grab dic lock for write                               */
/*----------------------------------------------------------------------------*/
void Task::vDicLockGrabWrite(MP_Task mpTask)
{
  ENTRY("vDicLockGrabWrite"_Stz);
  
  gxDicLock.vGrabWrite();
    
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
