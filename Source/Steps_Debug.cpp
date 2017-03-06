/*============================================================================*/
/* Steps_Debug.cpp - debug step group                                         */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern Arguments       gxArguments;

extern AttachMap*      gpAttachMap;
extern BufferPool*     gpBufferPool;
extern Heap*           gpMediumHeap;
extern BPA*            gpBPA;
extern Dic*            gpMainDic;
extern Word*           gpWord;

extern Grid*           gpGrid;
extern StepArray*      gpStepArray;
extern ThreadList*     gpThreadList;
extern TaskList*       gpTaskList;
extern TxCollection*   gpTxCollection;

extern NP_Task         gnpRootTask;
extern StepBoard*      gpStepBoard;
extern Lock            gxDdlLock;
extern Lock            gxDicLock;
extern SEitfList*      gpSEitfList;

// debug: //////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Debug(void)
{
  vAddPrimitive("debug.trap"_Str,      &vDebugTrap,        false);
}

/*----------------------------------------------------------------------------*/
/* debug.trap -- call debug trap                                              */
/*----------------------------------------------------------------------------*/
void Task::vDebugTrap(NNP_Task nnpTask)
{
ENTRY("vDebugTrap");

  Debugging::vDebugTrap();
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
