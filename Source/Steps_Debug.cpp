/*============================================================================*/
/* Steps_Debug.cpp - debug step group                                         */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern AttachMap*      gpAttachMap;
extern BufferPool*     gpBufferPool;
extern BPA*            gpBPA;
extern Dic*            gpMainDic;
extern Word*           gpWord;

extern Grid*           gpGrid;
extern StepArray*      gpStepArray;
extern ThreadList*     gpThreadList;
extern TaskList*       gpTaskList;

extern NP_Task         gnpBootTask;
extern Lock            gxDdlLock;
extern Lock            gxDicLock;
extern SEitfList*      gpSEitfList;

// debug: //////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Debug(void)
{
  vAddPrimitive("debug::trap"_Stz,      &vDebugTrap,        false);
}

/*----------------------------------------------------------------------------*/
/* debug::trap -- call debug trap                                              */
/*----------------------------------------------------------------------------*/
void Task::vDebugTrap(MP_Task mpTask)
{
ENTRY("vDebugTrap"_Stz);

  Debugging::vDebugTrap();
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
