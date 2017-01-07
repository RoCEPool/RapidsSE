/*============================================================================*/
/* Task_groups.cpp - task groups                                              */
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

// diclock: ////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_DicLock(void)
{
  vAddPrimitive("dicLock.dropWrite",&vDicLockDropWrite, false);
  vAddPrimitive("dicLock.grabWrite",&vDicLockGrabWrite, false);
}

/*----------------------------------------------------------------------------*/
/* diclLock.dropWrite -- drop dic lock for write                              */
/*----------------------------------------------------------------------------*/
void Task::vDicLockDropWrite(NNP_Task nnpTask)
{
  ENTRY("vDicLockDropWrite");
  
  gxDicLock.vDropWrite();
  
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dicLock.grabWrite -- grab dic lock for write                               */
/*----------------------------------------------------------------------------*/
void Task::vDicLockGrabWrite(NNP_Task nnpTask)
{
  ENTRY("vDicLockGrabWrite");
  
  gxDicLock.vGrabWrite();
    
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
